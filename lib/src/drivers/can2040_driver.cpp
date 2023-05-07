#include "neotje/drivers/can2040_driver.hpp"

#include "hardware/irq.h"
#include "hardware/pio.h"
#include "RP2040.h"

#include <string.h>
#include <stdlib.h>

#ifndef can2040_driver_hpp
#include "can2040_driver.hpp"
#endif

struct can2040 can2040_driver::cbus;
can2040_driver* can2040_driver::instance = nullptr;
std::queue<can_frame_t*> can2040_driver::rx_queue;

struct can2040_msg temp_msg;

void can_frame_t_to_can2040_msg(can_frame_t* can_frame, struct can2040_msg* can2040_msg)
{
    can2040_msg->id = can_frame->id;
    can2040_msg->dlc = can_frame->dlc;

    if (can_frame->extended)
        can2040_msg->id |= CAN2040_ID_EFF;

    if (can_frame->rtr) {
        can2040_msg->id |= CAN2040_ID_RTR;
        return;
    }

    memcpy(can2040_msg->data, can_frame->data, can_frame->dlc);
}

void can2040_msg_to_can_frame_t(struct can2040_msg* can2040_msg, can_frame_t* can_frame)
{
    can_frame->id = can2040_msg->id & 0x1FFFFFFF;
    can_frame->dlc = can2040_msg->dlc;

    can_frame->extended = true ? can2040_msg->id & CAN2040_ID_EFF : false;
    can_frame->rtr = true ? can2040_msg->id & CAN2040_ID_RTR : false;

    if (can_frame->data != nullptr) {
        free(can_frame->data);
    }

    can_frame->data = (uint8_t*)malloc(can_frame->dlc);

    memcpy(can_frame->data, can2040_msg->data, can_frame->dlc);
}

can2040_driver::can2040_driver()
{
}

can2040_driver::~can2040_driver()
{
}

void can2040_driver::irqhandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

void can2040_driver::can2040_cb(can2040* cb, uint32_t notify, can2040_msg* msg)
{
    can_frame_t* can_frame = (can_frame_t*)malloc(sizeof(can_frame_t));
    can_frame->data = nullptr;

    can2040_msg_to_can_frame_t(msg, can_frame);

    rx_queue.push(can_frame);
}

int can2040_driver::send_message(can_frame_t* message)
{
    if (!can2040_check_transmit(&cbus)) {
        return 0;
    }

    can_frame_t_to_can2040_msg(message, &temp_msg);

    return can2040_transmit(&cbus, &temp_msg) != -1;
}

int can2040_driver::receive_message(can_frame_t* message)
{
    if (rx_queue.empty()) {
        return 0;
    }

    can_frame_t* can_frame = rx_queue.front();
    rx_queue.pop();

    message->id = can_frame->id;
    message->dlc = can_frame->dlc;
    message->extended = can_frame->extended;
    message->rtr = can_frame->rtr;

    message->data = can_frame->data;
    free(can_frame);

    return 1;
}

can2040_driver* can2040_driver::get_instance(uint32_t can_bitrate, int rx_pin, int tx_pin)
{
    // check if instance is already created
    if (instance != nullptr) {
        return instance;
    }

    instance = new can2040_driver();

    // initialize can2040
    can2040_setup(&cbus, PIO_NUM);
    can2040_callback_config(&cbus, can2040_cb);

    // enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0_IRQn, irqhandler);
    NVIC_SetPriority(PIO0_IRQ_0_IRQn, 1);
    NVIC_EnableIRQ(PIO0_IRQ_0_IRQn);

    can2040_start(&cbus, SYSTEM_CLOCK_FREQUENCY, can_bitrate, rx_pin, tx_pin);

    return instance;
}

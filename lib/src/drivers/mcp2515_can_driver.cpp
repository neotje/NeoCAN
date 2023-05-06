#include "neotje/drivers/mcp2515_can_driver.hpp"
#include "neotje/mcp2515_timing.h"
#include "neotje/mcp2515_registers.h"
#include "neotje/mcp2515_bits.h"
#include <stdio.h>

#ifndef mcp2515_can_driver_hpp
#include "mcp2515_can_driver.hpp"
#endif

void mcp2515_can_driver::setup()
{
    this->init();

    //int brp = mcp2515_timing_brp(8000000, 6, this->config.baudrate);

    //this->write_register(CNF1, brp & BRP_MASK);
    //this->write_register(CNF2, BV(BLTMODE) | BV(PHSEG10)); // set phase segment 1 to 2Tqs
    //this->write_register(CNF3, BV(SOF) | BV(PHSEG20)); // set SOF to 1Tq

    this->write_register(CNF1, 0x00);
    this->write_register(CNF2, 0x80); // set phase segment 1 to 2Tqs
    this->write_register(CNF3, 0x80); // set SOF to 1Tq

    // set RXM to 00 Receives all vallid messages using either standard or extended identifiers that meet filter criteria.
    // set BUKT to 1 to enable rollover to RXB1 if RXB0 is full.
    //this->bit_modify(RXB0CTRL, RXM_MASK | BV(BUKT) | BV(FILHIT0), BV(BUKT)); 
    this->bit_modify(RXB0CTRL, BV(RXM1) | BV(RXM0) | BV(BUKT) | BV(FILHIT0), BV(RXM1) | BV(RXM0));

    // set RXM to 00 Receives all vallid messages using either standard or extended identifiers that meet filter criteria.
    //this->bit_modify(RXB1CTRL, RXM_MASK | BM(3, FILHIT0), 0 | BV(FILHIT0));
    this->bit_modify(RXB1CTRL, BV(RXM1) | BV(RXM0) | BM(4, FILHIT0), BV(FILHIT0) | BV(RXM1) | BV(RXM0));

    // clear all filters and masks
    for (size_t i = 0; i < 6; i++)
    {
        this->set_filter(i, true, 0);
    }

    for (size_t i = 0; i < 2; i++)
    {
        this->set_filter_mask(i, true, 0);
    }

    // set mode to normal
    this->set_mode(NORMAL_MODE);
}

mcp2515_can_driver::mcp2515_can_driver(mcp2515_config_t *config) : mcp2515(config)
{
    this->setup();
}

mcp2515_can_driver::mcp2515_can_driver(spi_inst_t *port, uint miso, uint mosi, uint sck, uint cs, uint baudrate): mcp2515(port, miso, mosi, sck, cs, baudrate) {
    this->setup();
}

int mcp2515_can_driver::send_message(can_frame_t *message)
{
    uint8_t txbuffers[] = {TXB0CTRL, TXB1CTRL, TXB2CTRL};
    uint8_t status_map[] = {BV(2), BV(4), BV(6)};

    uint8_t status = this->read_status();

    for (int txbn = 0; txbn < sizeof(txbuffers); txbn++) {
        if (!(status & status_map[txbn])) {
            return this->send_can_frame(txbn, message);
        }
    }

    return 0;
}

int mcp2515_can_driver::receive_message(can_frame_t *message)
{
    uint8_t status = this->read_status();

    if (status & BV(RX0IF)) {
        return this->read_can_frame(0, message);
    }

    if (status & BV(RX1IF)) {
        return this->read_can_frame(1, message);
    } 

    return 0;
}

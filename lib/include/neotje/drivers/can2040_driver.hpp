#ifndef can2040_driver_hpp
#define can2040_driver_hpp

#include "../abstract_can_driver.hpp"

#include <queue>

extern "C" {
    #include "can2040.h"
}

#define SYSTEM_CLOCK_FREQUENCY 125000000
#define PIO_NUM 0

class can2040_driver : public abstract_can_driver {
    private:
        static struct can2040 cbus;
        static can2040_driver *instance;
        static std::queue<can_frame_t*> rx_queue;

        static void can2040_cb(struct can2040 *cb, uint32_t notify, struct can2040_msg *msg);
        static void irqhandler(void);

    protected:
        can2040_driver();
        ~can2040_driver();

    public:
        // Inherited from abstract_can_driver
        int send_message(can_frame_t *message);
        int receive_message(can_frame_t *message);

        static can2040_driver *get_instance(uint32_t can_bitrate, int rx_pin, int tx_pin);
};

#endif
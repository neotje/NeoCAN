#include "neotje/drivers/can2040_driver.hpp"
#include "pico/stdlib.h"
#include <cstdio>

int main() {
    stdio_init_all();

    can2040_driver* driver = can2040_driver::get_instance(1000000, 2, 3);

    can_frame_t frame;

    while (true)
    {
        if (driver->receive_message(&frame)) {
            printf("Received id: %d, data:", frame.id);

            for (int i = 0; i < frame.dlc; i++) {
                printf(" %x", frame.data[i]);
            }
            printf("\n");
        } else {
            printf("Failed to receive message\n");
        }
    }

    return 0;
}
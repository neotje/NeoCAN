#include "neotje/drivers/can2040_driver.hpp"
#include "pico/stdlib.h"
#include <cstdio>

int main() {
    stdio_init_all();

    can2040_driver* driver = can2040_driver::get_instance(1000000, 2, 3);

    char data[] = "Hoi";
    can_frame_t frame = {
        .id = 1,
        .rtr = false,
        .extended = false,
        .dlc = sizeof(data),
        .data = (uint8_t*)data
    };

    while (true)
    {
        if (driver->send_message(&frame)) {
            printf("Sent id: %d, data:", frame.id);

            for (int i = 0; i < frame.dlc; i++) {
                printf(" %x", frame.data[i]);
            }
            printf("\n");
        } else {
            printf("Failed to send message\n");
        }

        frame.id++;
        sleep_ms(1000);
    }


    return 0;
}
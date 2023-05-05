#include <pico/stdlib.h>
#include <stdio.h>

#include "neotje/drivers/mcp2515_can_driver.hpp"
#include "neotje/mcp2515_registers.h"

int main() {
    stdio_init_all();

    mcp2515_can_driver driver(spi0, 20, 19, 18, 21, 10 * 1000 * 1000);

    printf("%d\n", driver.read_register(0x2B));
    
    uint8_t rx0_mask[4];
    driver.read_registers(RXM0SIDH, rx0_mask, sizeof(rx0_mask));
    uint8_t rx1_mask[4];
    driver.read_registers(RXM1SIDH, rx1_mask, sizeof(rx1_mask));

    // print both rx masks
    printf("RX0 Mask: ");
    for (int i = 0; i < sizeof(rx0_mask); i++) {
        printf("%02x ", rx0_mask[i]);
    }
    printf("\n");

    printf("RX1 Mask: ");
    for (int i = 0; i < sizeof(rx1_mask); i++) {
        printf("%02x ", rx1_mask[i]);
    }
    printf("\n");

    char data[] = "Hoi";
    can_frame_t frame = {
        .id = 1,
        .rtr = false,
        .extended = false,
        .dlc = sizeof(data),
        .data = (uint8_t*)data
    };

    int result = driver.send_message(&frame);

    if (!result) {
        printf("Error: %d\n", result);
        return 1;
    }

    sleep_ms(1000);

    result = driver.receive_message(&frame);

    if (!result) {
        printf("Error: %d\n", result);
        return 1;
    }

    printf("Received: %s\n", frame.data);

    return 0;
}
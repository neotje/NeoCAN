#include "neotje/drivers/mcp2515_can_driver.hpp"
#include "neotje/neo_can_node.hpp"
#include "neotje/protocols/non_organizer.hpp"
#include "pico/stdlib.h"
#include "neotje/mcp2515_registers.h"

int main() {
    stdio_init_all();

    mcp2515_can_driver driver(spi0, 20, 19, 18, 21, 1 * 1000 * 1000);

    neo_can_node node(&driver, 128);

    non_organizer organizer;

    node.add_protocol(&organizer);

    char data[] = "Hoi";
    can_frame_t frame = {
        .id = 1,
        .rtr = false,
        .extended = false,
        .dlc = sizeof(data),
        .data = (uint8_t*)data
    };

    int result = driver.send_message(&frame);

    while(true) {
        node.loop();
        printf("%d\n", driver.read_register(CANINTE));
    }

    return 0;
}
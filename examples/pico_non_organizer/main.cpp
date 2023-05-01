#include "neotje/drivers/mcp2515_can_driver.hpp"
#include "neotje/neo_can_node.hpp"
#include "neotje/protocols/non_organizer.hpp"
#include "pico/stdlib.h"

int main() {
    stdio_init_all();

    mcp2515_can_driver driver(spi0, 20, 19, 18, 21, 10 * 1000 * 1000);

    neo_can_node node(&driver, 128);

    non_organizer organizer;

    node.add_protocol(&organizer);

    while(true) {
        node.loop();
    }

    return 0;
}
#include "neotje/drivers/mcp2515_can_driver.hpp"
#include "neotje/drivers/can2040_driver.hpp"
#include "neotje/neo_can_node.hpp"
#include "neotje/protocols/non_organizer.hpp"
#include "pico/stdlib.h"
#include "neotje/mcp2515_registers.h"

int main() {
    stdio_init_all();

    printf("Hello, world!\n");

    //mcp2515_can_driver *driver = new mcp2515_can_driver(spi0, 20, 19, 18, 21, 1 * 1000 * 1000);
    //can2040_driver *driver = can2040_driver::get_instance(1000000, 2, 3);
    can2040_driver *driver = can2040_driver::get_instance(125000, 2, 3);

    neo_can_node node(driver, NON_ORGANIZER_ID);

    non_organizer organizer;

    node.add_protocol(&organizer);

    while(true) {
        node.loop();
    }

    return 0;
}
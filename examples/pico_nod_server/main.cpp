#include "neotje/drivers/can2040_driver.hpp"
#include "neotje/neo_can_node.hpp"
#include "neotje/protocols/nod_server.hpp"

int main() {
    can2040_driver *driver = can2040_driver::get_instance(125000, 2, 3);

    neo_can_node node(driver, 1);

    nod_server server;

    node.add_protocol(&server);

    server.init_dict_entry(0, 8008, NOD_VAL_ACCESS_R);
    server.init_dict_entry(1, 0, NOD_VAL_ACCESS_RW);
    server.init_dict_entry(2, 0, NOD_VAL_ACCESS_W);

    while(true) {
        node.loop();
    }

    return 0;
}
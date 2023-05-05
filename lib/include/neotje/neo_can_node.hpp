#ifndef neo_can_node_hpp
#define neo_can_node_hpp

#include "abstract_can_driver.hpp"
#include "abstract_can_protocol.hpp"
#include "abstract_node.hpp"

#include <stdio.h>

#include <vector>

class neo_can_node: public abstract_node
{
private:
    abstract_can_driver *driver;
    std::vector<abstract_can_protocol *> protocols;

    uint16_t node_id;

public:
    neo_can_node(abstract_can_driver *a_driver, uint16_t a_node_id);
    ~neo_can_node();

    abstract_can_driver *get_driver();

    int add_protocol(abstract_can_protocol *protocol);

    void set_node_id(uint16_t node_id);
    uint16_t get_node_id();

    void loop();
};

#endif
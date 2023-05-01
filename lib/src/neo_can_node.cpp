#include "neotje/neo_can_node.hpp"

#ifndef neo_can_node_hpp
#include "neo_can_node.hpp"
#endif

neo_can_node::neo_can_node(abstract_can_driver *a_driver, uint16_t a_node_id) : driver(a_driver), node_id(a_node_id)
{
}

neo_can_node::~neo_can_node()
{
}

abstract_can_driver *neo_can_node::get_driver()
{
    return this->driver;
}

int neo_can_node::add_protocol(abstract_can_protocol *protocol)
{
    if (protocol == NULL || !protocol->set_parent_node(this))
        return 0;

    protocols.push_back(protocol);

    return 1;
}

void neo_can_node::set_node_id(uint16_t node_id)
{
    this->node_id = node_id;
}

uint16_t neo_can_node::get_node_id()
{
    return this->node_id;
}

void neo_can_node::loop()
{
    can_frame_t frame;

    for (auto protocol : protocols)
        protocol->loop();

    if (this->driver->receive_message(&frame))
        for (auto protocol : protocols)
            protocol->on_message(&frame);
}

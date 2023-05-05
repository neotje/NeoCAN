#include "neotje/abstract_can_protocol.hpp"

abstract_can_protocol::abstract_can_protocol() : parent_node(nullptr)
{
}

abstract_can_protocol::~abstract_can_protocol()
{
}

int abstract_can_protocol::set_parent_node(abstract_node *parent_node)
{
    if (this->parent_node != nullptr) return 0;

    this->parent_node = parent_node;

    return 1;
}

abstract_node *abstract_can_protocol::get_parent_node()
{
    return this->parent_node;
}

#ifndef abstract_node_hpp
#define abstract_node_hpp

#include "abstract_can_driver.hpp"

class abstract_node {
public:
    virtual abstract_can_driver* get_driver() = 0;

    virtual void set_node_id(uint16_t node_id) = 0;
    virtual uint16_t get_node_id() = 0;
};

#endif
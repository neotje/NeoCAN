#ifndef abstract_node_hpp
#define abstract_node_hpp

#include "abstract_can_driver.hpp"

typedef uint16_t node_id_t;

class abstract_node {
public:
    virtual abstract_can_driver* get_driver() = 0;

    virtual void set_node_id(node_id_t node_id) = 0;
    virtual node_id_t get_node_id() = 0;
};

#endif
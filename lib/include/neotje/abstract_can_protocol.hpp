#ifndef abstract_can_protocol_hpp
#define abstract_can_protocol_hpp

#include "abstract_node.hpp"

#include "neotje/can.h"

class abstract_can_protocol
{
private:
    abstract_node *parent_node;

public:
    abstract_can_protocol();
    ~abstract_can_protocol();

    int set_parent_node(abstract_node *parent_node);
    abstract_node *get_parent_node();

    virtual void on_message(can_frame_t *frame) = 0;
    virtual void setup() = 0;
    virtual void loop() = 0;
};

#endif
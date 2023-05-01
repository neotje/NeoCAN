#ifndef abstract_can_protocol_hpp
#define abstract_can_protocol_hpp

#include "neotje/can.h"

class neo_can_node;

class abstract_can_protocol
{
private:
    neo_can_node *parent_node;

public:
    abstract_can_protocol();
    ~abstract_can_protocol();

    int set_parent_node(neo_can_node *parent_node);
    neo_can_node *get_parent_node();

    virtual void on_message(can_frame_t *frame) = 0;
    virtual void loop() = 0;
};

#endif
#ifndef non_participant_hpp
#define non_participant_hpp

#include "neotje/abstract_can_protocol.hpp"
#include "non_globals.h"

#include "pico/time.h"

class non_participant : public abstract_can_protocol
{
private:
    non_priority_level_t priority;
    uint32_t uuid;

    struct repeating_timer timer;
    static bool send_init_timer_cb(struct repeating_timer* t);

public:
    non_participant(non_priority_level_t priority);
    ~non_participant() {};

    /* Inherited from abstract_can_protocol */
    void setup();
    void loop();
    void on_message(can_frame_t* frame);

    void start_timer();
    void stop_timer();

    int send_init();
};

#endif
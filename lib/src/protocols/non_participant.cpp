#include "neotje/protocols/non_participant.hpp"
#include "neotje/debug.h"
#include "neotje/can.h"

#include "pico/unique_id.h"

#include <cassert>
#include <cstdio>

#ifndef non_participant_hpp
#include "non_participant.hpp"
#endif

non_participant::non_participant(non_priority_level_t priority)
{
    this->priority = priority;

    pico_unique_board_id_t board_id;
    pico_get_unique_board_id(&board_id);

    assert((uint64_t)board_id.id != 0xee);

    this->uuid = (uint32_t)board_id.id;

    printf("Created non_participant with priority %d and uuid %d\n", priority, uuid);
}

bool non_participant::send_init_timer_cb(struct repeating_timer* t)
{
    non_participant* self = (non_participant*)t->user_data;

    if (!self->send_init())
    {
        printf("Failed to send INIT message\n");
    }

    return true;
}

void non_participant::setup()
{
    // Start timer
    start_timer();
}

void non_participant::loop()
{
}

void non_participant::on_message(can_frame_t* frame)
{
    // check if the message is from the organizer
    if (can_get_sid(frame) == NON_ORGANIZER_ID && frame->extended)
    {
        uint32_t eid = can_get_eid(frame);

        // Recieved REINIT message
        if (eid == REINIT_EID && frame->rtr && frame->dlc == sizeof(this->uuid))
        {
            printf("Recieved REINIT message from organizer. Resetting node id\n");
            start_timer();
            return;
        }

        // Recieved ASSIGN message for this nodes priority
        if (eid == this->priority && frame->dlc == sizeof(non_participant_t))
        {
            non_participant_t* par = (non_participant_t*)frame->data;

            // is for me?
            if (par->uuid == this->uuid)
            {
                printf("Recieved ASSIGN message from organizer. New nodeid: %d\n", par->node_id);

                stop_timer();

                // Set node id
                this->get_parent_node()->set_node_id(par->node_id);
            }
        }
    }
}

void non_participant::start_timer()
{
    // Start timer
    add_repeating_timer_ms(NON_PARTICIPANT_SEND_INTERVAL, &this->send_init_timer_cb, this, &this->timer);
}

void non_participant::stop_timer()
{
    // Stop timer
    cancel_repeating_timer(&this->timer);
}

int non_participant::send_init()
{
    can_frame_t init_frame = {
        .id = CAN_SID_EID_TO_UINT(NON_ORGANIZER_ID, (uint)this->priority),
        .rtr = false,
        .extended = true,
        .dlc = sizeof(this->uuid),
        .data = (uint8_t*)&this->uuid
    };

    printf("Sending INIT message with uuid %d and priority %d\n", this->uuid, this->priority);

    return get_parent_node()->get_driver()->send_message(&init_frame);
}


#include "neotje/protocols/non_organizer.hpp"
#include "neotje/can.h"

#include <stdio.h>
#include <memory.h>

int get_priority_min(non_priority_level_t prio)
{
    switch (prio)
    {
    case EMERGENCY_PRIORITY:
        return EMERGENCY_PRIORITY_MIN;
    case HIGH_PRIORITY:
        return HIGH_PRIORITY_MIN;
    case NORMAL_PRIORITY:
        return NORMAL_PRIORITY_MIN;
    case LOW_PRIORITY:
        return LOW_PRIORITY_MIN;
    default:
        return -1;
    }
}

non_organizer::non_organizer()
{
}

void non_organizer::on_message(can_frame_t *frame)
{
    if (can_get_sid(frame) == 128 && frame->extended)
    {
        uint32_t eid = can_get_eid(frame);

        if (eid >= 1 && eid <= 3 && frame->dlc == 4)
        {
            uint32_t uuid;

            memcpy(&uuid, frame->data, sizeof(uuid));

            this->init_participant(uuid, (non_priority_level_t)(eid - 1));
        }
    }
}

int non_organizer::new_node_id_in_priority(non_priority_level_t priority)
{
    int highest = get_priority_min(priority);

    for (auto participant : participants)
    {
        switch (priority)
        {
        case EMERGENCY_PRIORITY:
            if (participant.node_id >= EMERGENCY_PRIORITY_MIN && participant.node_id <= EMERGENCY_PRIORITY_MAX)
                if (participant.node_id > highest)
                    highest = participant.node_id + 1;
            break;

        case HIGH_PRIORITY:
            if (participant.node_id >= HIGH_PRIORITY_MIN && participant.node_id <= HIGH_PRIORITY_MAX)
                if (participant.node_id > highest)
                    highest = participant.node_id + 1;
            break;
        
        case NORMAL_PRIORITY:
            if (participant.node_id >= NORMAL_PRIORITY_MIN && participant.node_id <= NORMAL_PRIORITY_MAX)
                if (participant.node_id > highest)
                    highest = participant.node_id + 1;
            break;

        case LOW_PRIORITY:
            if (participant.node_id >= LOW_PRIORITY_MIN && participant.node_id <= LOW_PRIORITY_MAX)
                if (participant.node_id > highest)
                    highest = participant.node_id + 1;
            break;
        };
    }

    return highest;
}

const non_participant_t *non_organizer::get_participant(non_uuid_t uuid)
{
    for (auto participant : participants)
        if (participant.uuid == uuid)
            return &participant;

    return nullptr;
}

const non_participant_t *non_organizer::init_participant(non_uuid_t uuid, non_priority_level_t priority)
{   
    const non_participant_t *participant = this->get_participant(uuid);
    if (participant != nullptr)
        return participant;

    int new_node_id = this->new_node_id_in_priority(priority);

    if (new_node_id == -1)
        return nullptr;

    this->participants.push_back({uuid, (uint16_t)new_node_id});

    return &this->participants.back();
}
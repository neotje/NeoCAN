#include "neotje/protocols/non_organizer.hpp"
#include "neotje/can.h"
#include "neotje/debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    if (can_get_sid(frame) == this->get_parent_node()->get_node_id() && frame->extended)
    {
        uint32_t eid = can_get_eid(frame);

        // Recieved INIT message
        if (eid >= 1 && eid <= 3 && frame->dlc == 4)
        {   
            // Collect assign data
            uint32_t uuid;
            memcpy(&uuid, frame->data, sizeof(uuid));
            non_priority_level_t prio = (non_priority_level_t)(eid - 1);

            debug_print("Recieved INIT message from node UUID %d and priority %d\n", uuid, prio);

            const non_participant_t *participant = this->init_participant(uuid, prio);

            // Send ASSIGN message
            if (participant) this->send_assign(participant, prio);
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
            if (participant->node_id >= EMERGENCY_PRIORITY_MIN && participant->node_id <= EMERGENCY_PRIORITY_MAX)
                if (participant->node_id > highest)
                    highest = participant->node_id + 1;
            break;

        case HIGH_PRIORITY:
            if (participant->node_id >= HIGH_PRIORITY_MIN && participant->node_id <= HIGH_PRIORITY_MAX)
                if (participant->node_id > highest)
                    highest = participant->node_id + 1;
            break;

        case NORMAL_PRIORITY:
            if (participant->node_id >= NORMAL_PRIORITY_MIN && participant->node_id <= NORMAL_PRIORITY_MAX)
                if (participant->node_id > highest)
                    highest = participant->node_id + 1;
            break;

        case LOW_PRIORITY:
            if (participant->node_id >= LOW_PRIORITY_MIN && participant->node_id <= LOW_PRIORITY_MAX)
                if (participant->node_id > highest)
                    highest = participant->node_id + 1;
            break;
        };
    }

    return highest;
}

const non_participant_t *non_organizer::get_participant(non_uuid_t uuid)
{
    for (auto participant : participants)
        if (participant->uuid == uuid)
            return participant;

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

    non_participant_t *new_participant = (non_participant_t *)malloc(sizeof(non_participant_t));
    new_participant->uuid = uuid;
    new_participant->node_id = (uint16_t)new_node_id;
    this->participants.push_back(new_participant);

    debug_print("Assigning newly added node UUID %d to node ID %d\n", uuid, new_node_id);

    return this->participants.back();
}

int non_organizer::send_assign(const non_participant_t *participant, non_priority_level_t prio)
{
    can_frame_t assign_frame = {
        .id = CAN_SID_EID_TO_UINT(get_parent_node()->get_node_id(), (uint)(1 + prio)),
        .rtr = false,
        .extended = true,
        .dlc = sizeof(*participant),
        .data = (uint8_t*)participant
    };

    debug_print("Sending ASSIGN message to node UUID %d with node ID %d\n", participant->uuid, participant->node_id);

    return get_parent_node()->get_driver()->send_message(&assign_frame);
}
#ifndef non_organizer_hpp
#define non_organizer_hpp

#include "neotje/abstract_can_protocol.hpp"
#include "non_globals.h"

#include <vector>

class non_organizer : public abstract_can_protocol
{
private:
    std::vector<non_participant_t*> participants;
public:
    non_organizer();

    void loop(){};
    void on_message(can_frame_t *frame);

    int new_node_id_in_priority(non_priority_level_t priority);
    const non_participant_t *get_participant(non_uuid_t uuid);
    const non_participant_t *init_participant(non_uuid_t uuid, non_priority_level_t priority);

    int send_assign(const non_participant_t *participant, non_priority_level_t prio);
};

#endif
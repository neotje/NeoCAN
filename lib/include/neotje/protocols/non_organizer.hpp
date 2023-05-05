#ifndef non_organizer_hpp
#define non_organizer_hpp

#include "neotje/abstract_can_protocol.hpp"

#include <vector>

#define EMERGENCY_PRIORITY_MIN 0
#define EMERGENCY_PRIORITY_MAX 127
#define HIGH_PRIORITY_MIN 129
#define HIGH_PRIORITY_MAX 299
#define NORMAL_PRIORITY_MIN 300
#define NORMAL_PRIORITY_MAX 1999
#define LOW_PRIORITY_MIN 2000
#define LOW_PRIORITY_MAX 3777

typedef uint32_t non_uuid_t;

typedef struct
{
    non_uuid_t uuid;
    uint16_t node_id;
} non_participant_t;

typedef enum
{
    EMERGENCY_PRIORITY,
    HIGH_PRIORITY,
    NORMAL_PRIORITY,
    LOW_PRIORITY
} non_priority_level_t;

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
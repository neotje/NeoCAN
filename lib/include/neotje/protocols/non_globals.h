#ifndef non_globals_h
#define non_globals_h

#include <cstdint>

#define EMERGENCY_PRIORITY_MIN 0
#define EMERGENCY_PRIORITY_MAX 127
#define NON_ORGANIZER_ID 128
#define HIGH_PRIORITY_MIN 129
#define HIGH_PRIORITY_MAX 299
#define NORMAL_PRIORITY_MIN 300
#define NORMAL_PRIORITY_MAX 1999
#define LOW_PRIORITY_MIN 2000
#define LOW_PRIORITY_MAX 3777

#define UNKOWN_NODE_ID UINT16_MAX

#define REINIT_EID 0

#define NON_PARTICIPANT_SEND_INTERVAL 1000

typedef uint32_t non_uuid_t;

typedef struct __attribute__ ((packed))
{
    non_uuid_t uuid;
    node_id_t node_id;
} non_participant_t;

typedef enum
{
    EMERGENCY_PRIORITY = 1, // eid
    HIGH_PRIORITY = 2,
    NORMAL_PRIORITY = 3,
    LOW_PRIORITY = 4
} non_priority_level_t;

#endif
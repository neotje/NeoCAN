#ifndef abstract_can_driver_hpp
#define abstract_can_driver_hpp

#include "neotje/can.h"

class abstract_can_driver {
    public:
        virtual int send_message(can_frame_t *message) = 0;
        virtual int receive_message(can_frame_t *message) = 0;
};

#endif
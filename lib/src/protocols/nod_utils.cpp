#include "neotje/protocols/nod_utils.hpp"

#include "neotje/protocols/nod_globals.h"

bool dict_entry_is_writeable(nod_dict_entry_t *entry) {
    return entry->access == NOD_VAL_ACCESS_W || entry->access == NOD_VAL_ACCESS_RW;
}

bool dict_entry_is_readable(nod_dict_entry_t *entry) {
    return entry->access == NOD_VAL_ACCESS_R || entry->access == NOD_VAL_ACCESS_RW;
}

nod_message_type_t get_message_type(can_frame_t *frame, bool is_server) {
    if (!frame->extended)
        return NOD_MSG_UNKMOWN;

    int address = can_get_eid(frame) - NOD_START_EID;

    if (address < 0 || address >= NOD_DICTIONARY_LEN)
        return NOD_MSG_UNKMOWN;

    if (is_server) {
        if (frame->dlc == 2)
            return NOD_MSG_REQUEST;
        else if (frame->dlc == 6)
            return NOD_MSG_SET_VALUE;
        else
            return NOD_MSG_UNKMOWN;
    }

    if (frame->dlc == 6)
        return NOD_MSG_RESPONSE;
    else if (frame->dlc == 3)
        return NOD_MSG_ERROR;
    
    return NOD_MSG_UNKMOWN;
}
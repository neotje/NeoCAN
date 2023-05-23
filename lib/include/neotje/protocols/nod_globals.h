#ifndef nod_globals_h
#define nod_globals_h

#include "neotje/can.h"

#include <inttypes.h>

typedef uint32_t nod_value_t;

#define NOD_START_EID 0
#define NOD_DICTIONARY_SIZE 1024
#define NODE_VALUE_SIZE sizeof(nod_value_t)

#define NOD_ADDRESS_TO_EID(address) (address + NOD_START_EID)

const int NOD_DICTIONARY_LEN = NOD_DICTIONARY_SIZE / NODE_VALUE_SIZE;

typedef enum {
    NOD_VAL_ACCESS_R = 0,
    NOD_VAL_ACCESS_W = 1,
    NOD_VAL_ACCESS_RW = 2
} nod_value_access_t;

typedef enum {
    UNABLE_TO_SET_VALUE = 0,
    UNABLE_TO_GET_VALUE = 1,
    UNKOWN_ADDRESS = 2,
} nod_error_codes_t;

typedef enum {
    NOD_MSG_REQUEST = 0,
    NOD_MSG_RESPONSE = 1,
    NOD_MSG_SET_VALUE = 2,
    NOD_MSG_ERROR = 3,
    NOD_MSG_UNKMOWN = 4
} nod_message_type_t;

typedef struct {
    nod_value_t value;
    nod_value_access_t access;
} nod_dict_entry_t;

typedef struct __attribute__((__packed__)) {
    node_id_t client_id;
} nod_request_t;

typedef struct __attribute__((__packed__)) {
    node_id_t client_id;
    nod_value_t value;
} nod_set_value_t;

typedef struct __attribute__((__packed__)) {
    node_id_t client_id;
    nod_value_t value;
} nod_response_t;

typedef struct __attribute__((__packed__)) {
    node_id_t client_id;
    uint8_t error_code;
} nod_error_t;

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

#endif
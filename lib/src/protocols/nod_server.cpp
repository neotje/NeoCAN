#include "neotje/protocols/nod_server.hpp"
#include "neotje/protocols/nod_utils.hpp"
#include "neotje/can.h"

nod_server::nod_server()
{
}

bool nod_server::init_dict_entry(int address, nod_value_t value = 0, nod_value_access_t access = NOD_VAL_ACCESS_RW)
{
    if (address >= NOD_DICTIONARY_LEN)
        return false;

    if (dict.count(address) != 0)
        return false;

    nod_dict_entry_t *entry = new nod_dict_entry_t;
    entry->value = value;
    entry->access = access;

    dict[address] = entry;

    return true;
}

bool nod_server::address_exists(int address)
{
    return dict.count(address) != 0;
}

bool nod_server::address_is_readable(int address)
{
    return address_exists(address) && dict_entry_is_readable(dict[address]);
}

bool nod_server::address_is_writeable(int address)
{
    return address_exists(address) && dict_entry_is_writeable(dict[address]);
}

void nod_server::set_value(int address, nod_value_t value)
{
    if(address_is_writeable(address))
        dict[address]->value = value;
}

nod_value_t nod_server::get_value(int address)
{
    if(address_is_readable(address))
        return dict[address]->value;

    return 0;
}

void nod_server::handle_request(can_frame_t *frame)
{
    int address = can_get_eid(frame) - NOD_START_EID;
    nod_request_t *body = (nod_request_t*)frame->data;

    if (!address_exists(address)) {
        send_error(body->client_id, address, UNKOWN_ADDRESS);
        return;
    }

    if (!address_is_readable(address)) {
        send_error(body->client_id, address, UNABLE_TO_GET_VALUE);
        return;
    }

    send_response(body->client_id, address, get_value(address));
}

void nod_server::handle_set_value(can_frame_t *frame)
{
    int address = can_get_eid(frame) - NOD_START_EID;
    nod_set_value_t *body = (nod_set_value_t*)frame->data;

    if (!address_exists(address)) {
        send_error(body->client_id, address, UNKOWN_ADDRESS);
        return;
    }

    if (!address_is_writeable(address)) {
        send_error(body->client_id, address, UNABLE_TO_SET_VALUE);
        return;
    }

    set_value(address, body->value);
}

int nod_server::send_response(uint16_t client_id, int address, nod_value_t value)
{
    nod_response_t response = {
        .client_id = client_id,
        .value = value
    };

    can_frame_t response_frame = {
        .id = CAN_SID_EID_TO_UINT(get_parent_node()->get_node_id(), NOD_ADDRESS_TO_EID(address)),
        .rtr = false,
        .extended = true,
        .dlc = sizeof(nod_response_t),
        .data = (uint8_t*)(&response)
    };

    return get_parent_node()->get_driver()->send_message(&response_frame);
}

int nod_server::send_error(uint16_t client_id, int address, nod_error_codes_t error_code)
{
    nod_error_t error = {
        .client_id = client_id,
        .error_code = error_code
    };

    can_frame_t error_frame = {
        .id = CAN_SID_EID_TO_UINT(get_parent_node()->get_node_id(), NOD_ADDRESS_TO_EID(address)),
        .rtr = false,
        .extended = true,
        .dlc = sizeof(nod_error_t),
        .data = (uint8_t*)(&error)
    };

    return get_parent_node()->get_driver()->send_message(&error_frame);
}

void nod_server::setup()
{
}

void nod_server::loop()
{
}

void nod_server::on_message(can_frame_t *frame)
{
    if (can_get_sid(frame) != this->get_parent_node()->get_node_id())
        return;

    switch (get_message_type(frame, true))
    {
    case NOD_MSG_REQUEST:
        handle_request(frame);
        break;

    case NOD_MSG_SET_VALUE:
        handle_set_value(frame);
        break;
    
    default:
        break;
    }
}
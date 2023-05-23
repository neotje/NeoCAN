#ifndef nod_server_hpp
#define nod_server_hpp

#include <unordered_map>

#include "neotje/abstract_can_protocol.hpp"
#include "nod_globals.h"

class nod_server : public abstract_can_protocol
{
private:
    std::unordered_map<int, nod_dict_entry_t*> dict;

public:
    nod_server();
    ~nod_server() {};

    bool init_dict_entry(int address, nod_value_t value, nod_value_access_t access);

    bool address_exists(int address);
    bool address_is_readable(int address);
    bool address_is_writeable(int address);

    void set_value(int address, nod_value_t value);
    nod_value_t get_value(int address);

    void handle_request(can_frame_t* frame);
    void handle_set_value(can_frame_t* frame);

    int send_response(uint16_t client_id, int address, nod_value_t value);
    int send_error(uint16_t client_id, int address, nod_error_codes_t error_code);

    /* Inherited from abstract_can_protocol */
    void setup();
    void loop();
    void on_message(can_frame_t* frame);
};

#endif
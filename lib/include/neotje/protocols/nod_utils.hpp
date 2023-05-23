#ifndef nod_utils_h
#define nod_utils_h

#include "neotje/protocols/nod_globals.h"

bool dict_entry_is_writeable(nod_dict_entry_t *entry);

bool dict_entry_is_readable(nod_dict_entry_t *entry);

nod_message_type_t get_message_type(can_frame_t *frame, bool is_server);

#endif
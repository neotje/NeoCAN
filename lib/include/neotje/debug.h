#ifndef debug_h
#define debug_h

#ifdef DEBUG
#define debug_print(...) printf(__VA_ARGS__)
#else
#define debug_print(...)
#endif

#endif
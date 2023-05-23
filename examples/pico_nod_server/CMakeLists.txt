project(pico_nod_server C CXX ASM)

add_executable(pico_nod_server
${CMAKE_CURRENT_LIST_DIR}/main.cpp)

pico_set_program_name(pico_nod_server "pico_nod_server")
pico_set_program_version(pico_nod_server "0.1")
    
pico_enable_stdio_uart(pico_nod_server 1)
pico_enable_stdio_usb(pico_nod_server 1)
    
target_link_libraries(pico_nod_server 
    pico_stdlib
    pico_bootsel_via_double_reset
    neotje_NeoCAN)
    
pico_add_extra_outputs(pico_nod_server)
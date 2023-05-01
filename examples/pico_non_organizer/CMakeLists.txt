project(pico_non_organizer C CXX ASM)

add_executable(pico_non_organizer
${CMAKE_CURRENT_LIST_DIR}/main.cpp)

pico_set_program_name(pico_non_organizer "pico_non_organizer")
pico_set_program_version(pico_non_organizer "0.1")
    
pico_enable_stdio_uart(pico_non_organizer 1)
pico_enable_stdio_usb(pico_non_organizer 0)
    
target_link_libraries(pico_non_organizer 
    pico_stdlib
    neotje_NeoCAN)
    
pico_add_extra_outputs(pico_non_organizer)
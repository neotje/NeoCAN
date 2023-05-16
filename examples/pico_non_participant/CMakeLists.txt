project(pico_non_participant C CXX ASM)

add_executable(pico_non_participant
${CMAKE_CURRENT_LIST_DIR}/main.cpp)

pico_set_program_name(pico_non_participant "pico_non_participant")
pico_set_program_version(pico_non_participant "0.1")
    
pico_enable_stdio_uart(pico_non_participant 1)
pico_enable_stdio_usb(pico_non_participant 1)
    
target_link_libraries(pico_non_participant 
    pico_stdlib
    pico_bootsel_via_double_reset
    neotje_NeoCAN)
    
pico_add_extra_outputs(pico_non_participant)
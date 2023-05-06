project(can2040_driver_test_listener C CXX ASM)

add_executable(can2040_driver_test_listener
${CMAKE_CURRENT_LIST_DIR}/listener.cpp)

pico_set_program_name(can2040_driver_test_listener "can2040_driver_test_listener")
pico_set_program_version(can2040_driver_test_listener "0.1")
    
pico_enable_stdio_uart(can2040_driver_test_listener 1)
pico_enable_stdio_usb(can2040_driver_test_listener 0)
    
target_link_libraries(can2040_driver_test_listener 
    pico_stdlib
    neotje_NeoCAN)
    
pico_add_extra_outputs(can2040_driver_test_listener)



project(can2040_driver_test_sender C CXX ASM)

add_executable(can2040_driver_test_sender
${CMAKE_CURRENT_LIST_DIR}/sender.cpp)

pico_set_program_name(can2040_driver_test_sender "can2040_driver_test_sender")
pico_set_program_version(can2040_driver_test_sender "0.1")
    
pico_enable_stdio_uart(can2040_driver_test_sender 1)
pico_enable_stdio_usb(can2040_driver_test_sender 0)
    
target_link_libraries(can2040_driver_test_sender 
    pico_stdlib
    neotje_NeoCAN)
    
pico_add_extra_outputs(can2040_driver_test_sender)
pico_add_subdirectory(mcp2515_driver/lib)

# import can2040 as pico library
if (NOT TARGET KevinOConnor_can20400)
    pico_add_library(KevinOConnor_can20400)

    target_include_directories(KevinOConnor_can20400_headers INTERFACE ${CMAKE_CURRENT_LIST_DIR}/can2040/src)
    target_sources(KevinOConnor_can20400 INTERFACE ${CMAKE_CURRENT_LIST_DIR}/can2040/src/can2040.c)

    pico_mirrored_target_link_libraries(KevinOConnor_can20400 INTERFACE
            hardware_dma
            hardware_pio
            cmsis_core
            hardware_irq
        )
endif()

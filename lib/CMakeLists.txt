if (NOT TARGET neotje_NeoCAN)
    pico_add_library(neotje_NeoCAN)
    target_include_directories(neotje_NeoCAN_headers 
        INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include
        INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include/drivers)

    file(GLOB_RECURSE neocan_src_files ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

    target_sources(neotje_NeoCAN INTERFACE
        ${neocan_src_files})

    pico_mirrored_target_link_libraries(neotje_NeoCAN INTERFACE
        pico_stdlib
        neotje_mcp2515
        KevinOConnor_can20400
        pico_unique_id
        pico_time)
    
    pico_add_doxygen(${CMAKE_CURRENT_LIST_DIR}/include)
endif()
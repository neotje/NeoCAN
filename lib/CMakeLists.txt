if (NOT TARGET neotje_NeoCAN)
    pico_add_library(neotje_NeoCAN)
    target_include_directories(neotje_NeoCAN_headers INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

    target_sources(neotje_NeoCAN INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/NeoCAN.cpp)

    pico_mirrored_target_link_libraries(neotje_NeoCAN INTERFACE
        pico_stdlib)
    
    pico_add_doxygen(${CMAKE_CURRENT_LIST_DIR}/include)
endif()
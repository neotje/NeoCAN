cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_SDK_PATH "~/pico/pico-sdk")
set(PICO_BOARD pico CACHE STRING "Board type")

include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(NeoCAN-project C CXX ASM)

pico_sdk_init()
pico_add_subdirectory(lib)
pico_add_subdirectory(deps/mcp2515_driver/lib)

add_executable(NeoCAN-project
  NeoCAN.cpp
)

pico_set_program_name(NeoCAN-project "NeoCAN-project")
pico_set_program_version(NeoCAN-project "0.1")
pico_enable_stdio_uart(NeoCAN-project 1)
pico_enable_stdio_usb(NeoCAN-project 0)

target_link_libraries(NeoCAN-project
        pico_stdlib
        neotje_NeoCAN)

target_include_directories(NeoCAN-project PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/..)

pico_add_extra_outputs(NeoCAN-project)

add_subdirectory(examples)
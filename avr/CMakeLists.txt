cmake_minimum_required(VERSION 3.0.0)
project(SmartChargeDevice CXX)

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

include_directories(${ARDUINO_BOARD_BUILD_CORE_PATH})
add_executable(SmartChargeDevice src/main.cpp)
target_link_arduino_libraries(SmartChargeDevice PRIVATE core)
target_enable_arduino_upload(SmartChargeDevice)
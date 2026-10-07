cmake_minimum_required(VERSION 4.4)
project(GCalendar_rest_api)

set(CMAKE_CXX_STANDARD 20)

add_executable(GCalendar_rest_api main.cpp
        Secrets.h)

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(GCalendar_rest_api PRIVATE cpr::cpr nlohmann_json::nlohmann_json)


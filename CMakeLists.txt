cmake_minimum_required(VERSION 3.5)

project(calendar-creator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  cli
  GIT_REPOSITORY https://github.com/daniele77/cli.git
  GIT_TAG v2.1.0
)
FetchContent_MakeAvailable(cli)

find_package(LibIcal REQUIRED)

add_executable(calendar-creator
    src/main.cpp
    src/calendar/calendar.hpp src/calendar/calendar.cpp
    src/calendar/event.hpp src/calendar/event.cpp
    src/calendar/profilemanager.hpp src/calendar/profilemanager.cpp
    src/calendar/app.hpp src/calendar/app.cpp
    src/rapidcsv.h
    src/date.h
    src/calendar/icsfile.hpp src/calendar/icsfile.cpp
    src/calendar/helpers.hpp src/calendar/helpers.cpp
)

target_include_directories(calendar-creator PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${LibIcal_DIR}
)

target_link_libraries(calendar-creator PRIVATE
    cli::cli
    ical
)


cmake_minimum_required(VERSION 2.6)
project(monitor)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp") # From: https://cmake.org/cmake/help/v3.0/variable/PROJECT_SOURCE_DIR.html & https://stackoverflow.com/a/15551302

add_executable(monitor src/main.cpp ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})

target_compile_options(monitor PRIVATE -Wall -Wextra -Werror)
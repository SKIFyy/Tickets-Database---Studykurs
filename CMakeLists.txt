cmake_minimum_required(VERSION 3.16)
project(TicketsDatabase LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(ticket_core Functions.cpp)
target_include_directories(ticket_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(TicketsDatabase MAIN1.cpp)
target_link_libraries(TicketsDatabase PRIVATE ticket_core)

if(MSVC)
    target_compile_options(ticket_core PRIVATE /utf-8 /W4)
    target_compile_options(TicketsDatabase PRIVATE /utf-8 /W4)
else()
    target_compile_options(ticket_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(TicketsDatabase PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(MINGW)
    target_link_options(TicketsDatabase PRIVATE -static -static-libgcc -static-libstdc++)
endif()

configure_file(sample.csv sample.csv COPYONLY)

include(CTest)
if(BUILD_TESTING)
    add_executable(ticket_tests tests.cpp)
    target_link_libraries(ticket_tests PRIVATE ticket_core)
    add_test(NAME ticket_tests COMMAND ticket_tests)
endif()

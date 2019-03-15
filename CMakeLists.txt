cmake_minimum_required(VERSION 3.0.0)

project(binaryen-pass-example VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Werror -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -Og")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)

include(ExternalProject)
include(binaryen.cmake)

add_executable(binaryen-pass-example
    main.cpp
)

target_link_libraries(binaryen-pass-example
    libbinaryen
)

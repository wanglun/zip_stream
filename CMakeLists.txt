cmake_minimum_required(VERSION 2.8.10)
project(zip_stream C CXX)

option(BUILD_UNIT_TESTS "Whether to build unit tests" OFF)
option(BUILD_C++11 "Whether to build with C++11" OFF)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

find_path(ZLIB_INCLUDE_PATH zlib.h)
find_library(ZLIB_LIBRARY NAMES libz.a)
if((NOT ZLIB_INCLUDE_PATH) OR (NOT ZLIB_LIBRARY))
    message(FATAL_ERROR "Fail to find libz")
endif()
include_directories(${ZLIB_INCLUDE_PATH})

find_path(MINIZIP_INCLUDE_PATH unzip.h)
find_library(MINIZIP_LIBRARY NAMES libminizip.a)
if((NOT MINIZIP_INCLUDE_PATH) OR (NOT MINIZIP_LIBRARY))
    message(FATAL_ERROR "Fail to find minizip")
endif()
include_directories(${MINIZIP_INCLUDE_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CPP_FLAGS} -DNDEBUG -O2 -D__const__= -pipe -Werror -W -Wall -Wextra -Wno-unused-parameter -fPIC -fno-omit-frame-pointer")

if (BUILD_C++11)
    if(CMAKE_VERSION VERSION_LESS "3.1.3")
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        endif()
        if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        endif()
    else()
        set(CMAKE_CXX_STANDARD 11)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
    endif()
endif()

set(LIBS
    ${MINIZIP_LIBRARY} # must before ZLIB_LIBRARY
    ${ZLIB_LIBRARY}
    )

add_subdirectory(src)

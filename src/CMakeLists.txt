set(SRCS
    ${PROJECT_SOURCE_DIR}/src/zip_stream.cpp
    )
add_library(zip_stream STATIC ${SRCS})

if (BUILD_UNIT_TESTS)
    include(FindThreads)

    find_path(GTEST_INCLUDE_PATH gtest/gtest.h)
    find_library(GTEST_LIBRARY NAMES libgtest.a)
    find_library(GTEST_MAIN_LIBRARY NAMES libgtest_main.a)
    if((NOT GTEST_INCLUDE_PATH) OR (NOT GTEST_LIBRARY) OR (NOT GTEST_MAIN_LIBRARY))
        message(FATAL_ERROR "Fail to find gtest")
    endif()
    include_directories(${GTEST_INCLUDE_PATH})

    add_executable(zip_stream_test ${PROJECT_SOURCE_DIR}/src/zip_stream_test.cpp)
    target_link_libraries(zip_stream_test 
        zip_stream
        ${GTEST_MAIN_LIBRARY}
        ${GTEST_LIBRARY}
        ${LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
        )
endif()

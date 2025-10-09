cmake_minimum_required(VERSION 3.15)
project(MyTasks C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

add_library(utils src/utils.c src/utils_2.c)
target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(utils m)

if(DEFINED TASK)
    message(STATUS "Configuring only task: ${TASK}")
    add_subdirectory(${CMAKE_SOURCE_DIR}/tasks/${TASK})
else()
    file(GLOB children RELATIVE ${CMAKE_SOURCE_DIR}/tasks ${CMAKE_SOURCE_DIR}/tasks/*)
    foreach(child ${children})
        if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/tasks/${child})
            add_subdirectory(${CMAKE_SOURCE_DIR}/tasks/${child})
        endif()
    endforeach()
endif()

# --- GoogleTest via FetchContent ---
include(CTest)
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
    TLS_VERIFY false
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
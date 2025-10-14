add_library(task1_lib parser.c task_math.c)
target_include_directories(task1_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(task1_lib PUBLIC utils)

add_executable(task1 main.c)
target_link_libraries(task1 PRIVATE utils task1_lib)

add_executable(task1_tests
    task1_tests.cpp
)

target_link_libraries(task1_tests
    PRIVATE
    gtest_main
    task1_lib
    utils
    m
)

target_include_directories(task1_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_dependencies(task1_tests task1)

enable_testing()
add_test(NAME Task1IntegrationTests COMMAND task1_tests)
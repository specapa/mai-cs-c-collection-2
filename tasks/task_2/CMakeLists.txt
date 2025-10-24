add_library(task2_lib notstring.c)
target_include_directories(task2_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(task2_lib PUBLIC utils)


add_executable(task2_tests
    task2_tests.c
)

add_executable(task2_tests2
    task2_tests2.c
)

target_link_libraries(task2_tests
    PRIVATE
    task2_lib
    m
)

target_link_libraries(task2_tests2
    PRIVATE
    task2_lib
    m
)

target_include_directories(task2_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_test(NAME Task2IntegrationTests COMMAND task2_tests)
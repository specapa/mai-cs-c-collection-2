add_library(task3_lib helpers.c overprintf.c)
target_include_directories(task3_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(task3_lib PUBLIC utils)


add_executable(task3_tests
    task3_tests.c
)

add_executable(task3_def
    task3_default.c
)

target_link_libraries(task3_tests
    PRIVATE
    task3_lib
    m
)


target_link_libraries(task3_def
    PRIVATE
    task3_lib
    m
)

target_include_directories(task3_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(task3_def PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_test(NAME Task3IntegrationTests COMMAND task3_tests)
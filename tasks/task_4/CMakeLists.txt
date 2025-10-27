add_library(task4_lib helpers.c overscanf.c)
target_include_directories(task4_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(task4_lib PUBLIC utils)


add_executable(task4_tests
    task4_tests.c
)

add_executable(task4_def
    task4_def.c
)

target_link_libraries(task4_tests
    PRIVATE
    task4_lib
    m
)


target_link_libraries(task4_def
    PRIVATE
    task4_lib
    m
)

target_include_directories(task4_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(task4_def PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_test(NAME Task4IntegrationTests COMMAND task4_tests)
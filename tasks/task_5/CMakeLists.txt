add_library(task5_lib process_file.c)
target_include_directories(task5_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(task5_lib PUBLIC utils)

add_executable(task5 main.c)
target_link_libraries(task5 PRIVATE utils task5_lib)

# add_executable(task5_tests
#     task5_tests.cpp
# )

# target_link_libraries(task5_tests
#     PRIVATE
#     gtest_main
#     task5_lib
#     utils
#     m
# )

# target_include_directories(task5_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# add_dependencies(task5_tests task5)

# enable_testing()
# add_test(NAME Task5IntegrationTests COMMAND task5_tests)
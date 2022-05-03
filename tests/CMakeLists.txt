
add_executable(testlib testlib.c)
target_link_libraries(testlib PRIVATE gamesh_library)

add_executable(test_con_queue test_con_queue.c)
target_link_libraries(test_con_queue PRIVATE gamesh_library)

add_executable(test_msg_aggregator test_msg_aggregator.c)
target_link_libraries(test_msg_aggregator PRIVATE gamesh_library)

# If you register a test, then ctest and make test will run it.
# You can also run examples and check the output, as well.
add_test(NAME test_lib_test COMMAND testlib) # Command can be a target
add_test(NAME test_con_queue_test COMMAND test_con_queue) # Command can be a target
add_test(NAME test_msg_aggregator_test COMMAND test_msg_aggregator) # Command can be a target

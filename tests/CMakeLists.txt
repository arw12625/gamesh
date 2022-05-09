
SET(BASE_TEST_LIST "con_queue" "message" "template_event" "image_rec" "human_input")
SET(INTERACTIVE_TEST_LIST "window" "misc")

foreach(T IN LISTS BASE_TEST_LIST)
add_executable("test_${T}" "test_${T}.c")
target_link_libraries("test_${T}" PRIVATE gamesh_library log_library glfw glad nuklear_library)
add_test(NAME "test_${T}_test" COMMAND "test_${T}")
# Set labels for running tests that match a given regex
set_tests_properties("test_${T}_test" PROPERTIES LABELS "base")
endforeach()

foreach(T IN LISTS INTERACTIVE_TEST_LIST)
add_executable("test_${T}" "test_${T}.c")
target_link_libraries("test_${T}" PRIVATE gamesh_library log_library glfw glad nuklear_library)
add_test(NAME "test_${T}_test" COMMAND "test_${T}") 
set_tests_properties("test_${T}_test" PROPERTIES LABELS "interactive")
endforeach()


#add_executable(testlib testlib.c)
#target_link_libraries(testlib PRIVATE gamesh_library)
#add_test(NAME test_lib_test COMMAND testlib) # Command can be a target


add_executable(app app.c)

target_link_libraries(app PRIVATE gamesh_library log_library Threads::Threads glad OpenGL::GL  glfw)

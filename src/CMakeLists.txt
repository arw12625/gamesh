# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

set(HEADER_LIST "con_queue.h" "message.h" "template_event.h" "image_rec.h" "human_input.h" "window.h" "nuklear_glfw_glad_gl4.h")
list(TRANSFORM HEADER_LIST PREPEND "${GaMesh_SOURCE_DIR}/include/gamesh/")

# Make an automatic library - will be static or dynamic based on user setting
add_library(gamesh_library con_queue.c message.c template_event.c image_rec.c human_input.c window.c ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(gamesh_library PUBLIC ../include)

# This depends on (header only) boost
target_link_libraries(gamesh_library PRIVATE log_library stb_library glfw nuklear_library)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

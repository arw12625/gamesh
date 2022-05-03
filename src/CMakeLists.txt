#add_definitions(-DDEBUG)

# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

set(HEADER_LIST "lib.h" "con_queue.h" "msg_aggregator.h")
list(TRANSFORM HEADER_LIST PREPEND "${GaMesh_SOURCE_DIR}/include/gamesh/")

# Make an automatic library - will be static or dynamic based on user setting
add_library(gamesh_library lib.c con_queue.c msg_aggregator.c ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(gamesh_library PUBLIC ../include)

# This depends on (header only) boost
#target_link_libraries(gamesh_library PRIVATE glad)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

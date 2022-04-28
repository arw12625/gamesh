# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

set(HEADER_LIST "${GaMesh_SOURCE_DIR}/include/gamesh/lib.h")

# Make an automatic library - will be static or dynamic based on user setting
add_library(gamesh_library lib.c ${HEADER_LIST})

# We need this directory, and users of our library will need it too
target_include_directories(gamesh_library PUBLIC ../include)

# This depends on (header only) boost
#target_link_libraries(gamesh_library PRIVATE glad)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})

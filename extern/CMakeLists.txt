
#logging
# Make an object library
add_library(log_library OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/log.c/src/log.c" "${CMAKE_CURRENT_SOURCE_DIR}/log.c/src/log.h")

# We need this directory, and users of our library will need it too
target_include_directories(log_library PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/log.c/src/")

#stb
# Make an interface library
add_library(stb_library INTERFACE)
target_include_directories(stb_library INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb")


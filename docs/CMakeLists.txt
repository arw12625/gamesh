set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)

doxygen_add_docs(docs gamesh/lib.h gamesh/con_queue.h gamesh/msg_aggregator.h "${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md"
                 WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/include")

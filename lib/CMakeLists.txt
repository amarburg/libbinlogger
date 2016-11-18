
# Set source files
set(logger_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/LogReader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/LogWriter.cpp
)

# build shared library.
add_library(logger SHARED ${logger_SOURCE_FILES})
target_link_libraries(logger
  ${LIBLOGGER_LIBS}
)

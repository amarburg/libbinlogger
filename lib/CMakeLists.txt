
# Set source files
set(logger_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/LogReader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/LogWriter.cpp
)


include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/lib
  ${OpenCV_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  ${CONAN_INCLUDE_DIRS}
)

# build shared library.
add_library(logger SHARED ${logger_SOURCE_FILES})
target_link_libraries(logger
  ${Boost_LIBRARIES}
  ${OpenCV_LIBS}
  ${CONAN_LIBS}
)

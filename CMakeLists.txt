cmake_minimum_required(VERSION 2.8.7)
project(liblogger)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

#list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set( OPENCV_VERSION 2.4 )
list(APPEND OPENCV_COMPONENTS core highgui imgproc video)
find_package(OpenCV ${OPENCV_VERSION} COMPONENTS ${OPENCV_COMPONENTS} REQUIRED)

find_package(Boost COMPONENTS thread filesystem system REQUIRED)


## C++ flags applied to all builds
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

## Set global include paths
set( LIBLOGGER_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/include
	${CONAN_INCLUDE_DIRS}
)
include_directories( ${LIBLOGGER_INCLUDE_DIRS} )
add_subdirectory("lib/")

#add_subdirectory("tools")

option( BUILD_UNIT_TESTS "Build unit tests" OFF )
if( BUILD_UNIT_TESTS )
  add_subdirectory( test/unit/ )
endif()

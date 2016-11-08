cmake_minimum_required(VERSION 2.8.7)
project(liblogger)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

#list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set( OPENCV_VERSION 2.4 )
list(APPEND OPENCV_COMPONENTS core highgui imgproc video)
find_package(OpenCV ${OPENCV_VERSION} COMPONENTS ${OPENCV_COMPONENTS} REQUIRED)

find_package(Boost COMPONENTS thread filesystem system REQUIRED)
# find_package(Eigen3 REQUIRED)


EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
message( STATUS "Architecture: ${ARCHITECTURE}" )
if( ${ARCHITECTURE} STREQUAL "aarch64" )
	# This is needed to work around a problem in Jetpack 2.3 / Cuda 8.0
	SET( CUDA_USE_STATIC_CUDA_RUNTIME OFF )
elseif( ${ARCHITECTURE} STREQUAL "x86_64" )
	#
endif()

##====== Support for Google Snappy compression lib ==================
# option( USE_GOOGLE_SNAPPY "Use Google Snappy" ON )
# if( USE_GOOGLE_SNAPPY )
# 	include( ${PROJECT_SOURCE_DIR}/cmake/BuildGoogleSnappy.cmake )
# endif()

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

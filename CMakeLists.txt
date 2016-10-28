cmake_minimum_required(VERSION 2.8.7)
project(liblogger)
include(ExternalProject)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set( OPENCV_VERSION 2.4 )
list(APPEND OPENCV_COMPONENTS core highgui imgproc video)
find_package(OpenCV ${OPENCV_VERSION} COMPONENTS ${OPENCV_COMPONENTS} REQUIRED)

find_package(Boost COMPONENTS thread filesystem system REQUIRED)
# find_package(Eigen3 REQUIRED)

find_package(TCLAP REQUIRED )
find_package(ZLIB REQUIRED)

include( ${PROJECT_SOURCE_DIR}/cmake/BuildG3Log.cmake )

set( EXTERNAL_PROJECTS g3log )
## Command used to build the external projects
set( EXTERNAL_PROJECT_PARALLELISM 4 CACHE STRING "Level of parallelism when making ExternalProjects" )
set( EXTERNAL_PROJECT_MAKE_COMMAND "make" )
if( EXTERNAL_PROJECT_PARALLELISM GREATER 0 )
	message( "Building ExternalProjects in Parallel, to disable set EXTERNAL_PROJECT_PARALLELISM to 0")
	set( EXTERNAL_PROJECT_MAKE_COMMAND make -j${EXTERNAL_PROJECT_PARALLELISM} )
endif()


##====== Support for Google Snappy compression lib ==================
option( USE_GOOGLE_SNAPPY "Use Google Snappy" ON )
if( USE_GOOGLE_SNAPPY )
	include( ${PROJECT_SOURCE_DIR}/cmake/BuildGoogleSnappy.cmake )
	list( APPEND EXTERNAL_PROJECTS snappy )
endif()

## C++ flags applied to all builds
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


if(APPLE)
	set( CMAKE_MACOSX_RPATH OFF )
endif()

## Set global include paths
set( LIBLOGGER_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/include
	${LIBACTIVE_OBJECT_INCLUDE_DIRS}
	${G3LOG_INCLUDE_DIR}
)
  # ${EIGEN3_INCLUDE_DIR}
  # ${OpenCV_INCLUDE_DIRS}
  # ${Boost_INCLUDE_DIR}
#)

include( ${PROJECT_SOURCE_DIR}/cmake/BuildLibActiveObject.cmake )

message( ${LIBLOGGER_INCLUDE_DIRS})
include_directories( ${LIBLOGGER_INCLUDE_DIRS} )



add_subdirectory("lib/")

#add_subdirectory("tools")

option( BUILD_UNIT_TESTS "Build unit tests" ON )
if( BUILD_UNIT_TESTS )
  ## Build a local copy of gtest (as per the GTest FAQ)
  include( ${CMAKE_SOURCE_DIR}/cmake/BuildGTestLibraries.cmake )
	list( APPEND EXTERNAL_PROJECTS gtest )

  add_subdirectory( test/unit/ )
endif()

add_custom_target( deps DEPENDS ${EXTERNAL_PROJECTS} )

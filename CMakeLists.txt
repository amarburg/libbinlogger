cmake_minimum_required(VERSION 2.8.7)
project(liblogger)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Boost COMPONENTS thread filesystem system REQUIRED)

# Tired of screwing around with this
set( CUDA_USE_STATIC_CUDA_RUNTIME OFF CACHE BOOL "" FORCE  )

set( OPENCV_VERSION 2.4 )
set( OPENCV_COMPONENTS core highgui imgproc video)
find_package(OpenCV ${OPENCV_VERSION} COMPONENTS ${OPENCV_COMPONENTS} REQUIRED)

## OpenCV will look for CUDA if we need it, but if it does,
## we need to re-run the CMake scripts to disable the static CUDA runtime
# if( CUDA_VERSION )
# 	message("Found CUDA version ${CUDA_VERSION}")
# 	if(${CUDA_VERSION} VERSION_EQUAL "8.0")
# 		message("Disabling CUDA_USE_STATIC_CUDA_RUNTIME")
# 	endif()
#
# 	find_package( CUDA REQUIRED ${CUDA_VERSION} )
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

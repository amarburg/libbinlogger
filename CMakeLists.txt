cmake_minimum_required(VERSION 2.8.7)
project(liblogger)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package( Threads )
find_package(Boost COMPONENTS thread filesystem system REQUIRED)
set( ZLIB_ROOT /usr )
find_package( ZLIB REQUIRED )
message( "ZLIB at ${ZLIB_LIBRARIES}")

find_package( CUDA )
if( CUDA_VERSION )
	message("Found CUDA version ${CUDA_VERSION}")
	if(${CUDA_VERSION} VERSION_EQUAL "8.0")
		message("Disabling CUDA_USE_STATIC_CUDA_RUNTIME")
		set( CUDA_USE_STATIC_CUDA_RUNTIME OFF CACHE BOOL "" FORCE  )
	endif()
  find_package( CUDA )
endif()

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

## Having odd problem under Sierra where /usr/local/ isn't in default search paths...
if( APPLE )
  include_directories( /usr/local/include )
  link_directories( /usr/local/lib )
endif()

## Set global include paths
set( LIBLOGGER_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/include
	${ZLIB_INCLUDE_DIRS}
	${OpenCV_INCLUDE_DIRS}
	${CONAN_INCLUDE_DIRS}
)
include_directories( ${LIBLOGGER_INCLUDE_DIRS} )

set( LIBLOGGER_LIBS
	${CMAKE_THREAD_LIBS_INIT}
	${Boost_LIBRARIES}
	${OpenCV_LIBS}
	${ZLIB_LIBRARIES}
	${CONAN_LIBS} )

add_subdirectory("lib/")

#add_subdirectory("tools")

option( BUILD_UNIT_TESTS "Build unit tests" OFF )
if( BUILD_UNIT_TESTS )
  add_subdirectory( test/unit/ )
endif()

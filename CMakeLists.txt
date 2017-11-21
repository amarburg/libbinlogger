cmake_minimum_required(VERSION 3.5)
get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include("${FIPS_ROOT_DIR}/cmake/fips.cmake")

set(FIPS_EXCEPTIONS ON)
fips_setup(PROJECT liblogger)

find_package(Boost COMPONENTS thread filesystem system REQUIRED)

# set( ZLIB_ROOT /usr )
# find_package( ZLIB REQUIRED )
# message( "ZLIB at ${ZLIB_LIBRARIES}")

# find_package( CUDA )
# if( CUDA_VERSION )
# 	message("Found CUDA version ${CUDA_VERSION}")
# 	if(${CUDA_VERSION} VERSION_EQUAL "8.0")
# 		message("Disabling CUDA_USE_STATIC_CUDA_RUNTIME")
# 		set( CUDA_USE_STATIC_CUDA_RUNTIME OFF CACHE BOOL "" FORCE  )
# 	endif()
#   find_package( CUDA )
# endif()

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
# if( APPLE )
#   include_directories( /usr/local/include )
#   link_directories( /usr/local/lib )
# endif()

## Set global include paths
set( LIBLOGGER_INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/include
)
include_directories( ${LIBLOGGER_INCLUDE_DIRS} )

set( LIBLOGGER_LIBS
 )

fips_add_subdirectory("lib/")

#add_subdirectory("tools")

#option( BUILD_UNIT_TESTS "Build unit tests" OFF )
#if( BUILD_UNIT_TESTS )
fips_add_subdirectory("test/unit/")
#endif()

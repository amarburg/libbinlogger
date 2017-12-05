if( NOT FIPS_IMPORT)
  cmake_minimum_required(VERSION 3.5)
  get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
  include("${FIPS_ROOT_DIR}/cmake/fips.cmake")

  fips_setup(PROJECT liblogger)
endif()

find_package(Boost COMPONENTS thread filesystem system REQUIRED)

#set( ZLIB_ROOT /usr )
find_package( ZLIB REQUIRED )

set( OPENCV_COMPONENTS core highgui imgproc video)
find_package(OpenCV COMPONENTS ${OPENCV_COMPONENTS} REQUIRED)

if( OpenCV_VERSION_MAJOR EQUAL "3" )
  message( STATUS "Using OpenCV 3.x")
  add_definitions( -DOPENCV3 )
else()
  message( STATUS "Using OpenCV 2.x")
  add_definitions( -DOPENCV2 )
endif()

## OpenCV will look for CUDA if we need it, but if it does,
## we need to re-run the CMake scripts to disable the static CUDA runtime
if( CUDA_VERSION )
	message("Found CUDA version ${CUDA_VERSION}")
	if(${CUDA_VERSION} VERSION_EQUAL "8.0")
		message(STATUS "Disabling CUDA_USE_STATIC_CUDA_RUNTIME in liblogger")
    SET( CUDA_USE_STATIC_CUDA_RUNTIME OFF CACHE BOOL "" FORCE )
	endif()

	find_package( CUDA REQUIRED ${CUDA_VERSION} )
endif()

## C++ flags applied to all builds
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


## Set global include paths
set( LIBLOGGER_INCLUDE_DIRS
  ${OpenCV_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/include
)
fips_include_directories( ${LIBLOGGER_INCLUDE_DIRS} )

#set( LIBLOGGER_LIBS  ${OpenCV_LIBS} )

fips_add_subdirectory("lib/")

if( FIPS_UNITTESTS )
  fips_add_subdirectory( test/unit/ )

  if( FIPS_UNITTESTS_RUN_AFTER_BUILD )
    ## TODO
  endif()
endif()

if( NOT FIPS_IMPORT )
  fips_finish()
endif()

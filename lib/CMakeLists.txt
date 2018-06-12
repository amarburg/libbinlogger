find_package(Boost COMPONENTS thread filesystem system REQUIRED)

# Set source files
set(logger_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/LogReader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/LogWriter.cpp
)

# build shared library.
fips_begin_module(binlogger)
  fips_files(
    LogReader.cpp
    LogWriter.cpp
  )
  fips_libs( ${Boost_LIBRARIES} ${ZLIB_LIBRARIES} ${OpenCV_LIBS})
  fips_deps( logger active_object )
fips_end_module()

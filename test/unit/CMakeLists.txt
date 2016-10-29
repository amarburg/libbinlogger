include_directories( ${GTEST_INCLUDE_DIRS}
                     ${CMAKE_SOURCE_DIR}/lib )

FILE(GLOB UNIT_TEST_SRCS *.cpp )

add_executable( logger_test
              ${UNIT_TEST_SRCS} )

target_link_libraries( logger_test
          ${GTEST_BOTH_LIBRARIES}
          active_object )
#
# # Don't use ctest, instead just execture the Google test binaries
add_custom_target( unit_test ${PROJECT_BINARY_DIR}/logger_test
                    DEPENDS logger_test )

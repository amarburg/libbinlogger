FILE(GLOB UNIT_TEST_SRCS *.cpp )

add_executable( logger_test
              ${UNIT_TEST_SRCS} )

target_link_libraries( logger_test
          ${LIBLOGGER_LIBS} )
#
# # Don't use ctest, instead just execture the Google test binaries
add_custom_target( unit_test
                    COMMAND ./logger_test
                    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/bin
                    DEPENDS logger_test )

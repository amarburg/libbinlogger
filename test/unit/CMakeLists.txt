FILE(GLOB UNIT_TEST_SRCS *.cpp )

gtest_begin(binlogger)
    fips_files(
            ${UNIT_TEST_SRCS}
    )
    fips_deps(logger active_object)
gtest_end()

# # Don't use ctest, instead just execture the Google test binaries
# add_custom_target( unit_test
#                     COMMAND ./logger_test
#                     WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/bin
#                     DEPENDS logger_test )

include( TestHelpers.cmake NO_POLICY_SCOPE )

enable_testing( ON )
INCLUDE(CTest)
IF(BUILD_TESTING)
  add_custom_target( RUN_TESTS_WITH_OUTPUT
				COMMAND ${CMAKE_CTEST_COMMAND} ${CMAKE_BINARY_DIR} --output-on-failure --force-new-ctest-process
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR} )
ENDIF(BUILD_TESTING)

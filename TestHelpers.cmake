cmake_minimum_required(VERSION 2.6)

function( add_boost_test_suite TARGET_NAME TIMEOUT )

  if( CROSS )
    message( FATAL_ERROR "Tests are disallowed in cross-compilation" )
  endif( CROSS )

  set( FILE_NAMES ${ARGN} )
  
  # create test suite executable
  add_executable( ${TARGET_NAME} 
				${FILE_NAMES} )
  target_link_libraries( ${TARGET_NAME}
						${Boost_LIBRARIES} )
						
  add_custom_target( ${TARGET_NAME}_run
				COMMAND ${TARGET_NAME}
				WORKING_DIRECTORY ${CMAKE_BINARY_DIR} )
						
# add_custom_command( TARGET ${TARGET_NAME}
#					POST_BUILD 
#					COMMAND ${TARGET_NAME}
#					WORKING_DIRECTORY ${PROJECT_BINARY_DIR} )

  # parse each file and find test suite name in it
  foreach( FILE_NAME ${FILE_NAMES} )
    # search for test suite name
    file( STRINGS ${FILE_NAME} TEST_SUITE_STRINGS
		  REGEX "^BOOST_.+_TEST_SUITE\\( *([^ ,\\)]+).*$" )	
		
    foreach( TEST_SUITE_STRING ${TEST_SUITE_STRINGS} )	
      # parse test suite name
      string( REGEX REPLACE
		    "^BOOST_.+_TEST_SUITE\\( *([^ ,\\)]+).*$" "\\1"
		    TEST_SUITE_NAME
		    ${TEST_SUITE_STRING} )
	  if ( NOT TEST_SUITE_NAME )
	    message( WARNING "Cannot extract test suit name from " ${TEST_SUITE_NAME} )
		return()
	  endif ( NOT TEST_SUITE_NAME )
		
      message( STATUS "${TEST_SUITE_NAME}" )

      # find all test cases definitions
      file( STRINGS ${FILE_NAME} TEST_CASES
		      REGEX "^BOOST_.*_TEST_CASE\\( *([^ ,\\)]+).*$" )
		
      # create test for each test case
      foreach( TEST_CASE ${TEST_CASES} ) 
	    # find test case name
	    string( REGEX REPLACE
		        "^BOOST_.*_TEST_CASE\\( *([^ ,\\)]+).*$" "\\1"
		        TEST_CASE
		        ${TEST_CASE} )
	    set( TEST_CASE_NAME "${TEST_SUITE_NAME}_${TEST_CASE}" )
	
	    if ( NOT TEST_CASE_NAME )
	      message( WARNING "Cannot extract test case name from its declaration in " ${FILE_NAME} )
	      return()
	    endif( NOT TEST_CASE_NAME )
	
	    message( STATUS "	${TEST_CASE}" )
		# we use cmake test execution to specify working path. Without it it won't find dlls
		get_target_property( TEST_PATH ${TARGET_NAME} LOCATION_${CMAKE_BUILD_TYPE} )
	    add_test( ${TEST_CASE_NAME} ${CMAKE_COMMAND} -E chdir ${PROJECT_BINARY_DIR} ${TEST_PATH} --log_level=test_suite --run_test=${TEST_SUITE_NAME}/${TEST_CASE} )
	    set_tests_properties( "${TEST_CASE_NAME}" PROPERTIES TIMEOUT ${TIMEOUT} )
		
      endforeach( TEST_CASE ${TEST_CASES} )
    endforeach( TEST_SUITE_STRING ${TEST_SUITE_STRINGS} )
  endforeach( FILE_NAME ${FILE_NAMES} )
endfunction( add_boost_test_suite )

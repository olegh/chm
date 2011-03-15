add_boost_test_suite( owfs_reader_test_suite 1
                      owfs_reader_tests.cpp
                      tests.cpp
                       )
 target_link_libraries( owfs_reader_test_suite
                       ${Boost_LIBRARIES} )
                       
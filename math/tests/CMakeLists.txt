add_boost_test_suite( fft_test_suite 1
                      fft_tests.cpp
                      tests.cpp
                       )
 target_link_libraries( fft_test_suite
                       ${Boost_LIBRARIES} )
                       
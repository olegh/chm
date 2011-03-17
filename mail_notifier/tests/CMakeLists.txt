add_boost_test_suite( mail_notifier_test_suite 20
                      mail_notifier_tests.cpp
                      tests.cpp
                       )
 target_link_libraries( mail_notifier_test_suite
                        mail_notifier
                        ${Boost_LIBRARIES} )
                       
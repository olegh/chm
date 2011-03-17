
add_library( mail_notifier
             mail_notifier.h
             mail_notifier.cpp )
          
 target_link_libraries( mail_notifier
                       ${Boost_LIBRARIES} )
                       

# add tests to testsuite
add_subdirectory( tests )
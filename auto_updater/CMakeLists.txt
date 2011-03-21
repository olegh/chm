add_executable( auto_updater
                main.cpp )
                
target_link_libraries( auto_updater
                       ${Boost_LIBRARIES}
                       ${CMAKE_THREAD_LIBS_INIT} )
                       
 install( TARGETS auto_updater
          RUNTIME DESTINATION /opt/chm/bin )
          
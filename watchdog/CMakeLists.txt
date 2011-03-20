add_executable( watchdog
                main.cpp )
                
target_link_libraries( watchdog
                       ${Boost_LIBRARIES}
                       ${CMAKE_THREAD_LIBS_INIT} )
                       
 install( TARGETS watchdog
          RUNTIME DESTINATION /opt/chm/bin )
          
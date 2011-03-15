add_executable( chm
                main.cpp )
                
target_link_libraries( chm
                       ${Boost_LIBRARIES}
                       ${CMAKE_THREAD_LIBS_INIT} )
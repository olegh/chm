
###################################################################
# boost setup
set( Boost_USE_STATIC_LIBS   ON)
set( Boost_USE_MULTITHREADED ON)
set(Boost_ADDITIONAL_VERSIONS "1.43" "1.44" "1.45" "1.46" "1.47" "1.48" "1.49" "1.50" "1.51" "1.52" )
find_package( Boost COMPONENTS system 
                               thread 
                               program_options 
                               unit_test_framework 
                               filesystem 
                               date_time
                               REQUIRED )
if( NOT Boost_FOUND )
  message("-----------------------------")
  message( FATAL_ERROR " Cannot find boost!" )
endif( NOT Boost_FOUND )
include_directories( ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )
if( WIN32 )
  add_definitions( -D_WIN32_WINNT=0x0501 )
  # disaple IO completion port in boost asio
  add_definitions( -DBOOST_ASIO_DISABLE_IOCP )
  # enable possibility of io cancel (without this warning will be received)
  add_definitions( -DBOOST_ASIO_ENABLE_CANCELIO )
endif( WIN32 )
###################################################################

###################################################################
# pthread setup for linux
if( UNIX )
 find_package (Threads)
 if( NOT Threads_FOUND )
   message("-----------------------------")
   message( FATAL_ERROR " Cannot find pthread!" )
 endif()
endif()
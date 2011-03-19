cmake_minimum_required(VERSION 2.6)
project( CountryHouseMonitor )

SET (CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose build type Debug or Release")

include( OsSettings.cmake NO_POLICY_SCOPE )
include( TestSettings.cmake NO_POLICY_SCOPE )
include( ThirdParties.cmake NO_POLICY_SCOPE ) 

include_directories( ${CMAKE_SOURCE_DIR} )

add_subdirectory( third_parties )
add_subdirectory( configuration_files )
add_subdirectory( plan_reader )
add_subdirectory( page_builder )
add_subdirectory( owfs_reader )
add_subdirectory( math )
add_subdirectory( power_supply_monitor )
add_subdirectory( mail_notifier )
add_subdirectory( main )


#include <boost/test/unit_test.hpp>
#include <boost/bind.hpp>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <cstdio>
#include <string>
#include <fstream>
#include <boost/function.hpp>
#include <boost/filesystem.hpp>
#include <boost/exception/all.hpp>
#include "power_supply_monitor/power_supply_monitor.h"

BOOST_AUTO_TEST_SUITE( power_supply_monitor_testsuite )

using namespace chm;
using namespace chm::detail;

/**
 * @Comment: checks return correct power state when power is on
 */
BOOST_AUTO_TEST_CASE( check_is_power_on )
{
  const std::string fileContent = "state:                   on-line\n";
  std::stringstream fileStream;
  fileStream << fileContent;

  power_supply_monitor monitor( fileStream );
  BOOST_CHECK( monitor.isPowerOn() );
}

/**
 * @Comment: checks return correct power state when power is off
 */
BOOST_AUTO_TEST_CASE( check_is_power_off )
{
  const std::string fileContent = "state:                   off-line\n";
  std::stringstream fileStream;
  fileStream << fileContent;

  power_supply_monitor monitor( fileStream );
  BOOST_CHECK( !monitor.isPowerOn() );

  std::basic_istream<char>& stream_ref = fileStream;
}

/**
 * @Comment: check if file content empty
 */
BOOST_AUTO_TEST_CASE( check_is_file_content_empty )
{
  const std::string fileContent = "\n";
  std::stringstream fileStream;
  fileStream << fileContent;

  power_supply_monitor monitor( fileStream );
  BOOST_CHECK( !monitor.isPowerOn() );
}

void remove_wrap( const char* fileName )
{
  std::remove( fileName );
}
/**
 * @Comment: read from file is powered on test
 */
BOOST_AUTO_TEST_CASE( read_from_file_is_powered_on_test )
{
  boost::shared_ptr<void> fileHolder( (void*)0, boost::bind(remove_wrap, "state"));
  {
	std::ofstream out;
	out.open( "state" );
	out << "state:                   on-line\n";
  }

  std::ifstream fileStream(( "state" ));
  BOOST_CHECK( fileStream.good() );
  power_supply_monitor monitor( fileStream );
  BOOST_CHECK( monitor.isPowerOn() );
}

/**
 *  @Comment: test for find ac adapter file at current platform
 */
BOOST_AUTO_TEST_CASE( find_ac_adapter_state_on_current_platform )
{
  BOOST_CHECK_NO_THROW( power_supply_monitor() );
  BOOST_CHECK_NO_THROW( power_supply_monitor().isPowerOn() );
}

BOOST_AUTO_TEST_SUITE_END()

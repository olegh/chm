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
#include <boost/date_time.hpp>
#include "power_supply_monitor/power_supply_monitor.h"
#include "power_supply_monitor/power_state_alarmer.h"

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
  BOOST_CHECK( monitor.is_power_on() );
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
  BOOST_CHECK( !monitor.is_power_on() );

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
  BOOST_CHECK( !monitor.is_power_on() );
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
  BOOST_CHECK( monitor.is_power_on() );
}

/**
 *  @Comment: test for find ac adapter file at current platform
 */
BOOST_AUTO_TEST_CASE( find_ac_adapter_state_on_current_platform )
{
  BOOST_CHECK_NO_THROW( power_supply_monitor() );
  BOOST_CHECK_NO_THROW( power_supply_monitor().is_power_on() );
}
//================================================================

struct fake_power_state
{
	bool power_state;
	bool get()
	{
		return power_state;
	}
};

struct fake_notificator
{
	std::string notification;
	void set( const std::string& str )
	{
		notification = str;
	}
};

boost::posix_time::ptime
fake_time_now()
{
	return boost::posix_time::ptime();
}
/**
 *  @Comment: test for power state alarmer
 */
BOOST_AUTO_TEST_CASE( power_alarmer_test )
{
	fake_power_state power_state;
	power_state.power_state = true;
	fake_notificator notificator;
	notificator.notification = "";

	power_state_alarmer alarmer( boost::bind(&fake_notificator::set, &notificator, _1),
			                         boost::bind(&fake_power_state::get, &power_state),
			                         fake_time_now );

	alarmer.notify_if_power_state_changed();
	BOOST_CHECK( notificator.notification.empty() );

	power_state.power_state = false;
	alarmer.notify_if_power_state_changed();

	BOOST_CHECK_EQUAL( std::string("Power state is: Off, checked at: not-a-date-time"), notificator.notification );

	notificator.notification = "";
	power_state.power_state = true;
	alarmer.notify_if_power_state_changed();

	BOOST_CHECK_EQUAL( std::string("Power state is: On, checked at: not-a-date-time"), notificator.notification );

	notificator.notification = "";
	alarmer.notify_if_power_state_changed();
	BOOST_CHECK( notificator.notification.empty() );
}
BOOST_AUTO_TEST_SUITE_END()

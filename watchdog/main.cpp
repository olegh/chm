#include <stdlib.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/exception/all.hpp>
#include <boost/date_time.hpp>
#include <boost/bind.hpp>
#include <boost/cast.hpp>
#include "version/version.hpp"

namespace po = boost::program_options;

void reboot()
{
  std::cout << "reboot\n";
  ::system( "reboot");
}
//============================================================================
bool check_condition_impl( bool isFirstCheck, const std::string& check_target, const std::string& host, unsigned ping_requests_number )
{
  const bool isTargetExists = boost::filesystem::exists(check_target);
  bool isHostReacheble = true;
  const std::string pint_requests = boost::lexical_cast<std::string>(ping_requests_number);

  if( !host.empty())
  {
    isHostReacheble = (0 == ::system( ("ping -v -c " + pint_requests + " -W 10 " + host).c_str() ));
  }

  if( !isTargetExists || !isHostReacheble )
  {
    if( isFirstCheck )
    {
      std::cerr << "first check of condition fails";
      return false;
    }
    else
      reboot();
  }

  return true;
}
//=====================================================================
void check_condition( bool isFirstCheck,
                      unsigned period,
                      const std::string& check_target,
                      const std::string& host,
                      unsigned ping_requests_number,
                      boost::asio::deadline_timer& timer,
                      const boost::system::error_code& )
{
  try
  {
    if( !check_condition_impl( isFirstCheck, check_target, host, ping_requests_number ))
      return;
  }
  catch( ... )
  {
    std::cerr << boost::current_exception_diagnostic_information();
  }

  timer.expires_from_now( boost::posix_time::seconds(period));
  timer.async_wait( boost::bind( check_condition,
                                 false,
                                 period,
                                 boost::ref(check_target),
                                 boost::ref(host),
                                 ping_requests_number,
                                 boost::ref(timer),
                                 _1));
}
//=====================================================================
int main( int argc, char* argv[] )
{

  try
  {
    std::string check_target, host;
    boost::asio::io_service io;
    boost::asio::deadline_timer check_target_timer((io));
    unsigned ping_request_number = 0, check_condition_period = 0;

    po::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "produce help message")
      ("daemon,d", "run as daemon" )
      ("check_target,c", po::value<std::string>(&check_target), "if target file or directory not found then system reboot")
      ("version,v", "get version number")
      ("remote_host,r", po::value<std::string>(&host)->default_value("google.com"), "if value is set, then also check connection to internet by pinging host")
      ("ping_request_number,p", po::value<unsigned>(&ping_request_number)->default_value(3), "number of ping request to check internet connection")
      ("seconds_delay_beween_checks,s", po::value<unsigned>(&check_condition_period)->default_value(300), "number of seconds in perdiod between checking internet connection")
    ;

    po::variables_map vars;
    po::store(po::parse_command_line(argc, argv, desc), vars );
    po::notify( vars );

    if( vars.count("help"))
    {
      std::cout << desc;
      return 0;
    }

    if( vars.count("version"))
    {
      std::cout << version::to_str() << "\n";
      return 0;
    }

    if( 0 == vars.count("check_target"))
    {
      std::cerr << "please specify the check target, try --help for more details\n";
      return -1;
    }

    if( vars.count("daemon"))
    {
      check_condition( true,
                       check_condition_period,
                       check_target,
                       host,
                       ping_request_number,
                       check_target_timer,
                       boost::system::error_code() );
      io.run();
    }
    else
      check_condition_impl(true, check_target, host, ping_request_number);

  }
  catch( ... )
  {
    std::cerr << boost::current_exception_diagnostic_information();
    return -1;
  }

  return 0;
}
//===============================================================

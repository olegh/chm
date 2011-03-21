#include <stdlib.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/exception/all.hpp>
#include <boost/date_time.hpp>
#include <boost/bind.hpp>

namespace po = boost::program_options;

void reboot()
{
  std::cout << "reboot\n";
  ::system( "reboot");
}
//============================================================================
bool check_condition_impl( bool isFirstCheck, const std::string& check_target )
{
  if( !boost::filesystem::exists(check_target) )
  {
    if( isFirstCheck )
    {
      std::cerr << "err file: " + check_target + " not found at first check\n";
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
                      boost::asio::deadline_timer& timer,
                      const boost::system::error_code& )
{
  try
  {
    if( !check_condition_impl( isFirstCheck, check_target ))
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
                                 boost::ref(timer),
                                 _1));
}
//=====================================================================
int main( int argc, char* argv[] )
{

  try
  {
    std::string check_target;
    boost::asio::io_service io;
    boost::asio::deadline_timer check_target_timer((io));

    po::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "produce help message")
      ("daemon,d", "run as daemon" )
      ("check_target,c", po::value<std::string>(&check_target), "if target file or directory not found then system reboot")
    ;

    po::variables_map vars;
    po::store(po::parse_command_line(argc, argv, desc), vars );
    po::notify( vars );

    if( vars.count("help"))
    {
      std::cout << desc;
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
                       10,
                       check_target,
                       check_target_timer,
                       boost::system::error_code() );
      io.run();
    }
    else
      check_condition_impl(true, check_target);

  }
  catch( ... )
  {
    std::cerr << boost::current_exception_diagnostic_information();
    return -1;
  }

  return 0;
}
//===============================================================

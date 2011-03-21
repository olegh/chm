#include <stdlib.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/exception/all.hpp>
#include <boost/date_time.hpp>
#include <boost/bind.hpp>

namespace po = boost::program_options;
int main( int argc, char* argv[] )
{
  try
  {
    const std::string git_url = "git://github.com/olegh/chm.git";
    const std::string branch = "stable";
    boost::filesystem::path local_tree_path = "/tmp/chm";

    po::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "produce help message")
      ("daemon,d", "run as daemon" )

    ;

    po::variables_map vars;
    po::store(po::parse_command_line(argc, argv, desc), vars );
    po::notify( vars );

    if( vars.count("help"))
    {
      std::cout << desc;
      return 0;
    }

    if( !boost::filesystem::exists( local_tree_path ))
    {
      const std::string cmd ="git clone " + git_url + " -b " + branch + " " + local_tree_path.file_string();
      if( 0 != ::system( cmd.c_str()))
      {
        BOOST_THROW_EXCEPTION( boost::enable_error_info(std::runtime_error("command fail: " + cmd)));
      }
    }
    else
    {
      const std::string cmd = "cd " + local_tree_path.file_string() + "; git pull; rm -rf ./bin; mkdir bin; cd ./bin; cmake ../";
      std::cout << cmd + "\n";
      if( 0 != ::system( cmd.c_str() ))
      {
        BOOST_THROW_EXCEPTION( boost::enable_error_info(std::runtime_error("command fail: " + cmd)));
      }

    }
  }
  catch( ... )
    {
      std::cerr << boost::current_exception_diagnostic_information();
      return -1;
    }

  return 0;
}

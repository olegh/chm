#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/exception/all.hpp>
#include <boost/date_time.hpp>
#include <boost/bind.hpp>
#include <version/version.hpp>
#include <boost/thread.hpp>

void try_update( const boost::filesystem::path& local_tree_path )
{
  const std::string git_url = "git://github.com/olegh/chm.git";
  const std::string branch = "stable";

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
    if( 0 != ::system( cmd.c_str() ))
    {
      BOOST_THROW_EXCEPTION( boost::enable_error_info(std::runtime_error("command fail: " + cmd)));
    }
  }


  std::ifstream version_number_file((( local_tree_path / "version" / "version").file_string().c_str()));

  if( !version_number_file.good() )
  {
    BOOST_THROW_EXCEPTION( std::runtime_error("Can't open file: " + ( local_tree_path / "version" / "version").file_string()));
  }

  unsigned version_number = 0;
  version_number_file >> version_number;

  if( version_number > version::get() )
  {
    boost::filesystem::path bin_path = local_tree_path / "bin";
    std::cout << "read version: " << version_number << " current version" << version::get() << "\n";
    if( 0 != ::system( ("cd " + bin_path.file_string() + "; make; make test").c_str() ))
    {
      BOOST_THROW_EXCEPTION( boost::enable_error_info(std::runtime_error("run tests fails")));
    }

    if( 0 != ::system( ("cd " + bin_path.file_string() + "; make install").c_str() ))
    {
      BOOST_THROW_EXCEPTION( boost::enable_error_info(std::runtime_error("install fails")));
    }

    if( 0 != ::system( "reboot"))
    {
      BOOST_THROW_EXCEPTION( boost::enable_error_info(std::runtime_error("reboot fails")));
    }
  }
}

namespace po = boost::program_options;
int main( int argc, char* argv[] )
{
  try
  {
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

    if( vars.count("daemon"))
    {
      for( ;; )
      {
        try
        {
          try_update(local_tree_path);
        }
        catch( ... )
        {
          std::cerr << boost::current_exception_diagnostic_information();
        }

         boost::this_thread::sleep( boost::posix_time::hours(5));
      }
    }
    else
      try_update(local_tree_path);

  }
  catch( ... )
    {
      std::cerr << boost::current_exception_diagnostic_information();
      return -1;
    }

  return 0;
}

#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <boost/exception/all.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/program_options.hpp>
#include <boost/date_time.hpp>
#include <boost/bind.hpp>
#include <sstream>
#include <boost/asio.hpp>
#include "page_builder/page.hpp"
#include "page_builder/js_text.hpp"
#include "page_builder/js_polygon.hpp"
#include "plan_reader/plan_reader.hpp"
#include "owfs_reader/owfs_reader.hpp"
#include "common/named_properties.hpp"
#include "common/ptree_helpers.hpp"

using namespace chm;
using boost::property_tree::iptree;
namespace po = boost::program_options;
using namespace boost::asio;
using namespace boost;

//============================================================
void add_builders( page& html_page, 
                  const iptree::value_type& pair )
{
  const iptree& cur_tree = pair.second; 
  html_page << js_polygon(cur_tree) 
    << js_text(cur_tree);
}
//============================================================
void generatePage( const std::string& owfs_path,
                   const std::string& target_file,
                   const std::string& plan_file )
{
  std::ifstream in_file(( plan_file.c_str() ));
  plan_reader reader( in_file, plan_reader::make_info_reader(in_file) );
  iptree tree = reader.read();

  owfs_reader ofws((owfs_path));
  ofws.update_tree( tree );

  page html_page;
  std::for_each( make_begin_filter_const(props::polygon::relative(), tree),
                 make_end_filter_const(tree),
                 bind( add_builders, ref(html_page), _1 ));

  std::ofstream out_file((target_file.c_str()));
  out_file << html_page();
}
//============================================================
void run_as_deamon_impl( const std::string& owfs_path,
                         const std::string& target_file,
                         const std::string& plan_file,
                         unsigned update_period_sec,
                         boost::asio::deadline_timer& timer,
                         const boost::system::error_code& error )
{
  using namespace boost;
  try
  {
    generatePage( owfs_path, target_file, plan_file );
  }
  catch( ... )
  {
    std::cerr << boost::current_exception_diagnostic_information();
  }

  timer.expires_from_now( posix_time::seconds(update_period_sec) );
  timer.async_wait( bind(run_as_deamon_impl,
                         ref(owfs_path),
                         ref(target_file),
                         ref(plan_file),
                         update_period_sec,
                         ref(timer),
                         _1));
}
//============================================================
void run_as_deamon( const std::string& owfs_path,
                    const std::string& target_file,
                    const std::string& plan_file,
                    unsigned update_period_sec )
{
  using namespace boost;
  io_service io_service;
  deadline_timer timer(( io_service ));
  
  run_as_deamon_impl( owfs_path, 
                      target_file, 
                      plan_file, 
                      update_period_sec, 
                      timer, 
                      boost::system::error_code() );

  io_service.run();
}
//============================================================
int main( int argc, char* argv[] )
{
  try
  {
    std::string owfs_path, target_file, plan_file;
    unsigned update_period_sec = 0;

    po::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "produce help message")
      ("deamon,d", "run as deamon" )
      ("owfs_path,o", po::value<std::string>(&owfs_path)->default_value("/media/1-wire"), "path to owfs mount point")
      ("target_file,t", po::value<std::string>(&target_file)->default_value("index.htm"), "generated page file")
      ("plan_file,p", po::value<std::string>(&plan_file)->default_value("plan.info"), "file with plan")
      ("update_period,u", po::value<unsigned>(&update_period_sec)->default_value(10), "update thermometers period in seconds, take place only with -d option")
      ;

    po::variables_map vars;
    po::store(po::parse_command_line(argc, argv, desc), vars );
    po::notify( vars );

    if( vars.count("help"))
    {
      std::cout << desc;
      return 0;
    }
   
    if( vars.count("deamon"))
    {
      run_as_deamon( owfs_path, target_file, plan_file, update_period_sec );
    }
    else
      generatePage( owfs_path, target_file, plan_file );
    
  }
  catch( ... )
  {
    std::cerr << boost::current_exception_diagnostic_information();
    return -1;
  }

  return 0;
}

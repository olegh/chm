#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <boost/exception/all.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/program_options.hpp>
#include <boost/date_time.hpp>
#include <boost/bind.hpp>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "page_builder/page.hpp"
#include "page_builder/js_text.hpp"
#include "page_builder/js_polygon.hpp"
#include "plan_reader/plan_reader.hpp"
#include "owfs_reader/owfs_reader.hpp"
#include "common/named_properties.hpp"
#include "common/ptree_helpers.hpp"
#include "power_supply_monitor/power_state_alarmer.h"
#include "power_supply_monitor/power_supply_monitor.h"
#include "power_supply_monitor/power_state_polygon_updater.hpp"
#include "mail_notifier/mail_notifier.h"

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
void generate_page( const std::string& owfs_path,
                    const std::string& target_file,
                    const std::string& plan_file,
                    const std::string& ac_adapter_path )
{
  std::ifstream in_file(( plan_file.c_str() ));
  plan_reader reader( in_file, plan_reader::make_info_reader(in_file) );
  iptree tree = reader.read();

  owfs_reader ofws((owfs_path));
  ofws.update_tree( tree );

  power_state_polygon_updater power_updater((ac_adapter_path));
  power_updater.update_tree( tree );

  page html_page;
  std::for_each( make_begin_filter_const(props::polygon::relative(), tree),
                 make_end_filter_const(tree),
                 bind( add_builders, ref(html_page), _1 ));

  std::ofstream out_file((target_file.c_str()));
  out_file << html_page();
}
//============================================================
void page_generator( const std::string& owfs_path,
                          const std::string& target_file,
                          const std::string& plan_file,
                          const std::string& ac_adapter_path,
                          unsigned update_period_sec,
                          boost::asio::deadline_timer& timer,
                          const boost::system::error_code& error )
{
  using namespace boost;

  if( error )
  {
    std::cerr << error.message();
  }

  try
  {
    generate_page( owfs_path, target_file, plan_file, ac_adapter_path );
  }
  catch( ... )
  {
    std::cerr << boost::current_exception_diagnostic_information();
  }

  timer.expires_from_now( posix_time::seconds(update_period_sec) );
  timer.async_wait( bind(page_generator,
                         ref(owfs_path),
                         ref(target_file),
                         ref(plan_file),
                         ref(ac_adapter_path),
                         update_period_sec,
                         ref(timer),
                         _1));
}
//============================================================
void power_state_checker( power_state_alarmer& alarmer,
		                      boost::asio::deadline_timer& timer,
		                      unsigned update_period_sec,
		                      const boost::system::error_code& error)
{
	using namespace boost;

	if( error )
	{
		std::cerr << error.message();
	}

	try
	{
		alarmer.notify_if_power_state_changed();
	}
	catch( ... )
	{
		std::cerr << boost::current_exception_diagnostic_information();
	}
	timer.expires_from_now( posix_time::seconds(update_period_sec) );
	timer.async_wait( bind( power_state_checker,
			                    ref(alarmer),
			                    ref(timer),
			                    update_period_sec,
			                    _1 ));
}
//============================================================
void fake_notifier( const std::string& cmd )
{
	std::cout << "fake_notifier:" << cmd;
}
//============================================================
void run_as_deamon( const std::string& owfs_path,
                    const std::string& target_file,
                    const std::string& plan_file,
                    unsigned update_period_sec,
                    unsigned thread_number,
                    const std::string& ac_adapter_path,
                    const std::string& mail_notify_to )
{
  using namespace boost;
  io_service io_service;
  deadline_timer timer_page_generator(( io_service ));
  deadline_timer timer_power_checker(( io_service ));

  mail_notifier notifier;
  power_supply_monitor power_state_monitor(( ac_adapter_path ));
  boost::function<void(const std::string&)> notificator;
  const std::string subject = "power state";
  if( mail_notify_to.empty() )
  {
  	notificator = fake_notifier;
  }
  else
  {
  	notificator = boost::bind( &mail_notifier::notify, &notifier, ref(mail_notify_to), ref(subject), _1 );
  }
  power_state_alarmer alarmer( notificator,
  		                         boost::bind(&power_supply_monitor::is_power_on, &power_state_monitor));
  
  page_generator( owfs_path, 
                      target_file, 
                      plan_file,
                      ac_adapter_path,
                      update_period_sec, 
                      timer_page_generator,
                      boost::system::error_code() );

  power_state_checker( alarmer,
  		                 timer_power_checker,
  		                 update_period_sec,
  		                 boost::system::error_code() );

  boost::thread_group thread_group;
  for( ; thread_number > 0; --thread_number )
  {
  	thread_group.create_thread( boost::bind(&io_service::run, &io_service));
  }

  thread_group.join_all();
}
//============================================================
int main( int argc, char* argv[] )
{
  try
  {
    std::string owfs_path, target_file, plan_file, ac_adapter_path, mail_notify_to;
    unsigned update_period_sec = 0, thread_number = 0;

    po::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "produce help message")
      ("deamon,d", "run as deamon" )
      ("owfs_path,o", po::value<std::string>(&owfs_path)->default_value("/media/1-wire"), "path to owfs mount point")
      ("target_file,t", po::value<std::string>(&target_file)->default_value("/var/www/index.html"), "generated page file")
      ("plan_file,p", po::value<std::string>(&plan_file)->default_value("/opt/chm/conf/plan.info"), "file with plan")
      ("update_period,u", po::value<unsigned>(&update_period_sec)->default_value(10), "update thermometers period in seconds, take place only with -d option")
      ("ac_adapter_path,a", po::value<std::string>(&ac_adapter_path)->default_value("/proc/acpi/ac_adapter"), "path to the ac_adapter dir, wich contains indormation about battery and AC")
      ("thread_number,t",  po::value<unsigned>(&thread_number)->default_value(2), "number of threads")
      ("mail_notify_to,m", po::value<std::string>(&mail_notify_to), "mail which will be notified about events")
    ;

    po::variables_map vars;
    po::store(po::parse_command_line(argc, argv, desc), vars );
    po::notify( vars );

    if( vars.count("help"))
    {
      std::cout << desc;
      return 0;
    }
   
    boost::filesystem::path target_js_lib =
        boost::filesystem::path(boost::filesystem::path(target_file).remove_filename().directory_string())
                                     / "jsDraw2D.js";
    std::remove( target_js_lib.file_string().c_str() );

    boost::filesystem::copy_file( "/opt/chm/js/jsDraw2D.js",
                                  target_js_lib );
    if( vars.count("deamon"))
    {
      run_as_deamon( owfs_path,
      		           target_file,
      		           plan_file,
      		           update_period_sec,
      		           thread_number,
      		           ac_adapter_path,
      		           mail_notify_to );
    }
    else
      generate_page( owfs_path, target_file, plan_file, ac_adapter_path );
    
  }
  catch( ... )
  {
    std::cerr << boost::current_exception_diagnostic_information();
    return -1;
  }

  return 0;
}

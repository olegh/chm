#include "mail_notifier/mail_notifier.h"

#include <string>
#include <boost/function.hpp>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdexcept>
#include <fstream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/exception/all.hpp>


namespace
{

struct linux_console
{
  static int system( const std::string& cmd )
  {
  	return ::system( cmd.c_str() );
  }
};

struct linux_tmp_file_name_creater
{
	static std::string create()
	{
		std::string file_name;
		file_name.resize( TMP_MAX );
		if( 0 == ::tmpnam(&file_name[0]))
		{
			BOOST_THROW_EXCEPTION( boost::enable_error_info(std::runtime_error("Can't create temp file name"))
			                       << boost::errinfo_api_function("::mkstemp")
			                       << boost::errinfo_errno(errno));
		}
		return file_name.c_str();
	}
};

} //namespace

namespace chm
{
mail_notifier::
mail_notifier( Console console, TmpFileNameCreator tmpFileNameCreater )
 : console_(console),
   tmp_file_name_creater_(tmpFileNameCreater)
{}
//======================================================================
void mail_notifier::file_deleter( const std::string& file_name )
{
	std::remove( file_name.c_str() );
}
//=======================================================================
mail_notifier::Console
mail_notifier::create_default_console()
{
	return boost::bind( linux_console::system, _1 );
}
//=======================================================================
mail_notifier::TmpFileNameCreator
mail_notifier::create_default_tmp_file_name_creater()
{
	return boost::bind(&linux_tmp_file_name_creater::create);
}
//=======================================================================
void mail_notifier::
notify( const std::string& email,
        const std::string& subject,
        const std::string& message )
{
  if( email.empty() )
  {
    BOOST_THROW_EXCEPTION( std::runtime_error("email argument can't be empty"));
  }
  const std::string tmp_file_name = tmp_file_name_creater_();
  std::ofstream tmp_file((tmp_file_name.c_str()));

  if( !tmp_file.good() )
  {
  	BOOST_THROW_EXCEPTION( std::runtime_error("can't create tmp file for mail"));
  }

  boost::shared_ptr<void>
    auto_tmp_file_deleter( (void*)0,
    		                    boost::bind( &mail_notifier::file_deleter,
    		                    		         tmp_file_name));
  tmp_file << message;
  tmp_file.close();

  const std::string cmd = "mailx -s \"" + subject + "\" " + email + " <" + tmp_file_name;
  int err = console_( cmd );
  if( 0 != err )
  {
  	BOOST_THROW_EXCEPTION( boost::enable_error_info(std::runtime_error("Can't send mail"))
  				                       << boost::errinfo_api_function(cmd.c_str())
  				                       << boost::errinfo_errno(errno));
  }
}
//=======================================================================
} //namespace chm

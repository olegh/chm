#include <boost/test/unit_test.hpp>
#include "mail_notifier/mail_notifier.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/cstdint.hpp>
#include <string>
#include <boost/exception/all.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <unistd.h>
#include <errno.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

BOOST_AUTO_TEST_SUITE( mail_notifier_tests )

using namespace chm;

struct fake_console
{
  int put( const std::string& cmd )
  {
    cmd_ += cmd;
    return 0;
  }

  std::string str() const
  {
    return cmd_;
  }

private:
  std::string cmd_;
};

static std::string fake_file()
{
	return "/tmp/some_tmp_file";
}

/**
 * @Comment: test notification by mail with fake console
 */
BOOST_AUTO_TEST_CASE( test_mail_notification )
{
  fake_console console;
  mail_notifier notifier( boost::bind( &fake_console::put, &console, _1 ), fake_file );
  notifier.notify( "some.mail@some.com", "power off", "power is off at !xxx!");

  const std::string expected =
  		"mailx -s \"power off\" some.mail@some.com </tmp/some_tmp_file";

  BOOST_CHECK_EQUAL( console.str(), expected );
}
//===================================================

/**
 *  @Comment: test notification several receivers
 */
BOOST_AUTO_TEST_CASE( test_notification_by_list )
{
  fake_console console;
  std::vector< std::string > emails;
  emails.push_back("some.mail@some.com");
  emails.push_back("some1.mail@some.com");

  mail_notifier notifier( boost::bind( &fake_console::put, &console, _1 ), fake_file );
  notifier.notify_by_list( emails, "power off", "power is off at !xxx!");

  const std::string expected =
         "mailx -s \"power off\" some.mail@some.com </tmp/some_tmp_filemailx -s \"power off\" some1.mail@some.com </tmp/some_tmp_file";

   BOOST_CHECK_EQUAL( console.str(), expected );
}
//===================================================




BOOST_AUTO_TEST_SUITE_END()

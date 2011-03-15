#include <boost/test/unit_test.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/bind.hpp>
#include "plan_reader/plan_reader.hpp"
#include <boost/cstdint.hpp>
#include <sstream>

#include <sstream>

using boost::uint64_t;

BOOST_AUTO_TEST_SUITE( plan_reader_tests )

/**
  *@Comment: test for read polygon from xml format
  */
BOOST_AUTO_TEST_CASE( read_plygon_whith_read_from_xml )
{
  const std::string xmldoc =
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    "<polygon>"
      "<thermometer_id>34287364287</thermometer_id>"
      "<name>bathroom</name>"
      "<point>"
        "<x>1</x>"
        "<y>2</y>"
      "</point>"
      "<point>"
        "<x>3</x>"
        "<y>4</y>"
      "</point>"
    "</polygon>";

  std::stringstream ss((xmldoc));

  using boost::property_tree::iptree;
  chm::plan_reader reader( ss, chm::plan_reader::make_xml_reader(ss));
  iptree tree;

  BOOST_CHECK_NO_THROW( tree = reader.read() );

  const iptree& child = tree.get_child("polygon");
  iptree::const_assoc_iterator it = child.find( "point" );
  BOOST_CHECK( it != child.not_found() );
  BOOST_CHECK_EQUAL( 1, (*it).second.get<int>("x"));
  BOOST_CHECK_EQUAL( 2, (*it).second.get<int>("y"));
  ++it;
  BOOST_CHECK( it != child.not_found() );
  BOOST_CHECK_EQUAL( 3, (*it).second.get<int>("x"));
  BOOST_CHECK_EQUAL( 4, (*it).second.get<int>("y"));
  BOOST_CHECK_EQUAL( (uint64_t)34287364287, tree.get<uint64_t>("polygon.thermometer_id"));
  BOOST_CHECK_EQUAL( std::string("bathroom"), tree.get<std::string>("polygon.name"));
}
//=======================================================


/**
  * @Comment: read polygon from info format test
  */
BOOST_AUTO_TEST_CASE( read_plygon_whith_read_from_info )
{
  const std::string infodoc =
    "polygon\n"
    "{\n"
       "thermometer_id 34287364287\n"
       "name \"bathroom\""
       "point\n"
       "{\n"
         "x 1\n"
         "y 2\n"
       "}\n"
       "point\n"
       "{\n"
         "x 3\n"
         "y 4\n"
       "}\n"
    "}";

  std::stringstream ss((infodoc));

  using boost::property_tree::iptree;
  chm::plan_reader reader( ss, chm::plan_reader::make_info_reader(ss));
  iptree tree;
  BOOST_CHECK_NO_THROW( tree = reader.read() );

  const iptree& child = tree.get_child("polygon");
  iptree::const_assoc_iterator it = child.find( "point" );
  BOOST_CHECK( it != child.not_found() );
  BOOST_CHECK_EQUAL( 1, (*it).second.get<int>("x"));
  BOOST_CHECK_EQUAL( 2, (*it).second.get<int>("y"));
  ++it;
  BOOST_CHECK( it != child.not_found() );
  BOOST_CHECK_EQUAL( 3, (*it).second.get<int>("x"));
  BOOST_CHECK_EQUAL( 4, (*it).second.get<int>("y"));
  BOOST_CHECK_EQUAL( (uint64_t)34287364287, tree.get<uint64_t>("polygon.thermometer_id"));
  BOOST_CHECK_EQUAL( std::string("bathroom"), tree.get<std::string>("polygon.name"));
}

BOOST_AUTO_TEST_SUITE_END()

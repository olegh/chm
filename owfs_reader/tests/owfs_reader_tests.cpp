#include <boost/test/unit_test.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/cstdint.hpp>
#include "common/named_properties.hpp"
#include "owfs_reader/owfs_reader.hpp"


using namespace chm;
using boost::uint64_t;

BOOST_AUTO_TEST_SUITE( page_builder_tests )


boost::property_tree::iptree make_square_room_helper()
{
  boost::property_tree::iptree tree;
  
  tree.put( props::polygon::full(), "" );
  tree.put( props::polygon::thermometer_id::full(), "22.A5A21900000039" );
  tree.put( props::polygon::color::full(), 0xFFCC33 );

  boost::property_tree::iptree p1, p2, p3, p4, text1, text2;
  p1.put( props::polygon::point::x::relative(), 0 );
  p1.put( props::polygon::point::y::relative(), 0 );
  p2.put( props::polygon::point::x::relative(), 1 );
  p2.put( props::polygon::point::y::relative(), 0 );
  p3.put( props::polygon::point::x::relative(), 1 );
  p3.put( props::polygon::point::y::relative(), 1 );
  p4.put( props::polygon::point::x::relative(), 0 );
  p4.put( props::polygon::point::y::relative(), 1 );
  tree.add_child( props::polygon::point::full(), p1 );
  tree.add_child( props::polygon::point::full(), p2 );
  tree.add_child( props::polygon::point::full(), p3 );
  tree.add_child( props::polygon::point::full(), p4 );
  
  text1.put( props::polygon::text::color::relative(), 0xFF0000 );
  text1.put( props::polygon::text::name::relative(), "bathroom" );
  text1.put( props::polygon::text::value::relative(), "bathroom" );
  text1.put( props::polygon::text::x::relative(), 10 );
  text1.put( props::polygon::text::y::relative(), 10 );

  text2.put( props::polygon::text::color::relative(), 0x0000FF );
  text2.put( props::polygon::text::name::relative(), "comment" );
  text2.put( props::polygon::text::value::relative(), "last updated at 0:31" );
  text2.put( props::polygon::text::x::relative(), 10 );
  text2.put( props::polygon::text::y::relative(), 15 );

  tree.add_child( props::polygon::text::full(), text1 );
  tree.add_child( props::polygon::text::full(), text2 );

  return tree;
}
/**
  *@Comment: Read temperature test
  */
BOOST_AUTO_TEST_CASE( read_temperature_test )
{
  boost::property_tree::iptree tree = make_square_room_helper();
  owfs_reader reader( "c://" );
  reader.update_tree(tree);
}

BOOST_AUTO_TEST_SUITE_END()
#include <boost/test/unit_test.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/cstdint.hpp>
#include "common/named_properties.hpp"
#include "page_builder/js_polygon.hpp"
#include "page_builder/page.hpp"
#include "page_builder/js_text.hpp"
#include "page_builder/js_image.hpp"

using boost::uint64_t;
using namespace chm;

BOOST_AUTO_TEST_SUITE( page_builder_tests )

boost::property_tree::iptree make_square_room_helper()
{
  boost::property_tree::iptree tree;
  
  tree.put( props::polygon::full(), "" );
  tree.put( props::polygon::thermometer_id::full(), "A2.43287423" );
  tree.put( props::polygon::color::full(), 0xFFCC33 );

  boost::property_tree::iptree p1, p2, p3, p4, text1, text2, image;
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

  image.put( props::polygon::image::url::relative(), "thermometers.png");
  image.put( props::polygon::image::x::relative(), 40 );
  image.put( props::polygon::image::y::relative(), 50 );

  tree.add_child( props::polygon::text::full(), text1 );
  tree.add_child( props::polygon::text::full(), text2 );
  tree.add_child( props::polygon::image::full(), image );

  return tree;
}
/**
  *@Comment: test build polygon java script
  */
BOOST_AUTO_TEST_CASE( build_polygon_js_test )
{
  using boost::property_tree::iptree; 
  iptree tree = make_square_room_helper();
  const iptree& polygon = tree.get_child( props::polygon::full() );

  std::string polygon_js = js_polygon( polygon )();
  const std::string expected = 
    "gr.fillPolygon( new jsColor( \"#FFCC33\" ), new Array( new jsPoint( 0, 0 ), new jsPoint( 1, 0 ), new jsPoint( 1, 1 ), new jsPoint( 0, 1 ) ) );\n"
    "gr.drawPolygon( new jsPen( new jsColor( \"#000000\" ), 3 ), new Array( new jsPoint( 0, 0 ), new jsPoint( 1, 0 ), new jsPoint( 1, 1 ), new jsPoint( 0, 1 ) ) );\n";

  BOOST_CHECK_EQUAL( expected, polygon_js );
}

/**
  * @Comment: test build page with empty content
  */
BOOST_AUTO_TEST_CASE( build_page_with_empty_content_test )
{
  std::string html = page().build();
  const std::string expected = 
    "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
    "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
    "<head>\n"
      "<title>chm</title>\n"
      "<meta http-equiv=\"refresh\" content=\"10\">\n"
      "<script type=\"text/JavaScript\" src=\"jsDraw2D.js\"></script>\n"
    "</head>\n"
    "<body>\n"
      "<div id=\"canvas\" style=\"position:relative;width:600px;height:300px;\"></div>\n" 
      "<script type=\"text/JavaScript\">\n"
        "var gr = new jsGraphics(document.getElementById(\"canvas\"));\n"
      "</script>\n"
    "</body>\n"
    "</html>\n";

  BOOST_CHECK_EQUAL( expected, html );
}

/**
  * @Comment: test build page with simple polygon
  */
BOOST_AUTO_TEST_CASE( build_page_with_simple_polygon )
{
  using boost::property_tree::iptree; 
  iptree tree = make_square_room_helper();
  const iptree& polygon = tree.get_child( props::polygon::full() );
  
  page html_page;
  html_page << js_polygon(polygon);

  const std::string expected = 
    "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
    "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
    "<head>\n"
      "<title>chm</title>\n"
      "<meta http-equiv=\"refresh\" content=\"10\">\n"
      "<script type=\"text/JavaScript\" src=\"jsDraw2D.js\"></script>\n"
    "</head>\n"
    "<body>\n"
      "<div id=\"canvas\" style=\"position:relative;width:600px;height:300px;\"></div>\n" 
      "<script type=\"text/JavaScript\">\n"
        "var gr = new jsGraphics(document.getElementById(\"canvas\"));\n"
        "gr.fillPolygon( new jsColor( \"#FFCC33\" ), new Array( new jsPoint( 0, 0 ), new jsPoint( 1, 0 ), new jsPoint( 1, 1 ), new jsPoint( 0, 1 ) ) );\n"
        "gr.drawPolygon( new jsPen( new jsColor( \"#000000\" ), 3 ), new Array( new jsPoint( 0, 0 ), new jsPoint( 1, 0 ), new jsPoint( 1, 1 ), new jsPoint( 0, 1 ) ) );\n"
      "</script>\n"
    "</body>\n"
    "</html>\n";

  BOOST_CHECK_EQUAL( html_page.build(), expected );
}

BOOST_AUTO_TEST_CASE( build_test_js_test  )
{
  using boost::property_tree::iptree; 
  iptree tree = make_square_room_helper();
  const iptree& polygon = tree.get_child( props::polygon::full() );

  std::string expected = 
  "gr.drawText(\"bathroom\", new jsPoint( 10, 10 ), \"Courier New\", new jsColor( \"#FF0000\" ));\n"
  "gr.drawText(\"last updated at 0:31\", new jsPoint( 10, 15 ), \"Courier New\", new jsColor( \"#0000FF\" ));\n";

  BOOST_CHECK_EQUAL( expected, js_text(polygon)() );
}

BOOST_AUTO_TEST_CASE( build_js_image_test )
{
  using boost::property_tree::iptree; 
  iptree tree = make_square_room_helper();
  const iptree& polygon = tree.get_child( props::polygon::full() );

  const std::string expected = 
    "gr.drawImage(thermometers.png, new jsPoint( 40, 50 ));\n";

  BOOST_CHECK_EQUAL( js_image(polygon)(), expected );
}

BOOST_AUTO_TEST_SUITE_END()
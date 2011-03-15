#if !defined( BA669AA0_EF10_4a56_B5DA_90FF38383232 )
# define BA669AA0_EF10_4a56_B5DA_90FF38383232

#include <string>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/foreach.hpp>
#include "common/ptree_helpers.hpp"
#include "common/named_properties.hpp"
#include "page_builder/js_simple_objects.hpp"

namespace chm{


template<class Ptree>
struct basic_js_polygon
{
  typedef js_simple_objects js_so;
  typedef basic_js_polygon<Ptree> this_type;

  basic_js_polygon( const Ptree& ptree )
    : ptree_( ptree )
  {}

  std::string operator()()
  {
    const std::string colorPath = props::polygon::color::relative();
    return 
      "gr.fillPolygon( " + js_so::jsColor( ptree_.template get<boost::uint32_t>(colorPath)) + ", " 
                                          + jsArray() + " );\n"
      + "gr.drawPolygon( " + js_so::jsPen() + ", " + jsArray() + " );\n";
  }
private:

  std::string jsArray()
  {
    const std::string pointPath = props::polygon::point::relative(); 
    std::string result = "new Array( ";

    std::for_each( make_begin_filter_const(pointPath, ptree_),
                   make_end_filter_const(ptree_),
                   boost::bind(&this_type::addPoint,
                                boost::ref(result), _1));
    
    result.erase( -- -- result.end() );
    return result += ")";
  }

  static void addPoint( std::string& result, 
                        const typename Ptree::value_type& pair )
  {
    const std::string x = props::polygon::point::x::relative();
    const std::string y = props::polygon::point::y::relative();

    const Ptree& point = pair.second;     
    result += js_so::jsPoint( point.template get<boost::uint32_t>(x), 
                              point.template get<boost::uint32_t>(y) ) + ", ";
  }

  Ptree ptree_;
}; //struct js_polygon_creater

typedef basic_js_polygon<boost::property_tree::iptree> js_polygon;

} //namespace chm{
#endif //#if !defined( BA669AA0_EF10_4a56_B5DA_90FF38383232 )
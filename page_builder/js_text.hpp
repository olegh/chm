#if !defined(B7AAE111_146A_49d1_813E_77420B6103B7)
# define B7AAE111_146A_49d1_813E_77420B6103B7

#include <string>
#include <algorithm>
#include <boost/cstdint.hpp>
#include <boost/property_tree/ptree.hpp>

#include "page_builder/js_simple_objects.hpp"
#include "common/named_properties.hpp"
#include "common/ptree_helpers.hpp"

namespace chm
{
  template<class Ptree>
  class basic_js_text
  {
  public:

    typedef basic_js_text<Ptree> this_type;

    explicit basic_js_text( const Ptree& ptree )
      : ptree_(ptree)
    {}

    std::string operator()()
    {
      std::string result, text_relative_path = props::polygon::text::relative();
      std::for_each( make_begin_filter_const(text_relative_path, ptree_),
                     make_end_filter_const(ptree_),
                     boost::bind(&this_type::build_text,
                                 boost::ref(result), _1 )); 
      return result;
    }

  private:
    static void build_text( std::string& result, 
                            const typename Ptree::value_type& pair )
    {
      typedef js_simple_objects js_so;
      const Ptree& ptree = pair.second;
     
      const std::string text_value = 
        ptree.template get<std::string>(props::polygon::text::value::relative());    
      boost::int32_t x = 
        ptree.template get<boost::int32_t>(props::polygon::text::x::relative());
      boost::int32_t y =
        ptree.template get<boost::int32_t>(props::polygon::text::y::relative());
      boost::uint32_t color = 
        ptree.template get<boost::uint32_t>(props::polygon::text::color::relative());
      
      result += 
        "gr.drawText(\"" + text_value + "\", " + js_so::jsPoint( x, y) + ", " + 
        "\"Courier New\", " + js_so::jsColor(color) + ");\n";
    }
    
    Ptree ptree_;
  }; //class basic_js_text

  typedef basic_js_text<boost::property_tree::iptree> js_text;
} //namespace chm

#endif

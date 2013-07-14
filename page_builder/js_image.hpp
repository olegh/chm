#if !defined(f8db4110_ec9f_11e2_91e2_0800200c9a66)
#define f8db4110_ec9f_11e2_91e2_0800200c9a66

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

namespace chm
{

template<class Ptree>
struct basic_js_image
{
    typedef js_simple_objects js_so;
    typedef basic_js_image<Ptree> this_type;

    explicit basic_js_image( const Ptree& ptree )
        : ptree_(ptree)
    {}

    std::string operator()()
    {
      std::string result, image_relative_path = props::polygon::image::relative();
      std::for_each( make_begin_filter_const(image_relative_path, ptree_),
                     make_end_filter_const(ptree_),
                     boost::bind(&this_type::build_image,
                                 boost::ref(result), _1 )); 
      return result;
    }

private:
    static void build_image( std::string& result,
                             const typename Ptree::value_type& pair )
    {
        typedef js_simple_objects js_so;
        const Ptree& ptree = pair.second;

        const std::string url = 
            ptree.template get<std::string>(props::polygon::image::url::relative());    

        boost::int32_t x = 
            ptree.template get<boost::int32_t>(props::polygon::image::x::relative());

        boost::int32_t y =
            ptree.template get<boost::int32_t>(props::polygon::image::y::relative());

        result += "gr.drawImage(\"" + url + "\", " + js_so::jsPoint( x, y) + ");\n";
    }

private:
    Ptree ptree_;
};
    
typedef basic_js_image<boost::property_tree::iptree> js_image;
}

#endif

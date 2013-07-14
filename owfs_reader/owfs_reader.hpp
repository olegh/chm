#if !defined(FE135791_A5F8_4e41_A39F_5422485FD5DB)
# define FE135791_A5F8_4e41_A39F_5422485FD5DB

#include <algorithm>
#include <iostream>
#include <math.h>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/time_zone_base.hpp>
#include <boost/bind.hpp>
#include "common/named_properties.hpp"
#include "common/ptree_helpers.hpp"
#include "common/polygon_ptree_helpers.hpp"

namespace chm
{
  class owfs_reader
  {
  public:
    explicit owfs_reader(const std::string& owfs_path )
      : owfs_path_(owfs_path)
    {}

    template<class Ptree>
    void update_tree( Ptree& target_tree )
    {
      std::for_each( make_begin_filter(props::polygon::relative(), target_tree),
                     make_end_filter(target_tree),
                     boost::bind(&owfs_reader::update_polygon<Ptree>,
                                 this,
                                 _1 ));
    }

  private:

    template<class Ptree>
    void update_polygon( typename Ptree::value_type& pair )
    {
      Ptree& polygon = pair.second;
      if(read_temperature_from_owfs(polygon))
      {
        update_temperature( polygon );
        update_color( polygon );
        update_comment( polygon );
      }
      else
      {
        update_temperature_not_found( polygon );
        update_color_not_found( polygon );
        update_comment_not_found( polygon );
      }
    }

    template<class Ptree>
    void update_temperature( Ptree& polygon )
    {
      const std::string temperature = 
        polygon.template get<std::string>(props::polygon::temperature::relative());

      update_temperature_impl( polygon, temperature + "C" );
    }

    template<class Ptree>
    void update_temperature_impl( Ptree& polygon, 
                                  const std::string& temperature )
    {
      typedef typename Ptree::assoc_iterator assoc_iterator;
      assoc_iterator temperature_node_it = find_text_node( "temperature", polygon );
      if( polygon.not_found() == temperature_node_it )
        return;

      (*temperature_node_it).second.put(props::polygon::text::value::relative(),
        "Temperature: " + temperature );
    }

    template<class Ptree>
    void update_color( Ptree& polygon )
    {
      double temperature = 
        polygon.template get<double>(props::polygon::temperature::relative());

      polygon.put( props::polygon::color::relative(), make_color(temperature) );
    }

    boost::uint32_t make_color( double temperature )
    {
      double color_rad = double(temperature + 5) * 5.14 * 3.14 / 180;
      double x = ::cos( color_rad ) * 50;
      double y = ::sin( color_rad ) * 50;

      boost::uint8_t r = 200, g = 200, b = 200;
      
      g += static_cast<boost::uint8_t>(::abs(y));
      ( x > 0 ? b : r ) += static_cast<boost::uint8_t>(::abs(x));

      return  (r << 16) + (g <<8) + b; 
    }

    template<class Ptree>
    void update_comment( Ptree& polygon )
    {
      typedef typename Ptree::assoc_iterator assoc_iterator;
      assoc_iterator comment_node_it = find_text_node( "comment", polygon );
      if( polygon.not_found() == comment_node_it )
        return;

      const std::string cur_time = 
      boost::posix_time::to_simple_string
        ( boost::posix_time::second_clock::local_time());
      
      (*comment_node_it).second.put( props::polygon::text::value::relative(),
                                     "Last update at: " + cur_time );
    }

    template<class Ptree>
    void update_temperature_not_found( Ptree& polygon )
    {
      update_temperature_impl( polygon, "Unknown" );
    }

    template<class Ptree>
    void update_color_not_found( Ptree& polygon )
    {
      polygon.put( props::polygon::color::relative(), 0xffffff );
    }

    template<class Ptree>
    void update_comment_not_found( Ptree& polygon )
    {
      update_comment(polygon);
    }

    template<class Ptree>
    bool read_temperature_from_owfs( Ptree& polygon )
    {
      namespace fs = boost::filesystem;
      if( !fs::exists(owfs_path_))
        return false;

      fs::path temperature_file = owfs_path_ /
        fs::path(polygon.template get<std::string>(props::polygon::thermometer_id::relative())) /
        fs::path("temperature");

      if( !fs::exists(temperature_file))
        return false;

      std::string file_string = temperature_file.string();
      double temperature;
      std::ifstream(file_string.c_str()) >> temperature;

      polygon.put( props::polygon::temperature::relative(), temperature );
      return true;
    }

    boost::filesystem::path owfs_path_;
  };
} //namespace chm

#endif //FE135791_A5F8_4e41_A39F_5422485FD5DB


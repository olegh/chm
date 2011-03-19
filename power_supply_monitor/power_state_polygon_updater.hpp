#if !defined( b0e6fa56_bef5_4d2a_98ba_7a449e71f33c )
#define b0e6fa56_bef5_4d2a_98ba_7a449e71f33c

#include <string>
#include <boost/property_tree/ptree.hpp>
#include "common/polygon_ptree_helpers.hpp"

namespace chm{

class power_state_polygon_updater
{
public:
  explicit power_state_polygon_updater( const std::string& ac_adapter_path = "/proc/acpi/ac_adapter" )
    : impl_(ac_adapter_path)
  {}
  //============================================================

  template<class Ptree>
  void update_tree( Ptree& target_tree )
  {
     std::for_each( make_begin_filter(props::polygon::relative(), target_tree),
                    make_end_filter(target_tree),
                    boost::bind(&power_state_polygon_updater::update_polygon<Ptree>,
                                 this,
                                  _1 ));
  }
  //=================================================================

private:

  template<class Ptree>
  void update_polygon( typename Ptree::value_type& pair )
  {
    Ptree& polygon = pair.second;
    const std::string power_state = std::string("Power is: ") + (impl_.is_power_on() ? "On" : "Off");

    typedef typename Ptree::assoc_iterator assoc_iterator;
    assoc_iterator power_node_it = find_text_node( "power", polygon );
    if( polygon.not_found() == power_node_it )
      return;

    (*power_node_it).second.put(props::polygon::text::value::relative(), power_state );
  }
  //=================================================================

	chm::power_supply_monitor impl_;
}; //class power_state_polygon_updater

} //namespace chm{

#endif //#define b0e6fa56_bef5_4d2a_98ba_7a449e71f33c


#if !defined(cc0b8df2_64bf_4b0b_bd5a_e6df6417373b)
#define cc0b8df2_64bf_4b0b_bd5a_e6df6417373b

#include <string>
#include <boost/bind.hpp>
#include "version/version.hpp"
#include "common/named_properties.hpp"
#include "common/polygon_ptree_helpers.hpp"
#include "common/ptree_helpers.hpp"

namespace chm
{
class version_polygon_updater
{
public:
  template<class Ptree>
  void update_tree( Ptree& target_tree )
  {
     std::for_each( make_begin_filter(props::polygon::relative(), target_tree),
                    make_end_filter(target_tree),
                    boost::bind(&version_polygon_updater::update_polygon<Ptree>,
                                 this,
                                 _1 ));
  }
  //=================================================================

  private:

    template<class Ptree>
    void update_polygon( typename Ptree::value_type& pair )
    {
      Ptree& polygon = pair.second;
      const std::string verstion_value = version::to_str();

      typedef typename Ptree::assoc_iterator assoc_iterator;
      assoc_iterator power_node_it = find_text_node( "version", polygon );
      if( polygon.not_found() == power_node_it )
        return;

      (*power_node_it).second.put(props::polygon::text::value::relative(), "Program version: " + verstion_value );
    }
    //=================================================================
};//class version_polygon_updater
}

#endif //cc0b8df2_64bf_4b0b_bd5a_e6df6417373b

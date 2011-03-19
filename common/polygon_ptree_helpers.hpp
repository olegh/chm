#if !defined( d76783b1_66b2_4e74_8b0a_605ab865311e )
# define d76783b1_66b2_4e74_8b0a_605ab865311e

#include <string>
#include <boost/property_tree/ptree.hpp>
#include "common/named_properties.hpp"

namespace chm
{
template<class Ptree>
inline typename Ptree::assoc_iterator
find_text_node( const std::string& name_in_text_node, Ptree& polygon  )
{
	typename Ptree::assoc_iterator it = polygon.ordered_begin(),
																 endIt = polygon.not_found();

	for( ; endIt != it; ++it )
	{
		if( (*it).first == props::polygon::text::relative())
		{
			const std::string name_in_cur_text_node =
				(*it).second.template get<std::string>(props::polygon::text::name::relative());

			if( name_in_text_node == name_in_cur_text_node )
				return it;
		}
	}

	return polygon.not_found();
}
//======================================================
} //namespace chm

#endif //d76783b1_66b2_4e74_8b0a_605ab865311e

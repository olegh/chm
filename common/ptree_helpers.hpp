#if !defined(CB90FD3C_4F7A_4192_8E9C_C92A12EA4C35)
# define CB90FD3C_4F7A_4192_8E9C_C92A12EA4C35

#include <string>
#include <functional>
#include <boost/iterator/filter_iterator.hpp>

namespace chm
{
  namespace detail
  {
    template<class Ptree>
    struct IsRelativePath
      : public std::unary_function<typename Ptree::value_type,bool>
    {
      typedef IsRelativePath<Ptree> this_type;
      typedef typename std::unary_function<typename Ptree::value_type,bool> base;
      typedef typename base::result_type result_type;
      typedef typename base::argument_type argument_type;

      explicit IsRelativePath( const std::string& relative_path )
        : relative_path_(relative_path)
      {}

      result_type
        operator()( const argument_type& pair ) const
      {
        return pair.first == relative_path_;
      }

    private:
      std::string relative_path_;
    };
  }
  

  template<class Ptree> static
  boost::filter_iterator<detail::IsRelativePath<Ptree>, typename Ptree::const_assoc_iterator>
  make_begin_filter_const( const std::string& relative_path,
                           const Ptree& tree )
  {
    return boost::make_filter_iterator( detail::IsRelativePath<Ptree>((relative_path)),
                                        tree.ordered_begin(),
                                        tree.not_found() );
  }

  template<class Ptree> static
  boost::filter_iterator<detail::IsRelativePath<Ptree>, typename Ptree::const_assoc_iterator>
  make_end_filter_const( const Ptree& tree )
  {
    return boost::make_filter_iterator( detail::IsRelativePath<Ptree>(("")),
                                        tree.not_found(),
                                        tree.not_found() );
  }

  template<class Ptree> static
  boost::filter_iterator<detail::IsRelativePath<Ptree>, typename Ptree::assoc_iterator>
  make_begin_filter( const std::string& relative_path,
                     Ptree& tree )
  {
    return boost::make_filter_iterator( detail::IsRelativePath<Ptree>((relative_path)),
                                        tree.ordered_begin(),
                                        tree.not_found() );
  }

  template<class Ptree> static
  boost::filter_iterator<detail::IsRelativePath<Ptree>, typename Ptree::assoc_iterator>
  make_end_filter( Ptree& tree )
  {
    return boost::make_filter_iterator( detail::IsRelativePath<Ptree>(("")),
                                        tree.not_found(),
                                        tree.not_found() );
  }
}

#endif //#if !defined(CB90FD3C_4F7A_4192_8E9C_C92A12EA4C35)

//EOF

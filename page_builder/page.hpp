#if !defined(CFADBB84_89B2_4e01_8E16_1559AA171875)
# define CFADBB84_89B2_4e01_8E16_1559AA171875

#include <string>
#include <vector>
#include <boost/function.hpp>

namespace chm
{
  class page
  {
  public:
    typedef boost::function<std::string()> content_maker;
    typedef std::vector<content_maker> impl_type;
    typedef impl_type::value_type value_type;
    typedef impl_type::reference reference;
    typedef impl_type::size_type size_type;
    typedef impl_type::const_iterator const_iterator;
    typedef impl_type::const_reverse_iterator const_reverse_iterator;
    typedef impl_type::difference_type difference_type;
    typedef impl_type::iterator iterator;
    typedef impl_type::reverse_iterator reverse_iterator;

    void swap( page& other )
    {
      impl_.swap( other.impl_ );
    }

    size_type size() const
    {
      return impl_.size();
    }

    const_iterator begin() const
    {
      return impl_.begin();
    }

    iterator begin()
    {
      return impl_.begin();
    }

    const_iterator end() const
    {
      return impl_.end();
    }

    iterator end()
    {
      return impl_.end();
    }

    void push_back( const value_type& val )
    {
      impl_.push_back( val );
    }

    std::string build() const
    {
      return before_content() + content() + after_content();
    }

    std::string operator()() const
    {
      return build();
    }

    page& operator<<( const value_type& val )
    {
      push_back( val );
      return *this;
    }

  private:

    std::string content() const
    {
      std::string result;
      const_iterator it = begin(), endIt = end();
      for( ;endIt != it; ++it )
      {
        result += (*it)();
      }

      return result;
    }

    std::string before_content() const
    {
      return
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
            "var gr = new jsGraphics(document.getElementById(\"canvas\"));\n";
    }

    std::string after_content() const
    {
      return
          "</script>\n"
        "</body>\n"
      "</html>\n";
    }

    impl_type impl_;
  };
}

#endif //#if !defined(CFADBB84_89B2_4e01_8E16_1559AA171875)
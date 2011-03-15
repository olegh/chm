#if !defined(A363B400_A254_4eab_AD20_5930D5EDED7B)
#define A363B400_A254_4eab_AD20_5930D5EDED7B

#include <boost/lexical_cast.hpp>
#include <string>
#include <sstream>
#include <boost/cstdint.hpp>

namespace chm
{
  struct js_simple_objects
  {
    static std::string jsPoint( boost::int32_t x, boost::int32_t y )
    {
      return std::string( "new jsPoint( ") + to_str( x ) + ", " + to_str( y ) + " )";
    }

    static std::string jsColor( boost::uint32_t color )
    {
      std::stringstream ss;
      ss << std::hex << std::uppercase << color;
      std::string color_value = ss.str();
      if( color_value.size() > 6 )
        color_value.resize( 6, '0');
      
      if( color_value.size() < 6 )
      {
        std::string zeros;
        zeros.resize( 6 - color_value.size(), '0' );
        zeros += color_value;
        zeros.swap( color_value );
      }
      
      return "new jsColor( \"#" + color_value + "\" )";
    }

    static std::string jsPen( boost::uint32_t color = 0, boost::uint32_t width = 3 )
    {
      return "new jsPen( " + jsColor( color ) + ", " + to_str( width ) + " )";
    }
  
  private:
    template<typename T> static
    std::string to_str( const T& value )
    {
      return boost::lexical_cast<std::string>(value);
    }
  };
} //namespace chm

#endif //#define A363B400_A254_4eab_AD20_5930D5EDED7B
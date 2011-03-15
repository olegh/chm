#if !defined(B04AC70B_7FBB_4f06_B21A_BEFEED5BE8CB)
# define B04AC70B_7FBB_4f06_B21A_BEFEED5BE8CB 

#include <string>

namespace chm
{
struct props
{
  struct polygon{
    static std::string full(){ return relative(); }
    static std::string relative(){ return "polygon"; }

    struct point{
      static std::string full(){ return polygon::full() + "." + relative(); }
      static std::string relative(){ return "point"; }

      struct x{
        static std::string full(){ return point::full() + "." + relative(); }
        static std::string relative(){ return "x"; }
      };

      struct y{
        static std::string full(){ return point::full() + "." + relative(); }
        static std::string relative(){ return "y"; }
      };
    };
    struct thermometer_id{
      static std::string full(){ return polygon::full() + "." + relative(); }
      static std::string relative(){ return "thermometer_id"; }
    };

    struct temperature{
      static std::string full(){ return polygon::full() + "." + relative(); }
      static std::string relative(){ return "temperature"; }
    };

    struct color{
      static std::string full(){ return polygon::full() + "." + relative(); }
      static std::string relative(){ return "color"; }
    };

    struct text{
      static std::string full(){ return polygon::full() + "." + relative(); }
      static std::string relative(){ return "text"; }

      struct x{
        static std::string full(){ return text::full() + "." + relative(); }
        static std::string relative(){ return "x"; }
      };

      struct y{
        static std::string full(){ return text::full() + "." + relative(); }
        static std::string relative(){ return "y"; }
      };

      struct name{
        static std::string full(){ return text::full() + "." + relative(); }
        static std::string relative(){ return "name"; }
      };

      struct value{
        static std::string full(){ return text::full() + "." + relative(); }
        static std::string relative(){ return "value"; }
      };

      struct color{
        static std::string full(){ return text::full() + "." + relative(); }
        static std::string relative(){ return "color"; }
      };
    };
  };
};

} //namespace chm

#endif //#if !define(B04AC70B_7FBB_4f06_B21A_BEFEED5BE8CB)
//EOF

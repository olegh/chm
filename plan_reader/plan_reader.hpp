#if !defined(BB58D060_B0DD_434b_8F12_11F1CFAACA6D)
# define BB58D060_B0DD_434b_8F12_11F1CFAACA6D 

#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

namespace chm
{
  namespace privateimpl
  {
    
    /**
     * @Comment: interface for reader implementation
     */
    template<class Ptree>
    class plan_reader_impl
    {
    public:
      typedef Ptree ptree;

      plan_reader_impl(){}
      virtual ~plan_reader_impl(){}
      
      /**
        * @Comment: read property tree
        */
      virtual ptree read() = 0;

    private:
      plan_reader_impl( const plan_reader_impl& );
      plan_reader_impl& operator=( const plan_reader_impl& );
    }; //class plan_reader_impl

    
    
    
    /**
      * @comment: template implementation for reader from stream
      */
    template<class Stream, class Reader, class Ptree>
    class plan_reader_specific
      : public plan_reader_impl<Ptree>
    {
    public:

      typedef plan_reader_impl<Ptree> base;
      typedef typename base::ptree ptree;

      /**
       * @Parameter: functor for read from stream
       */
      explicit 
      plan_reader_specific( Stream& stream, 
                            Reader reader )
        : stream_( stream ),
          reader_( reader )
      {}

      /**
        * @Comment: overrides pure virtual base method read
        *           implements read from stream with functor
        */
      virtual ptree read()
      {
        ptree tree;
        reader_( stream_, tree );
        
        return tree;
      }
    
    private:
      Stream& stream_;
      Reader reader_;
    }; //class plan_reader_impl
  } //namespace impl

  /**
    * @Comment: reads property tree from stream with specific reader
    */
  template<class Ptree>
  class basic_plan_reader
  {
    typedef boost::shared_ptr<privateimpl::plan_reader_impl<Ptree> > impl_pointer;
  public:

    typedef typename privateimpl::plan_reader_impl<Ptree>::ptree ptree;
    typedef basic_plan_reader<Ptree> this_type;
    typedef boost::shared_ptr<this_type> pointer;

    /**
      *@Parameter: stream - stream read from
      *@Parameter: reader - reader from stream, xml reader by default
      */
    template<class Stream, class Reader> explicit
    basic_plan_reader( Stream& stream, 
                       Reader reader )
      :defaultStream_(),
       impl_( new privateimpl::plan_reader_specific<Stream,Reader,ptree>(stream,reader))
    {}

    /**
      *@Comment: make xml reader from stream
      */
    template<class Stream> static boost::function<void( Stream&, ptree&)>
    make_xml_reader( Stream& )
    {
      using boost::property_tree::xml_parser::read_xml;
      return boost::bind( read_xml<ptree>, _1, _2, 0 );
    }

    /**
      *@Comment: make info format reader
      */
    template<class Stream> static boost::function<void( Stream&, ptree&)>
    make_info_reader( Stream& )
    {
      
      using boost::property_tree::info_parser::read_info;
      return boost::bind( read_info<ptree,typename Stream::char_type>, _1, _2 );
    }


    /**
      * @Parameter: fileName - name of file with tree
      * @Parameter: reader from stream
      */
    template<class Reader> explicit
    basic_plan_reader( const std::string& fileName,
                 Reader reader )
      : defaultStream_(fileName),
        impl_(new privateimpl::plan_reader_specific<std::ifstream,Reader,ptree>(defaultStream_,reader))
    {
    }

    /**
      * @Comment: returns read tree
      */
    ptree read()
    {
      return impl_->read();
    }

  private:
    this_type& operator=( const this_type& );
    basic_plan_reader( const this_type& );

    std::ifstream defaultStream_;
    impl_pointer impl_;
  };


  typedef basic_plan_reader<boost::property_tree::iptree> plan_reader;
  
} //namespace chm

#endif //#if !defined(BB58D060_B0DD_434b_8F12_11F1CFAACA6D) 
//EOF


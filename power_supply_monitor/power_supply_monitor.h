/*
 * power_supply_monitor.h
 *
 *  Created on: Mar 13, 2011
 *      Author: oleg
 */

#include <boost/bind.hpp>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <cstdio>
#include <string>
#include <fstream>
#include <boost/function.hpp>
#include <boost/filesystem.hpp>
#include <boost/exception/all.hpp>

#if !defined A0b14d60_4daa_11e0_b8af_0800200c9a66
#define A0b14d60_4daa_11e0_b8af_0800200c9a66

namespace chm{ namespace detail{

class stream_wrapper
{
public:
	typedef boost::shared_ptr<stream_wrapper> pointer;

  virtual std::basic_istream<char>& get() = 0;

  stream_wrapper(){}
  virtual ~stream_wrapper(){}

private:
  stream_wrapper( const stream_wrapper& );
  stream_wrapper& operator=( const stream_wrapper& );
}; //class stream_wrapper

/**
 * @Comment: simple wraps stream
 */
class stream_wrapper_impl
  : public stream_wrapper
{
public:
  explicit stream_wrapper_impl( std::basic_istream<char>& wrapped_stream )
    : wrapped_stream_(wrapped_stream)
  {}

  virtual std::basic_istream<char>& get()
	{
    return wrapped_stream_;
	}
private:
  std::basic_istream<char>& wrapped_stream_;
}; //class stream_wrapper_impl

 /**
  * @Comment: wraps stream + make lazy file open
  */
class stream_wrapper_lazy_init
 : public stream_wrapper
{
public:
  explicit stream_wrapper_lazy_init( const std::string& file_name )
    : file_name_(file_name)
  {}

  virtual std::basic_istream<char>& get()
	{
  	std::ifstream state_file((file_name_.c_str()));
  	if( !state_file.good() )
  	{
  	  BOOST_THROW_EXCEPTION( std::runtime_error("Can't open file:" + file_name_));
  	}

  	std::istream_iterator<char> begin(state_file);
  	std::istream_iterator<char> end;
  	std::string fileContent;
  	std::copy( begin, end, std::back_inserter(fileContent) );

  	wrapped_stream_.clear();
  	wrapped_stream_.str(fileContent);
  	return wrapped_stream_;
	}
private:
  std::string file_name_;
  std::stringstream wrapped_stream_;
};

stream_wrapper::pointer
make_stream_wrapper( std::basic_istream<char>& stream )
{
	return stream_wrapper::pointer( new stream_wrapper_impl(stream));
}

stream_wrapper::pointer
make_stream_wrapper( const std::string& file_name )
{
	return stream_wrapper::pointer( new stream_wrapper_lazy_init(file_name));
}
}} //namespace chm{ namespace detail

namespace chm
{
class power_supply_monitor
{
public:
  explicit power_supply_monitor( const std::string& ac_adapters_path = "/proc/acpi/ac_adapter" )
   : stream_wrapper_()
  {
    using namespace boost::filesystem;
  	path ac_adapters_dir = ac_adapters_path;
  	boost::filesystem::directory_iterator dirIter((ac_adapters_dir));
  	path ac_dir = *dirIter,
    state = ac_dir / "state";

    stream_wrapper_ = detail::make_stream_wrapper( state.file_string() );
  }

  explicit
  power_supply_monitor( std::basic_istream<char>& stream )
   : stream_wrapper_()
  {
  	stream_wrapper_ = detail::make_stream_wrapper( stream );
  }

  bool is_power_on()
  {
	  const std::string onLineStr = "on-line";
	  std::istream_iterator<char> begin(stream_wrapper_->get());
	  std::istream_iterator<char> end;
	  std::string fileContent;
	  std::copy( begin, end, std::back_inserter(fileContent) );

    return fileContent.end() !=
    		std::search( fileContent.begin(),
    		             fileContent.end(),
    		             onLineStr.begin(),
    		             onLineStr.end() );
  }

private:
  detail::stream_wrapper::pointer stream_wrapper_;
}; //class power_supply_monitor
} //namespace chm

#endif /* A0b14d60_4daa_11e0_b8af_0800200c9a66 */

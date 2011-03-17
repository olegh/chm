#if !defined( d38f75f4_c5ed_412a_91f0_4d6d6ef47092)
# define d38f75f4_c5ed_412a_91f0_4d6d6ef47092

#include <string>
#include <boost/function.hpp>
#include <boost/date_time.hpp>

namespace chm{

/**
 * @Comment: notifies when power state changed
 */
class power_state_alarmer
{
public:
	typedef boost::function<void( const std::string& )> Notificator;
	typedef boost::function<bool()> PowerState;
	typedef boost::function<boost::posix_time::ptime ()> TimeNow;

	power_state_alarmer( Notificator notificator,
			                 PowerState powerState,
			                 TimeNow timeNow = default_time_now() )
	: notificator_(notificator),
	  power_state_(powerState),
	  time_now_(timeNow),
	  isPowerOn_(powerState())
	{}
	//=====================================================================

	void notify_if_power_state_changed()
	{
    bool powerState = power_state_();
		if( isPowerOn_ != powerState )
    {
			isPowerOn_ = powerState;
    	notificator_( std::string("Power state is: ") + (isPowerOn_ ? "On" : "Off") + ", checked at: " +
    			boost::posix_time::to_simple_string(time_now_()));
    }
	}
	//=====================================================================

private:

	static TimeNow default_time_now()
	{
    return boost::bind( &power_state_alarmer::local_time );
	}

	static boost::posix_time::ptime
	local_time()
	{
		return boost::posix_time::second_clock::local_time();
	}

	Notificator notificator_;
	PowerState power_state_;
	TimeNow time_now_;
	bool isPowerOn_;
};

} //namespace chm{

#endif //# define d38f75f4_c5ed_412a_91f0_4d6d6ef47092

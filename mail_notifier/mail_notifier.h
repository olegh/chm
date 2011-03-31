#if !defined( e55c1ed8_706b_4135_8b0f_b9c11b6c3893 )
# define e55c1ed8_706b_4135_8b0f_b9c11b6c3893

#include <string>
#include <vector>
#include <boost/function.hpp>

namespace chm{
/**
 * @Comment: sends mail
 */
class mail_notifier
{
public:
	typedef boost::function<int (const std::string&)> Console;
	typedef boost::function<std::string()> TmpFileNameCreator;

	/**
	 * @Parameter: console - functor which execute command in console or create new porcess
	 * @Parameter: tmpFileNameCreater - functor which generates tmp file name
	 */
	explicit mail_notifier( Console console = create_default_console(),
			                    TmpFileNameCreator tmpFileNameCreater  = create_default_tmp_file_name_creater() );

	/**
	 * @Comment: sends mail to address at @email
	 * @Parameter: email - target e-mail address, can't be empty
	 * @Parameter: subject - header of the letter
	 * @Parameter: message - body of the letter
	 * @Throws: yes
	 */
	void notify( const std::string& email,
			     const std::string& subject,
			     const std::string& message );


	/**
	  * @Comment: sends mail to address at @email
	  * @Parameter: email - target e-mail address, can't be empty
	  * @Parameter: subject - header of the letter
	  * @Parameter: message - body of the letter
	  * @Throws: yes
	  */
	void notify_by_list( const std::vector< std::string >& mail_list,
	                     const std::string& subject,
	                     const std::string& message );



private:

	static void file_deleter( const std::string& file_name );

	static Console create_default_console();

	static TmpFileNameCreator create_default_tmp_file_name_creater();

	Console console_;
	TmpFileNameCreator tmp_file_name_creater_;
};

} //namespace chm

#endif //#if !defined( e55c1ed8_706b_4135_8b0f_b9c11b6c3893 )


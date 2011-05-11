/*!
 * @file sr_ui.cc
 * @brief System reporter class for UI - definitions.
 *
 * @author Piotr Trojanek <piotr.trojanek@gmail.com>
 *
 * @ingroup LIB
 */

#ifndef SR_UI_H_
#define SR_UI_H_

#include "base/lib/sr/srlib.h"

namespace mrrocpp {
namespace lib {

//! SR class for use in UI
class sr_ui : public sr
{
protected:
	//! Interpret the status code into a text message
	virtual void interpret(char * description, error_class_t message_type, uint64_t error_code0, uint64_t error_code1);

	//! Interprets non fatal error.
	virtual void interpret(char * description, const mrrocpp::lib::exception::mrrocpp_non_fatal_error & _e);

	//! Interprets fatal error.
	virtual void interpret(char * description, const mrrocpp::lib::exception::mrrocpp_fatal_error & _e);

	//! Interprets system error.
	virtual void interpret(char * description, const mrrocpp::lib::exception::mrrocpp_system_error & _e);

public:
	/**
	 * Constructor
	 * @param process_type reporting process type
	 * @param process_name reporting process name
	 * @param sr_channel_name name of the SR communication channel
	 * @param _multi_thread flag for selecting multi-threaded variant
	 */
	sr_ui(process_type_t process_type, const std::string & process_name, const std::string & sr_channel_name);

};

} // namespace lib
} // namespace mrrocpp

#endif /* SR_UI_H_ */

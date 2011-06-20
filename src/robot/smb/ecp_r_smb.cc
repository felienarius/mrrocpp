/*!
 * @file
 * @brief File contains ecp robot class definition for SwarmItFix Mobile Base
 * @author twiniars <twiniars@ia.pw.edu.pl>, Warsaw University of Technology
 *
 * @ingroup smb
 */

#include "base/lib/impconst.h"

#include "robot/smb/ecp_r_smb.h"
#include "base/lib/sr/sr_ecp.h"
#include "robot/smb/kinematic_model_smb.h"

namespace mrrocpp {
namespace ecp {
namespace smb {

robot::robot(const lib::robot_name_t & _robot_name, lib::configurator &_config, lib::sr_ecp &_sr_ecp) :
	ecp::common::robot::ecp_robot(_robot_name, lib::smb::NUM_OF_SERVOS, _config, _sr_ecp),
			epos_cubic_command_data_port(lib::epos::EPOS_CUBIC_COMMAND_DATA_PORT, port_manager),
			epos_trapezoidal_command_data_port(lib::epos::EPOS_TRAPEZOIDAL_COMMAND_DATA_PORT, port_manager),
			smb_multi_pin_insertion_data_port(lib::smb::MULTI_PIN_INSERTION_DATA_PORT, port_manager),
			smb_multi_pin_locking_data_port(lib::smb::MULTI_PIN_LOCKING_DATA_PORT, port_manager),
			epos_reply_data_request_port(lib::epos::EPOS_REPLY_DATA_REQUEST_PORT, port_manager),
			smb_multi_leg_reply_data_request_port(lib::smb::MULTI_LEG_REPLY_DATA_REQUEST_PORT, port_manager)
{

	create_kinematic_models_for_given_robot();
}

robot::robot(const lib::robot_name_t & _robot_name, common::task::task_base& _ecp_object) :
	ecp::common::robot::ecp_robot(_robot_name, lib::smb::NUM_OF_SERVOS, _ecp_object),
			epos_cubic_command_data_port(lib::epos::EPOS_CUBIC_COMMAND_DATA_PORT, port_manager),
			epos_trapezoidal_command_data_port(lib::epos::EPOS_TRAPEZOIDAL_COMMAND_DATA_PORT, port_manager),
			smb_multi_pin_insertion_data_port(lib::smb::MULTI_PIN_INSERTION_DATA_PORT, port_manager),
			smb_multi_pin_locking_data_port(lib::smb::MULTI_PIN_LOCKING_DATA_PORT, port_manager),
			epos_reply_data_request_port(lib::epos::EPOS_REPLY_DATA_REQUEST_PORT, port_manager),
			smb_multi_leg_reply_data_request_port(lib::smb::MULTI_LEG_REPLY_DATA_REQUEST_PORT, port_manager)
{

	create_kinematic_models_for_given_robot();
}

// Stworzenie modeli kinematyki dla robota IRp-6 na postumencie.
void robot::create_kinematic_models_for_given_robot(void)
{
	// Stworzenie wszystkich modeli kinematyki.
	add_kinematic_model(new kinematics::smb::model());
	// Ustawienie aktywnego modelu.
	set_kinematic_model(0);
}

void robot::create_command()
{

	//	int new_data_counter;
	bool is_new_data;
	bool is_new_request;

	sr_ecp_msg.message("create_command");

	is_new_data = false;

	if (epos_cubic_command_data_port.get() == mrrocpp::lib::NewData) {
		ecp_command.set_type = ARM_DEFINITION;
		// generator command interpretation
		// narazie proste przepisanie

		ecp_edp_cbuffer.variant = lib::smb::CBUFFER_EPOS_CUBIC_COMMAND;

		ecp_edp_cbuffer.epos_cubic_command_structure = epos_cubic_command_data_port.data;

		if (is_new_data) {
			throw common::robot::ECP_error(lib::NON_FATAL_ERROR, INVALID_COMMAND_TO_EDP);
		} else {
			is_new_data = true;
		}
	}

	if (epos_trapezoidal_command_data_port.get() == mrrocpp::lib::NewData) {
		ecp_command.set_type = ARM_DEFINITION;
		// generator command interpretation
		// narazie proste przepisanie

		ecp_edp_cbuffer.variant = lib::smb::CBUFFER_EPOS_TRAPEZOIDAL_COMMAND;

		ecp_edp_cbuffer.epos_trapezoidal_command_structure = epos_trapezoidal_command_data_port.data;

		if (is_new_data) {
			throw common::robot::ECP_error(lib::NON_FATAL_ERROR, INVALID_COMMAND_TO_EDP);
		} else {
			is_new_data = true;
		}
	}

	/*
	 if (epos_gen_parameters_data_port.get(epos_gen_parameters_structure) == mrrocpp::lib::NewData) {
	 ecp_command.set_type = ARM_DEFINITION;
	 // generator command interpretation
	 // narazie proste przepisanie

	 ecp_edp_cbuffer.variant = lib::smb::CBUFFER_EPOS_GEN_PARAMETERS;

	 ecp_edp_cbuffer.epos_gen_parameters_structure = epos_gen_parameters_structure;

	 if (is_new_data) {
	 throw common::robot::ECP_error(lib::NON_FATAL_ERROR, INVALID_COMMAND_TO_EDP);
	 } else {
	 is_new_data = true;
	 }
	 }
	 */
	if (smb_multi_pin_insertion_data_port.get() == mrrocpp::lib::NewData) {
		ecp_command.set_type = ARM_DEFINITION;
		// generator command interpretation
		// narazie proste przepisanie

		ecp_edp_cbuffer.variant = lib::smb::CBUFFER_PIN_INSERTION;

		ecp_edp_cbuffer.multi_pin_insertion = smb_multi_pin_insertion_data_port.data;

		if (is_new_data) {
			throw common::robot::ECP_error(lib::NON_FATAL_ERROR, INVALID_COMMAND_TO_EDP);
		} else {
			is_new_data = true;
		}
	}

	if (smb_multi_pin_locking_data_port.get() == mrrocpp::lib::NewData) {
		ecp_command.set_type = ARM_DEFINITION;
		// generator command interpretation
		// narazie proste przepisanie

		ecp_edp_cbuffer.variant = lib::smb::CBUFFER_PIN_LOCKING;

		ecp_edp_cbuffer.multi_pin_locking = smb_multi_pin_locking_data_port.data;

		if (is_new_data) {
			throw common::robot::ECP_error(lib::NON_FATAL_ERROR, INVALID_COMMAND_TO_EDP);
		} else {
			is_new_data = true;
		}
	}

	is_new_request = (epos_reply_data_request_port.is_new_request()
			|| smb_multi_leg_reply_data_request_port.is_new_request());

	communicate_with_edp = true;

	if (is_new_data && is_new_request) {
		ecp_command.instruction_type = lib::SET_GET;
	} else if (is_new_data) {
		ecp_command.instruction_type = lib::SET;
	} else if (is_new_request) {
		ecp_command.instruction_type = lib::GET;
	} else {
		communicate_with_edp = false;
	}

	if (is_new_request) {
		ecp_command.get_type = ARM_DEFINITION;
	}

	// message serialization
	if (communicate_with_edp) {
		memcpy(ecp_command.arm.serialized_command, &ecp_edp_cbuffer, sizeof(ecp_edp_cbuffer));
		assert(sizeof(ecp_command.arm.serialized_command) >= sizeof(ecp_edp_cbuffer));
	}
}

void robot::get_reply()
{

	// message deserialization
	memcpy(&edp_ecp_rbuffer, reply_package.arm.serialized_reply, sizeof(edp_ecp_rbuffer));

	if (epos_reply_data_request_port.is_new_request()) {
		// generator reply generation
		for (int i = 0; i < lib::smb::NUM_OF_SERVOS; i++) {
			epos_reply_data_request_port.data.epos_controller[i].position = edp_ecp_rbuffer.epos_controller[i].position;
			epos_reply_data_request_port.data.epos_controller[i].current = edp_ecp_rbuffer.epos_controller[i].current;
			epos_reply_data_request_port.data.epos_controller[i].motion_in_progress
					= edp_ecp_rbuffer.epos_controller[i].motion_in_progress;
		}
		epos_reply_data_request_port.set();
	}

	if (smb_multi_leg_reply_data_request_port.is_new_request()) {
		smb_multi_leg_reply_data_request_port.data = edp_ecp_rbuffer.multi_leg_reply;
		smb_multi_leg_reply_data_request_port.set();
	}

}

} // namespace smb
} // namespace ecp
} // namespace mrrocpp


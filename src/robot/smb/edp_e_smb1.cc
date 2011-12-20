
#include "base/lib/typedefs.h"
#include "base/lib/impconst.h"
#include "base/lib/com_buf.h"
#include "base/lib/mrmath/mrmath.h"

#include "edp_e_smb1.h"
#include "const_smb1.h"
#include "base/edp/reader.h"
// Kinematyki.
#include "robot/smb/kinematic_model_smb.h"

#include "base/lib/exception.h"
using namespace mrrocpp::lib::exception;

namespace mrrocpp {
namespace edp {
namespace smb1 {

// Konstruktor.
effector::effector(common::shell &_shell) :
	smb::effector(_shell, lib::smb1::ROBOT_NAME)
{

}

void effector::synchronise(void)
{
	try {
		if (robot_test_mode) {
			controller_state_edp_buf.is_synchronised = true;
			return;
		}

		controller_state_edp_buf.is_synchronised = false;

		// Step 1: Setup velocity control with analog setpoint.
		pkm_rotation_node->setOperationMode(maxon::epos::OMD_POSITION_MODE);

		// Velocity and acceleration limits.
		pkm_rotation_node->setMaxProfileVelocity(50);
		pkm_rotation_node->setMaxAcceleration(3000);

		// Velocity setpoint configuration.
		pkm_rotation_node->configureAnalogInput(1, maxon::epos::VELOCITY_SETPOINT);

		// NOTE: We assume, that scaling and offset are already set in the EPOS2

		// Get the target offset.
		const maxon::INTEGER32 offset = pkm_rotation_node->getAnalogVelocitySetpointOffset();

		// Start motion.
		pkm_rotation_node->setControlword(0x000F);

		// Setup timer for monitoring.
		boost::system_time wakeup = boost::get_system_time();

		// Loop until reaching zero offset.
		while(pkm_rotation_node->getAnalogInput1() != offset) {
			std::cout << "AnalogInput(1) = " << (unsigned int) pkm_rotation_node->getAnalogInput1() <<
					" offset " << (unsigned int) offset << std::endl;
			// Sleep for a constant period of time
			wakeup += boost::posix_time::milliseconds(5);

			boost::thread::sleep(wakeup);
		};

		// Step 2: Homing.
		// Activate homing mode.
		//pkm_rotation_node->doHoming(maxon::epos::HM_INDEX_NEGATIVE_SPEED, -1970);
		// Step-by-step homing in order to omit the offset setting (the value will be stored in the EPOS for every agent separatelly).
		pkm_rotation_node->setOperationMode(maxon::epos::OMD_HOMING_MODE);
		pkm_rotation_node->reset();
		pkm_rotation_node->startHoming();
		pkm_rotation_node->monitorHomingStatus();

		// Restore velocity and acceleration limits.
		pkm_rotation_node->setMaxProfileVelocity(Vdefault[1]);
		pkm_rotation_node->setMaxAcceleration(Adefault[1]);

		// Compute joints positions in the home position
		get_current_kinematic_model()->mp2i_transform(current_motor_pos, current_joints);

		// Homing of the motor controlling the legs rotation - set current position as 0.
		//legs_rotation_node->doHoming(mrrocpp::edp::maxon::epos::HM_ACTUAL_POSITION, 0);
		legs_relative_zero_position = legs_rotation_node->getActualPosition();

		// Check whether the synchronization was successful.
		check_controller_state();

		// Throw non-fatal error - if synchronization wasn't successful.
		if (!controller_state_edp_buf.is_synchronised) {
			BOOST_THROW_EXCEPTION(mrrocpp::edp::exception::fe_synchronization_unsuccessful());
		}

	} catch (mrrocpp::lib::exception::non_fatal_error & e_) {
		// Standard error handling.
		HANDLE_EDP_NON_FATAL_ERROR(e_)
	} catch (mrrocpp::lib::exception::fatal_error & e_) {
		// Standard error handling.
		HANDLE_EDP_FATAL_ERROR(e_)
	} catch (mrrocpp::lib::exception::system_error & e_) {
		// Standard error handling.
		HANDLE_EDP_SYSTEM_ERROR(e_)
	} catch (...) {
		HANDLE_EDP_UNKNOWN_ERROR()
	}
}

}
// namespace smb


namespace common {

effector* return_created_efector(common::shell &_shell)
{
	return new smb1::effector(_shell);
}

} // namespace common
} // namespace edp
} // namespace mrrocpp


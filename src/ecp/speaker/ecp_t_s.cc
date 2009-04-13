// ------------------------------------------------------------------------
//   ecp_t_tran.cc - przezroczyste wersja dla dowolnego z robotow
//
//                     EFFECTOR CONTROL PROCESS (ECP) - main()
//
// Ostatnia modyfikacja: 2006
// ------------------------------------------------------------------------


#include <stdio.h>
#include <unistd.h>
#include <map>

#include "common/typedefs.h"
#include "common/impconst.h"
#include "common/com_buf.h"

#include "lib/srlib.h"
#include "ecp/speaker/ecp_local.h"
#include "ecp/speaker/ecp_t_s.h"
#include "ecp_mp/ecp_mp_s_mic.h"

namespace mrrocpp {
namespace ecp {
namespace speaker {
namespace task {

// KONSTRUKTORY
speaking::speaking(configurator &_config) : base(_config)
{
    speak = NULL;
}

// methods for ECP template to redefine in concrete classes
void speaking::task_initialization(void)
{
    ecp_m_robot = new ecp_speaker_robot (*this);

    sensor_m[SENSOR_MIC] =
        new ecp_mp::sensor::mic(SENSOR_MIC, "[vsp_mic]", *this);

    // Konfiguracja wszystkich czujnikow
    for (std::map <SENSOR_ENUM, ::sensor*>::iterator sensor_m_iterator = sensor_m.begin();
            sensor_m_iterator != sensor_m.end(); sensor_m_iterator++)
    {
        sensor_m_iterator->second->to_vsp.parameters=1; // biasowanie czujnika
        sensor_m_iterator->second->configure_sensor();
    }

    usleep(1000*100);

    speak = new generator::speaking (*this, 8);
    speak->sensor_m = sensor_m;

    sr_ecp_msg->message("ECP loaded");
}

void speaking::main_task_algorithm(void)
{
	for(;;)
	{
		sr_ecp_msg->message("NOWA SERIA");
		sr_ecp_msg->message("Ruch");
		sr_ecp_msg->message("Zakocz - nacisnij PULSE ECP trigger");
		speak->Move();
	}
}

}
} // namespace speaker

namespace common {
namespace task {

base* return_created_ecp_task (configurator &_config)
{
	return new speaker::task::speaking(_config);
}

}
} // namespace common
} // namespace ecp
} // namespace mrrocpp


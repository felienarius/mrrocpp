#if !defined(_ECP_GEN_SPEECHRECOGNITION_H)
#define _ECP_GEN_SPEECHRECOGNITION_H

#include "ecp/common/ecp_generator.h"
#include "player/playerclient.h"

namespace mrrocpp {
namespace ecp {
namespace player {
namespace generator {

class speechrecognition : public common::generator::base
{
	private:
		PlayerClient *client;
		SpeechRecognitionProxy *device;

	public:
		// konstruktor
		speechrecognition(common::task::base& _ecp_task);
		~speechrecognition();

		virtual bool first_step ();

		virtual bool next_step ();
};

}
} // namespace player
} // namespace ecp
} // namespace mrrocpp

#endif /* _ECP_GEN_SPEECHRECOGNITION_H */

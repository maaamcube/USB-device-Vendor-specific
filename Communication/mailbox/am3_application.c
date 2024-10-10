#include "am3_application.h"
#ifdef HAS_PRESSURE
#include "vanne.h"
#endif

void cb_link_received(AM3_Buffer* rx)
{
#ifdef LINK_CHECK
	AM3_Buffer* tx = get_communication_bufferS(false);
	if(tx != 0)
	{
		uint8_t* tx_buffer = (uint8_t*) tx->buffer;
		int_to_be(ID_TX_LINK_ESTABLISHED_MSG,tx_buffer);
		append_to_send_control_fifoS(tx);
	}

#ifdef HAS_PRESSURE
	status_start();
#endif
#endif
}

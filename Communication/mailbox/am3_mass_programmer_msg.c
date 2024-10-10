#include "am3_mass_programmer_msg.h"
#include "am3_communication_generic.h"

#ifdef IS_MASS_PROGRAMMER
#include "am3_mass_programmer.h"
#endif

void cb_prg_set_address_received(AM3_Buffer*  rx)
{
#ifdef IS_MASS_PROGRAMMER
	AM3_Buffer* tx = get_communication_bufferS(false);
	if(tx != 0)
	{
		uint8_t* rx_buffer	= (uint8_t*) rx->buffer;
		uint8_t* tx_buffer 	= (uint8_t*) tx->buffer;
		uint32_t address = be_to_int(&(rx_buffer[4]));


		change_address(address);

		int_to_be(ID_TX_PRG_SET_ADDRESS_ACQ_MSG,tx_buffer);
		int_to_be(address,&(tx_buffer[4]));

		append_to_send_control_fifoS(tx);
	}
	else
	    chSysHalt("Err01");

#endif
}

void cb_prg_set_status_received(AM3_Buffer*  rx)
{
#ifdef IS_MASS_PROGRAMMER
	AM3_Buffer* tx = get_communication_bufferS(false);
	if(tx != 0)
	{
		uint8_t* rx_buffer	= (uint8_t*) rx->buffer;
		uint8_t* tx_buffer 	= (uint8_t*) tx->buffer;
		uint32_t status = be_to_int(&(rx_buffer[4]));


		change_status(status);

		int_to_be(ID_TX_PRG_SET_STATUS_ACQ_MSG,tx_buffer);
		int_to_be(status,&(tx_buffer[4]));

		append_to_send_control_fifoS(tx);
	}
    else
        chSysHalt("Err03");
#endif
}

void cb_prg_calibrate_received(AM3_Buffer*  rx)
{
#ifdef IS_MASS_PROGRAMMER
	uint8_t* rx_buffer = (uint8_t*) rx->buffer;
	calibrate();
#endif
}

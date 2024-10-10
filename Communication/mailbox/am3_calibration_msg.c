#include "am3_calibration_generic.h"
#include "am3_calibration_msg.h"
#include "am3_communication_generic.h"
#include "am3_identification_generic.h"


/**********************************************************************************************/
void cb_get_calibration_received(AM3_Buffer* rx)
{
	AM3_Buffer* tx = get_communication_bufferS(false);
	if (tx != 0)
	{
        uint8_t* rx_buffer	= (uint8_t*) rx->buffer ;
		uint8_t* tx_buffer 	= (uint8_t*) tx->buffer ;

		uint32_t packet_number = be_to_int(&(rx_buffer[4])) ;
		int_to_be(ID_TX_GET_CALIBRATION_MSG, tx_buffer) ;
		int_to_be(packet_number, &(tx_buffer[4])) ;

//        uint16_t size_loaded = load_calibration_to_buffer(packet_number, &tx_buffer[8]) ;
        (void)load_calibration_to_buffer(packet_number, &tx_buffer[8]) ;

		append_to_send_control_fifoS(tx) ;
	}
}


/**********************************************************************************************/
void cb_set_calibration_received(AM3_Buffer* rx)
{
	uint8_t* rx_buffer = (uint8_t*) rx->buffer ;
	uint32_t processed_bytes = 0 ;
	processed_bytes += sizeof(uint32_t) ; // ID bytes

	uint32_t packet_number = be_to_int(&(rx_buffer[processed_bytes])) ;
	processed_bytes += sizeof(uint32_t) ; // ID bytes

	flash_calibration(packet_number, &(rx_buffer[processed_bytes])) ;

	AM3_Buffer* tx = get_communication_bufferS(false) ;
	if (tx != 0)
	{
		uint8_t* tx_buffer = (uint8_t*) tx->buffer ;

		int_to_be(ID_TX_SET_CALIBRATION_ACQ_MSG, tx_buffer) ;
		int_to_be(packet_number, &(tx_buffer[4])) ;

		append_to_send_control_fifoS(tx) ;
	}
}

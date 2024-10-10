/**********************************************************************************************/
#include "am3_acquisition_msg.h"
#include "am3_measurement_generic.h"
#include "am3_communication_generic.h"
#include "am3_tools_generic.h"
#include "am3_temperature_generic.h"
#include "am3_flash.h"


/**********************************************************************************************/
extern void set_measurement_state(bool etat) ;


/**********************************************************************************************/
//typedef struct
//{
//    uint16_t data[2] ;
//} bias ; //AM3_Calibration2;
//
//typedef struct
//{
//    bias* b ; //am3_calibration2;
//}bias_Status ;
//bias_Status bias_read ;

extern AM3_Zip am3_zip ;
uint32_t PositionCarte ;


/**********************************************************************************************/
void cb_start_v7_received(AM3_Buffer* rx)
{
	uint8_t* rx_buffer = (uint8_t*) rx->buffer;
	uint32_t period = be_to_int(&rx_buffer[4]);

	AM3_Buffer* tx = get_communication_bufferS(false);
	if(tx != 0)
	{
		uint8_t* tx_buffer = (uint8_t*) tx->buffer;
		int_to_be(ID_TX_START_ACQ_MSG,tx_buffer);
		int_to_be((uint32_t)am3_zip.offset,&tx_buffer[4]);
		append_to_send_control_fifoS(tx);
	}
	start_measurement_V7(period);
}


/**********************************************************************************************/
void cb_stop_received(AM3_Buffer* rx)
{
	uint8_t* rx_buffer = (uint8_t*) rx->buffer ;
	uint32_t code = be_to_int(&rx_buffer[4]) ;
	uint32_t verification = be_to_int(&rx_buffer[8]) ;
	AM3_Buffer* tx = get_communication_bufferS(false) ;
	if(tx != 0)
	{
		uint8_t* tx_buffer = (uint8_t*) tx->buffer ;
		int_to_be(ID_TX_STOP_ACQ_MSG,tx_buffer) ;
		int_to_be(code,&tx_buffer[4]) ;
		int_to_be(verification,&tx_buffer[8]) ;
		append_to_send_control_fifoS(tx) ;
	}
    set_measurement_state(false) ;
}


/**********************************************************************************************/
void cb_start_v8_received(AM3_Buffer* rx)
{
    uint8_t* rx_buffer = (uint8_t*) rx->buffer ;
    uint32_t period = be_to_int(&rx_buffer[4]) ;

    PositionCarte = be_to_int(&rx_buffer[12]) ;

    AM3_Buffer* tx = get_communication_bufferS(false) ;
    if(tx != 0)
    {
        uint8_t* tx_buffer = (uint8_t*) tx->buffer ;
        int_to_be(ID_TX_MEASURE_V8_MSG,tx_buffer) ;
        int_to_be((uint32_t)am3_zip.offset,&tx_buffer[4]) ;
        append_to_send_control_fifoS(tx) ;
    }
    start_measurement_V8(period) ;
}


/**********************************************************************************************/
void cb_start_v8_zip_received(AM3_Buffer* rx)
{
    uint8_t* rx_buffer = (uint8_t*) rx->buffer ;
    uint32_t period = be_to_int(&rx_buffer[4]) ;

    PositionCarte = be_to_int(&rx_buffer[12]) ;

    if ((PositionCarte != POSITION_0) &&  (PositionCarte != POSITION_1) && (PositionCarte != POSITION_2)) return ;

    AM3_Buffer* tx = get_communication_bufferS(false) ;
    if(tx != 0)
    {
        uint8_t* tx_buffer = (uint8_t*) tx->buffer ;
        int_to_be(ID_TX_MEASURE_V8_ZIP_MSG,tx_buffer) ;
        int_to_be((uint32_t)am3_zip.offset,&tx_buffer[4]) ;
        append_to_send_control_fifoS(tx);
    }
    start_measurement_V8_ZIP(period) ;
}


/**********************************************************************************************/
void cb_get_sensor(AM3_Buffer* rx)
{
  uint32_t value[2] = {0,0};
#ifdef HAS_INTERNAL_VREF_SENSOR
  value[0] = get_temperature();
#endif

#ifdef HAS_TEMPERATURE_SENSOR
  value[1] = get_internal_voltage();
#endif

  uint8_t* rx_buffer = (uint8_t*) rx->buffer;
  AM3_Buffer* tx = get_communication_bufferS(false);
  if(tx != 0)
  {
      uint8_t* tx_buffer = (uint8_t*) tx->buffer;
      int_to_be(ID_TX_SENSOR_MEASURE_ACQ_MSG,tx_buffer);
      int_to_be(value[0],&tx_buffer[4]);
      int_to_be(value[1],&tx_buffer[8]);
      append_to_send_control_fifoS(tx);
  }
}


/**********************************************************************************************/
void cb_get_bias(AM3_Buffer* rx)
{
//    uint16_t value = 0;
//    bias_read.b =  (bias*)UNIQUE_BIAS_ADDRESS;
//    value =  bias_read.b->data[0];

     uint16_t* bias_value;
     bias_value = (uint16_t)UNIQUE_BIAS_ADDRESS;

    uint8_t* rx_buffer = (uint8_t*) rx->buffer;
    AM3_Buffer* tx = get_communication_bufferS(false);
    if(tx != 0)
    {
        uint8_t* tx_buffer = (uint8_t*) tx->buffer;
        int_to_be(ID_TX_GET_BIAS_MSG,tx_buffer);
        int_to_be(*bias_value,&tx_buffer[4]);
        append_to_send_control_fifoS(tx);
    }
}


/**********************************************************************************************/
void cb_set_bias(AM3_Buffer* rx)
{
  //uint16_t value = 0;
  uint16_t* bias_value;
  dacsample_t ValeurDAC ;

    uint8_t* rx_buffer = (uint8_t*) rx->buffer;
    *bias_value = be_to_int(&rx_buffer[4]);
    if (*bias_value > 4095) *bias_value = BIAS_DEFAULT ;
    erase_sector(UNIQUE_BIAS_ADDRESS,0x100) ;
    write_flash_word(UNIQUE_BIAS_ADDRESS,*bias_value);

    bias_value = (uint16_t)UNIQUE_BIAS_ADDRESS;
//     bias_read.b = (bias*)UNIQUE_BIAS_ADDRESS;
//     ValeurDAC = (uint16_t)bias_read.b->data[0];
    ValeurDAC = *bias_value;
     ValeurDAC &= 0x0FFF ;
     dacPutChannelX (&DACD1, 0, ValeurDAC) ;

    AM3_Buffer* tx = get_communication_bufferS(false);
    if(tx != 0)
    {
        uint8_t* tx_buffer = (uint8_t*) tx->buffer;
        int_to_be(ID_TX_SET_BIAS_MSG,tx_buffer);
        append_to_send_control_fifoS(tx);
    }
}

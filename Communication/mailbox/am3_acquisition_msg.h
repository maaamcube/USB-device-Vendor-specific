#ifndef COMMUNICATION_MAILBOX_AM3_ACQUISITION_MSG_H_
#define COMMUNICATION_MAILBOX_AM3_ACQUISITION_MSG_H_
#include "am3_msg_generic.h"

void cb_start_v7_received(AM3_Buffer* );
void cb_stop_received(AM3_Buffer* );
void cb_start_v8_received(AM3_Buffer* );
void cb_start_v8_zip_received(AM3_Buffer* );
void cb_get_sensor(AM3_Buffer* );
void cb_get_bias(AM3_Buffer* rx) ;
void cb_set_bias(AM3_Buffer* rx) ;

#define start_in_msg		{ ID_RX_START_MSG , cb_start_v7_received }
#define stop_in_msg			{ ID_RX_STOP_MSG , cb_stop_received }
#define get_sensor          { ID_RX_GET_SENSOR_MSG , cb_get_sensor }

#define start_v8            { ID_RX_START_MSG , cb_start_v8_received }
#define start_v8_zip        { ID_RX_START_MSG , cb_start_v8_zip_received }

#define get_Bias            { ID_RX_GET_BIAS_MSG  , cb_get_bias }
#define set_Bias            { ID_RX_SET_BIAS_MSG  , cb_set_bias }



#endif /* COMMUNICATION_MAILBOX_AM3_ACQUISITION_MSG_H_ */

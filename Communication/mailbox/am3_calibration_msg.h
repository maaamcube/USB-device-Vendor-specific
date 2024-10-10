#ifndef COMMUNICATION_MAILBOX_AM3_CALIBRATION_MSG_H_
#define COMMUNICATION_MAILBOX_AM3_CALIBRATION_MSG_H_
#include "am3_msg_generic.h"

void cb_get_calibration_received(AM3_Buffer* );
void cb_set_calibration_received(AM3_Buffer* );

#define get_calibration_in_msg		{ ID_RX_GET_CALIBRATION_MSG , cb_get_calibration_received }
#define set_calibration_in_msg		{ ID_RX_SET_CALIBRATION_MSG , cb_set_calibration_received }

#endif /* COMMUNICATION_MAILBOX_AM3_CALIBRATION_MSG_H_ */

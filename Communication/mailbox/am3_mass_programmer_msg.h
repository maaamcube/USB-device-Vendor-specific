#ifndef COMMUNICATION_MAILBOX_AM3_MASS_PROGRAMMER_MSG_H_
#define COMMUNICATION_MAILBOX_AM3_MASS_PROGRAMMER_MSG_H_

#include "am3_msg_generic.h"

void cb_prg_set_address_received(AM3_Buffer* );
void cb_prg_set_status_received(AM3_Buffer* );
void cb_prg_calibrate_received(AM3_Buffer* );

#define prg_set_address_in_msg		{ ID_RX_PRG_SET_ADDRESS_MSG , cb_prg_set_address_received }
#define prg_set_status_in_msg		{ ID_RX_PRG_SET_STATUS_MSG , cb_prg_set_status_received }
#define prg_calibrate_in_msg		{ ID_RX_PRG_CALIBRATE_MSG , cb_prg_calibrate_received }

#endif /* COMMUNICATION_MAILBOX_AM3_MASS_PROGRAMMER_MSG_H_ */

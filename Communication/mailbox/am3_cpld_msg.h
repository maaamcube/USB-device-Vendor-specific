#ifndef COMMUNICATION_MAILBOX_AM3_CPLD_MSG_H_
#define COMMUNICATION_MAILBOX_AM3_CPLD_MSG_H_


#include "am3_msg_generic.h"

void cb_program_cpld_received(AM3_Buffer* );
void cb_read_cpld_received(AM3_Buffer* );
void cb_erase_cpld_received(AM3_Buffer*);
void cb_start_stop_cpld_received(AM3_Buffer*);

#define program_cpld_in_msg		{ ID_RX_CPLD_WRITE_CMD_MSG , cb_program_cpld_received }
#define read_cpld_in_msg		{ ID_RX_CPLD_READ_CMD_MSG , cb_read_cpld_received }
#define erase_cpld_in_msg		{ ID_RX_CPLD_ERASE_CMD_MSG , cb_erase_cpld_received }
#define start_stop_cpld_in_msg	{ ID_RX_CPLD_START_STOP_CMD_MSG, cb_start_stop_cpld_received }

#endif /* COMMUNICATION_MAILBOX_AM3_CPLD_MSG_H_ */

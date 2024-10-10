/*
 * am3_board_operation_msg.h
 *
 *  Created on: 17 mai 2016
 *      Author: sylvain
 */

#ifndef COMMUNICATION_MAILBOX_AM3_BOARD_OPERATION_MSG_H_
#define COMMUNICATION_MAILBOX_AM3_BOARD_OPERATION_MSG_H_
#include "am3_msg_generic.h"

void cb_bootloader_start_received( AM3_Buffer* );
void cb_watchdog_reset_received( AM3_Buffer* );
void cb_write_flash( AM3_Buffer* );
void cb_erase_flash( AM3_Buffer* );
void cb_read_flash( AM3_Buffer* );

#define bootloader_start_in_msg		{ ID_RX_BOOTLOADER_START_MSG ,cb_bootloader_start_received }
#define watchdog_reset_in_msg		{ ID_RX_WATCHDOG_RESET_MSG , cb_watchdog_reset_received }
#define write_flash_in_msg			{ ID_RX_WRITE_FLASH_MSG , cb_write_flash }
#define erase_flash_in_msg			{ ID_RX_ERASE_FLASH_MSG , cb_erase_flash }
#define read_flash_in_msg			{ ID_RX_READ_FLASH_MSG , cb_read_flash }


#endif /* COMMUNICATION_MAILBOX_AM3_BOARD_OPERATION_MSG_H_ */

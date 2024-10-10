/*
 * am3_application.h
 *
 *  Created on: 13 févr. 2017
 *      Author: sylvain
 */

#ifndef COMMUNICATION_MAILBOX_AM3_APPLICATION_H_
#define COMMUNICATION_MAILBOX_AM3_APPLICATION_H_

#include "am3_msg_generic.h"

void cb_link_received(AM3_Buffer* );

#define link_in_msg		{ ID_RX_LINK_CHECK_MSG , cb_link_received }


#endif /* COMMUNICATION_MAILBOX_AM3_APPLICATION_H_ */

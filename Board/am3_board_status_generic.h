#ifndef BOARD_STATUS_GENERIC_H_
#define BOARD_STATUS_GENERIC_H_

#include "hal.h"

void am3_init_board(void);
bool is_watchdog_authorized(void);
void authorize_watchdog(bool enable);

#endif /* STATE_MACHINE_GENERIC_H_ */

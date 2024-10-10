#include "am3_board_status_generic.h"
#include "am3_identification_generic.h"
#include "am3_calibration_generic.h"
#include "am3_communication_generic.h"
#include "am3_measurement_generic.h"
#include "am3_watchdog.h"
#include "am3_powerup_generic.h"
#include "am3_cpld_flash.h"
#ifdef OWN_BOARD_STATUS
#include "am3_board_status.h"
#endif

bool authorized;

void am3_init_board(void)
{
	authorized = true;
	halInit();
	chSysInit();
	init_marker();
	am3_init_identification();

	am3_init_calibration();

	am3_init_communication();

	am3_init_measurement();

	am3_powerup_sequence();
#ifdef OWN_BOARD_STATUS
	am3_init_board_specific();
#endif

#ifndef WATCHDOG_DISABLE
	am3_set_watchdog_interval(10000);
	am3_start_watchdog();
#endif
#ifdef HAS_CPLD_RAM
	am3_program_cpld_ram();
#endif
}


bool is_watchdog_authorized(void)
{
	return authorized;
}

void authorize_watchdog(bool enable)
{
	authorized = enable;
}

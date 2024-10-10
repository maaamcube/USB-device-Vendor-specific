#include "am3_powerup_generic.h"
#include "am3_board.h"
#include "hal.h"
typedef struct {
	stm32_gpio_t * 	port;
	uint32_t		gpio;
	bool			dir;
}AM3_PowerUp_Channel;

#ifdef HAS_POWERUP_SEQUENCE
	AM3_PowerUp_Channel powerup_sequence[TOTAL_POWER_UP] = POWER_UP_INIT;
#endif

void am3_powerup_sequence(void)
{
#ifdef HAS_POWERUP_SEQUENCE
	uint8_t index_power_up;
	for(index_power_up=0 ; index_power_up<TOTAL_POWER_UP ; index_power_up++)
	{
		if(powerup_sequence[index_power_up].dir ==true)
			palClearPad(powerup_sequence[index_power_up].port,powerup_sequence[index_power_up].gpio);
		else
			palSetPad(powerup_sequence[index_power_up].port,powerup_sequence[index_power_up].gpio);

		chThdSleepMilliseconds(POWER_UP_TIME_MS);
	}
#endif
}

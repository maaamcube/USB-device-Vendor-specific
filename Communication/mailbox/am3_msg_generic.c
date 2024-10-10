/**********************************************************************************************/
#include "am3_msg_generic.h"
#include "am3_calibration_msg.h"
#include "am3_acquisition_msg.h"
#include "am3_mass_programmer_msg.h"
#include "am3_board_operation_msg.h"
#include "am3_tools_generic.h"
#include "am3_cpld_msg.h"
#include "am3_application.h"


/**********************************************************************************************/
typedef struct
{
	uint32_t id;
	void (*cb_msg_received)(AM3_Buffer*);
}	inbox_msg;


/**********************************************************************************************/
#define null_in_msg		{ ID_RX_NULL_MSG , 0 }


/**********************************************************************************************/
void unusedFn( AM3_Buffer* rx )
{
}
#define unused { 0 , unusedFn }


/**********************************************************************************************/
const inbox_msg in_box[TOTAL_RX_MSG] = {
	null_in_msg,
	get_calibration_in_msg,
	set_calibration_in_msg,
	start_in_msg,
	stop_in_msg,
	bootloader_start_in_msg,
	watchdog_reset_in_msg,
	write_flash_in_msg,
	read_flash_in_msg,
	prg_set_address_in_msg,
	prg_set_status_in_msg,
	prg_calibrate_in_msg,
	unused,
	unused,
	unused,
	unused,
	unused,
	unused,
	unused,
	program_cpld_in_msg,
	read_cpld_in_msg,
	erase_cpld_in_msg,
	erase_flash_in_msg,
	unused,
	link_in_msg,
	unused,
	start_stop_cpld_in_msg,
	get_sensor,
    get_Bias,
    set_Bias,
    start_v8,
    start_v8_zip
};


/**********************************************************************************************/
void parse_msg(AM3_Buffer* msg)
{
	uint32_t id = be_to_int((uint8_t*)msg->buffer) ;

	if( ( (id & 0xFFFF0000) == 0x32ab0000) && ((id & 0x000000FF) < TOTAL_RX_MSG))
	{
		in_box[(id & 0x000000FF)].cb_msg_received(msg) ;
	}
	else
	{
//		chSysHalt("Err04") ;
	}

}

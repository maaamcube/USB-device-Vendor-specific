
#ifndef AM3_MSG_H_
#define AM3_MSG_H_

#include "hal.h"
#include "am3_board_generic.h"
#include "am3_tools_generic.h"

#define ID_RX_CHECK							(uint32_t) 0x32AB0000
#define ID_RX_NULL_MSG						(uint32_t) (0x00000000 | ID_RX_CHECK)
#define ID_RX_GET_CALIBRATION_MSG			(uint32_t) (0x00000001 | ID_RX_CHECK)
#define ID_RX_SET_CALIBRATION_MSG			(uint32_t) (0x00000002 | ID_RX_CHECK)
#define ID_RX_START_MSG						(uint32_t) (0x00000003 | ID_RX_CHECK)
#define ID_RX_STOP_MSG						(uint32_t) (0x00000004 | ID_RX_CHECK)
#define ID_RX_BOOTLOADER_START_MSG			(uint32_t) (0x00000005 | ID_RX_CHECK)
#define ID_RX_WATCHDOG_RESET_MSG			(uint32_t) (0x00000006 | ID_RX_CHECK)
#define ID_RX_WRITE_FLASH_MSG				(uint32_t) (0x00000007 | ID_RX_CHECK)
#define ID_RX_READ_FLASH_MSG				(uint32_t) (0x00000008 | ID_RX_CHECK)
#define ID_RX_PRG_SET_ADDRESS_MSG			(uint32_t) (0x00000009 | ID_RX_CHECK)
#define ID_RX_PRG_SET_STATUS_MSG			(uint32_t) (0x0000000A | ID_RX_CHECK)
#define ID_RX_PRG_CALIBRATE_MSG				(uint32_t) (0x0000000B | ID_RX_CHECK)
#define ID_RX_STP_MOVE_MSG					(uint32_t) (0x0000000C | ID_RX_CHECK)
#define ID_RX_STP_INTERRUPT_MSG				(uint32_t) (0x0000000D | ID_RX_CHECK)
#define ID_RX_STP_ORIGIN_MSG				(uint32_t) (0x0000000E | ID_RX_CHECK)
#define ID_RX_PRESSURE_CMD_MSG				(uint32_t) (0x0000000F | ID_RX_CHECK)
#define ID_RX_PRESSURE_INITIALIZE_MSG		(uint32_t) (0x00000010 | ID_RX_CHECK)
#define ID_RX_PRESSURE_RESET_STATE_MSG		(uint32_t) (0x00000011 | ID_RX_CHECK)
#define ID_RX_PRESSURE_VANNE_ACTION_CMD_MSG	(uint32_t) (0x00000012 | ID_RX_CHECK)
#define ID_RX_CPLD_WRITE_CMD_MSG			(uint32_t) (0x00000013 | ID_RX_CHECK)
#define ID_RX_CPLD_READ_CMD_MSG				(uint32_t) (0x00000014 | ID_RX_CHECK)
#define ID_RX_CPLD_ERASE_CMD_MSG			(uint32_t) (0x00000015 | ID_RX_CHECK)
#define ID_RX_ERASE_FLASH_MSG				(uint32_t) (0x00000016 | ID_RX_CHECK)
#define ID_RX_STP_UNTIL_PRESENCE_MSG		(uint32_t) (0x00000017 | ID_RX_CHECK)
#define ID_RX_LINK_CHECK_MSG				(uint32_t) (0x00000018 | ID_RX_CHECK)
#define ID_RX_STP_CONFIG_MSG				(uint32_t) (0x00000019 | ID_RX_CHECK)
#define ID_RX_CPLD_START_STOP_CMD_MSG		(uint32_t) (0x0000001A | ID_RX_CHECK)
#define ID_RX_GET_SENSOR_MSG                (uint32_t) (0x0000001B | ID_RX_CHECK)
#define ID_RX_GET_BIAS_MSG                  (uint32_t) (0x0000001C | ID_RX_CHECK)
#define ID_RX_SET_BIAS_MSG                  (uint32_t) (0x0000001D | ID_RX_CHECK)
#define ID_RX_DEVICE_START_V8               (uint32_t) (0x0000001E | ID_RX_CHECK)
#define ID_RX_DEVICE_START_V8_ZIP           (uint32_t) (0x0000001F | ID_RX_CHECK)


#define ID_TX_CHECK							(uint32_t) 0xCD410000
#define ID_TX_NULL_MSG						(uint32_t) (0x00000000 | ID_TX_CHECK)
#define ID_TX_GET_CALIBRATION_MSG			(uint32_t) (0x00000001 | ID_TX_CHECK)
#define ID_TX_SET_CALIBRATION_ACQ_MSG		(uint32_t) (0x00000002 | ID_TX_CHECK)
#define ID_TX_START_ACQ_MSG					(uint32_t) (0x00000003 | ID_TX_CHECK)
#define ID_TX_MEASURE_MSG					(uint32_t) (0x00000004 | ID_TX_CHECK)
#define ID_TX_STOP_ACQ_MSG					(uint32_t) (0x00000005 | ID_TX_CHECK)
#define ID_TX_BOOTLOADER_START_ACQ_MSG		(uint32_t) (0x00000006 | ID_TX_CHECK)
#define ID_TX_WATCHDOG_RESET_ACQ_MSG		(uint32_t) (0x00000007 | ID_TX_CHECK)
#define ID_TX_WRITE_FLASH_ACQ_MSG			(uint32_t) (0x00000008 | ID_TX_CHECK)
#define ID_TX_READ_FLASH_ACQ_MSG			(uint32_t) (0x00000009 | ID_TX_CHECK)
#define ID_TX_PRG_SET_ADDRESS_ACQ_MSG		(uint32_t) (0x0000000A | ID_TX_CHECK)
#define ID_TX_PRG_SET_STATUS_ACQ_MSG		(uint32_t) (0x0000000B | ID_TX_CHECK)
#define ID_TX_PRG_CURRENT_MSG				(uint32_t) (0x0000000C | ID_TX_CHECK)
#define ID_TX_PRG_CALIBRATE_MSG				(uint32_t) (0x0000000D | ID_TX_CHECK)
#define ID_TX_STP_MOVE_ACQ_MSG				(uint32_t) (0x0000000E | ID_TX_CHECK)
#define ID_TX_STP_INTERRUPT_ACQ_MSG			(uint32_t) (0x0000000F | ID_TX_CHECK)
#define ID_TX_STP_ORIGIN_ACQ_MSG			(uint32_t) (0x00000010 | ID_TX_CHECK)
#define ID_TX_PRESSURE_MSG					(uint32_t) (0x00000011 | ID_TX_CHECK)
#define ID_TX_PRESSURE_REACHED_MSG			(uint32_t) (0x00000012 | ID_TX_CHECK)
#define ID_TX_PRESSURE_STABILIZED_MSG		(uint32_t) (0x00000013 | ID_TX_CHECK)
#define ID_TX_PRESSURE_CMD_ACQ_MSG			(uint32_t) (0x00000014 | ID_TX_CHECK)
#define ID_TX_CPLD_WRITE_ACQ_MSG			(uint32_t) (0x00000015 | ID_TX_CHECK)
#define ID_TX_CPLD_READ_CHUNK_MSG			(uint32_t) (0x00000016 | ID_TX_CHECK)
#define ID_TX_CPLD_ERASE_ACQ_MSG			(uint32_t) (0x00000017 | ID_TX_CHECK)
#define ID_TX_ERASE_FLASH_ACQ_MSG			(uint32_t) (0x00000018 | ID_TX_CHECK)
#define ID_TX_LINK_ESTABLISHED_MSG			(uint32_t) (0x00000019 | ID_TX_CHECK)
#define ID_TX_PRESSURE_RESET_ACQ_MSG		(uint32_t) (0x0000001A | ID_TX_CHECK)
#define ID_TX_STP_CONFIG_ACQ_MSG			(uint32_t) (0x0000001B | ID_TX_CHECK)
#define ID_TX_CPLD_START_STOP_ACQ_MSG		(uint32_t) (0x0000001C | ID_TX_CHECK)
#define ID_TX_SENSOR_MEASURE_ACQ_MSG        (uint32_t) (0x0000001D | ID_TX_CHECK)
#define ID_TX_GET_BIAS_MSG                  (uint32_t) (0x0000001E | ID_TX_CHECK)
#define ID_TX_SET_BIAS_MSG                  (uint32_t) (0x0000001F | ID_TX_CHECK)
#define ID_TX_DEVICE_START_V8               (uint32_t) (0x00000020 | ID_TX_CHECK)
#define ID_TX_DEVICE_START_V8_ZIP           (uint32_t) (0x00000021 | ID_TX_CHECK)
#define ID_TX_MEASURE_V8_MSG                (uint32_t) (0x00000022 | ID_TX_CHECK)
#define ID_TX_MEASURE_V8_ZIP_MSG            (uint32_t) (0x00000023 | ID_TX_CHECK)


#define ID_INTERNAL_CHECK				    (uint32_t) 0xF4120000
#define ID_INTERNAL_NULL_MSG			    (uint32_t) (0x00000000 | ID_INTERNAL_CHECK)
#define ID_INTERNAL_ZIP_MSG				    (uint32_t) (0x00000001 | ID_INTERNAL_CHECK)

#define TOTAL_RX_MSG					    32

void parse_msg(AM3_Buffer* msg);

#endif /* AM3_MSG_H_ */

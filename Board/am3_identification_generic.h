/*
 * identification.h
 *
 *  Created on: 24 juin 2015
 *      Author: ElecPortable
 */

#ifndef AM3_IDENTIFICATION_H_
#define AM3_IDENTIFICATION_H_

#include "hal.h"

#define SERIAL_SIZE								17
#define PRODUCT_SIZE							8
#define HARDWARE_SIZE							8
#define FIRMWARE_MICRO_SIZE						8
#define FIRMWARE_CPLD_SIZE						8
#define FIRMWARE_BOOTLOADER_SIZE				8
#define FIRMWARE_WIFI_SIZE						8


typedef struct{
	uint32_t 	vid;
	uint32_t 	pid;

	uint8_t 	board_serial_number[SERIAL_SIZE];
	uint8_t 	product_identification_string[PRODUCT_SIZE];
	uint8_t 	hardware_version[HARDWARE_SIZE];
	uint8_t 	microcontroller_firmware_version[FIRMWARE_MICRO_SIZE];
	uint8_t 	cpld_firmware_version[FIRMWARE_CPLD_SIZE];
	uint8_t 	bootloader_firmware_version[FIRMWARE_BOOTLOADER_SIZE];
	uint8_t 	wifi_firmware_version[FIRMWARE_WIFI_SIZE];

	uint16_t 	unicode_board_serial_number[SERIAL_SIZE+1];
	uint16_t 	unicode_product_identification_string[PRODUCT_SIZE+1];
	uint16_t	unicode_hardware_version[HARDWARE_SIZE+1];
	uint16_t	unicode_micro_version[FIRMWARE_MICRO_SIZE+1];
	uint16_t	unicode_cpld_version[FIRMWARE_CPLD_SIZE+1];
	uint16_t	unicode_bootloader_version[FIRMWARE_BOOTLOADER_SIZE+1];
	uint16_t	unicode_wifi_version[FIRMWARE_WIFI_SIZE+1];

} AM3_Product;

extern AM3_Product am3_product_properties;

void am3_init_identification(void);



#endif /* AM3_IDENTIFICATION_H_ */

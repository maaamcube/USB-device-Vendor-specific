#include "am3_identification_generic.h"

#include "../Tools/am3_tools_generic.h"
#include "am3_board_generic.h"

#define MICRO_VERSION_INDEX		0
#define WIFI_VERSION_INDEX		1

const uint8_t product_string[PRODUCT_SIZE] = PRODUCT_STRING;


#ifdef IS_BOOTLOADER
__attribute__((section(".bootloader"),aligned(0x0800))) static const uint32_t bootloader_version = BOOTLOADER_VERSION;
uint32_t* identification_page = FIRMWARE_VERSION_ADDRESS;
#else
__attribute__((section(".bootloader"),aligned(0x0800))) static const uint32_t bootloader_version;
__attribute__((section(".version"),aligned(0x0800))) static const uint32_t identification_page[2] = {MICROCONTROLLER_FIRMWARE_VERSION,WIFI_FIRMWARE_VERSION };
#endif
uint32_t* add ;
AM3_Product am3_product_properties;

void load_unique_id_unicode( void );
void load_unique_id(uint32_t* unique_id_address);

void load_cpld_version(void);
void load_cpld_string_unicode(void);

void load_hardware_version(void);
void load_hardware_string_unicode(void);

void load_microcontroller_version(void);
void load_microcontroller_string_unicode(void);

void load_wifi_version(void);
void load_wifi_string_unicode(void);

void load_bootloader_version(void);
void load_bootloader_string_unicode(void);

void load_product_specific(void);
void load_product_string_unicode(void);

void am3_init_identification(void)
{
	load_unique_id(UNIQUE_ID_ADDRESS);

	load_cpld_version();
	load_hardware_version();
	load_microcontroller_version();
	load_wifi_version();

	load_bootloader_version();
	load_product_specific();
	add = &bootloader_version;
}

void load_unique_id_unicode( void )
{
#ifdef HAS_USB_COMMUNICATION
	am3_product_properties.unicode_board_serial_number[0] = USB_DESC_BYTE(USB_DESCRIPTOR_STRING) << 8 |sizeof(am3_product_properties.unicode_board_serial_number);
#else
	am3_product_properties.unicode_board_serial_number[0] = sizeof(am3_product_properties.unicode_board_serial_number);;
#endif

	uint8_t index = 0;
	for( index=0 ; index<SERIAL_SIZE ;index++)
		am3_product_properties.unicode_board_serial_number[index+1] = am3_product_properties.board_serial_number[index];
}

void load_unique_id(uint32_t* unique_id_address)
{
	uint32_t *x_y_coordinate,*wafer_num, *wafer_lot;
	x_y_coordinate =  unique_id_address;
	wafer_num = unique_id_address +1;
	wafer_lot = unique_id_address +2;

	/* Lot number [23:0] (ASCII encoded) 	*/
	am3_product_properties.board_serial_number[0] = ( (*wafer_num) >> 24 );
	am3_product_properties.board_serial_number[1] = ( (*wafer_num) >> 16 );
	am3_product_properties.board_serial_number[2] = ( (*wafer_num) >> 8 );

	/* Lot number [55:24] (ASCII encoded) 	*/
	int_to_be( (*wafer_lot), &am3_product_properties.board_serial_number[3]);

	am3_product_properties.board_serial_number[3] = 'X';

	/* X, Y coordinates (BCD encoded) 		*/
	am3_product_properties.board_serial_number[7] = to_char( ((*x_y_coordinate) >> 28) &0x000F);
	am3_product_properties.board_serial_number[8] = to_char( ((*x_y_coordinate) >> 24) &0x000F);
	am3_product_properties.board_serial_number[9] = to_char( ((*x_y_coordinate) >> 12) &0x000F);
	am3_product_properties.board_serial_number[10] = to_char( ((*x_y_coordinate) >> 8) &0x000F);
	am3_product_properties.board_serial_number[11] = to_char( ((*x_y_coordinate) >> 20) &0x000F);
	am3_product_properties.board_serial_number[12] = to_char( ((*x_y_coordinate) >> 16) &0x000F);
	am3_product_properties.board_serial_number[13] = to_char( ((*x_y_coordinate) >> 4) &0x000F);
	am3_product_properties.board_serial_number[14] = to_char( ((*x_y_coordinate)) &0x000F);

	/* Wafer number unsigned int 8 bit 		*/
	am3_product_properties.board_serial_number[15] = to_char( ((*wafer_num) >> 4) &0x000F);
	am3_product_properties.board_serial_number[16] = to_char( ((*wafer_num)) &0x000F);

	load_unique_id_unicode();
}


void load_cpld_version(void)
{
	uint8_t version = PAL_CPLD_VERSION;
	int_to_char(version,am3_product_properties.cpld_firmware_version);
	load_cpld_string_unicode();
}

void load_hardware_version(void)
{
	uint32_t version = PAL_HARDWARE_VERSION;
	int_to_char(version,am3_product_properties.hardware_version);
	load_hardware_string_unicode();

}

void load_microcontroller_version(void)
{
	uint32_t* add = identification_page;
	int_to_char(identification_page[MICRO_VERSION_INDEX],am3_product_properties.microcontroller_firmware_version);
	load_microcontroller_string_unicode();
}

void load_wifi_version(void)
{
	int_to_char(identification_page[WIFI_VERSION_INDEX],am3_product_properties.wifi_firmware_version);
	load_wifi_string_unicode();
}


void load_bootloader_version(void)
{
	int_to_char(bootloader_version,am3_product_properties.bootloader_firmware_version);

	load_bootloader_string_unicode();
}

void load_product_specific(void)
{
	am3_product_properties.vid = PRODUCT_VID ;
	am3_product_properties.pid = PRODUCT_PID ;

	uint8_t index;
	for(index = 0 ; index < PRODUCT_SIZE ; index++)
	{
		am3_product_properties.product_identification_string[index] = product_string[index] ;
	}
	load_product_string_unicode() ;
}

void load_bootloader_string_unicode(void)
{

#ifdef HAS_USB_COMMUNICATION
	am3_product_properties.unicode_bootloader_version[0] = USB_DESC_BYTE(USB_DESCRIPTOR_STRING) << 8 | sizeof(am3_product_properties.unicode_bootloader_version);
#else
	am3_product_properties.unicode_bootloader_version[0] = sizeof(am3_product_properties.unicode_bootloader_version);
#endif

	uint8_t index = 0;
	for( index=0 ; index<FIRMWARE_BOOTLOADER_SIZE ;index++)
		am3_product_properties.unicode_bootloader_version[index+1] = am3_product_properties.bootloader_firmware_version[index];

}
void load_wifi_string_unicode(void)
{

#ifdef HAS_USB_COMMUNICATION
	am3_product_properties.unicode_wifi_version[0] = USB_DESC_BYTE(USB_DESCRIPTOR_STRING) << 8 | sizeof(am3_product_properties.unicode_wifi_version);
#else
	am3_product_properties.unicode_wifi_version[0] = sizeof(am3_product_properties.unicode_wifi_version);
#endif

	uint8_t index = 0;
	for( index=0 ; index<FIRMWARE_WIFI_SIZE ;index++)
		am3_product_properties.unicode_wifi_version[index+1] = am3_product_properties.wifi_firmware_version[index];


}

void load_cpld_string_unicode(void)
{

#ifdef HAS_USB_COMMUNICATION
	am3_product_properties.unicode_cpld_version[0] = USB_DESC_BYTE(USB_DESCRIPTOR_STRING) << 8 | sizeof(am3_product_properties.unicode_cpld_version);
#else
	am3_product_properties.unicode_cpld_version[0] = sizeof(am3_product_properties.unicode_cpld_version);
#endif

	uint8_t index = 0;
	for( index=0 ; index<FIRMWARE_CPLD_SIZE ;index++)
		am3_product_properties.unicode_cpld_version[index+1] = am3_product_properties.cpld_firmware_version[index];


}
void load_hardware_string_unicode(void)
{
#ifdef HAS_USB_COMMUNICATION
	am3_product_properties.unicode_hardware_version[0] = USB_DESC_BYTE(USB_DESCRIPTOR_STRING) << 8 | sizeof(am3_product_properties.unicode_hardware_version);
#else
	am3_product_properties.unicode_hardware_version[0] = sizeof(am3_product_properties.unicode_hardware_version);
#endif

	uint8_t index = 0;
	for( index=0 ; index<HARDWARE_SIZE ;index++)
		am3_product_properties.unicode_hardware_version[index+1] = am3_product_properties.hardware_version[index];

}
void load_microcontroller_string_unicode(void)
{
#ifdef HAS_USB_COMMUNICATION
	am3_product_properties.unicode_micro_version[0] = USB_DESC_BYTE(USB_DESCRIPTOR_STRING) << 8 | sizeof(am3_product_properties.unicode_micro_version);
#else
	am3_product_properties.unicode_micro_version[0] = sizeof(am3_product_properties.unicode_micro_version);
#endif

	uint8_t index = 0;
	for( index=0 ; index<FIRMWARE_MICRO_SIZE ;index++)
		am3_product_properties.unicode_micro_version[index+1] = am3_product_properties.microcontroller_firmware_version[index];

}

void load_product_string_unicode(void)
{
#ifdef HAS_USB_COMMUNICATION
	am3_product_properties.unicode_product_identification_string[0] = USB_DESC_BYTE(USB_DESCRIPTOR_STRING) << 8 | sizeof(am3_product_properties.unicode_product_identification_string);
#else
	am3_product_properties.unicode_product_identification_string[0] = sizeof(am3_product_properties.unicode_product_identification_string);
#endif

	uint8_t index = 0;
	for( index=0 ; index<PRODUCT_SIZE ;index++)
		am3_product_properties.unicode_product_identification_string[index+1] = am3_product_properties.product_identification_string[index];

}

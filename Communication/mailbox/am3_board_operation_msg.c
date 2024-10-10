#include "am3_board_operation_msg.h"
#include "am3_communication_generic.h"
#include "am3_flash.h"
#include "am3_watchdog.h"

void cb_bootloader_start_received( AM3_Buffer* rx )
{

#ifdef IS_BOOTLOADER

	uint32_t address = FIRMWARE_ADDRESS;
	am3_reset_watchdog_counter();
	jump_to(address);
#endif
}

void cb_watchdog_reset_received( AM3_Buffer* rx )
{
	/*am3_reset_watchdog_counter();
	uint8_t* rx_buffer = (uint8_t*) rx->buffer;
	am3_set_watchdog(be_to_int(&(rx_buffer[4])));*/
}

void cb_write_flash( AM3_Buffer* rx )
{
#ifdef IS_BOOTLOADER
	uint8_t* rx_buffer = (uint8_t*) rx->buffer;
	uint32_t address = FIRMWARE_ADDRESS + be_to_int(&(rx_buffer[4]));
	uint32_t size = be_to_int(&(rx_buffer[8]));
	uint32_t module = be_to_int(&(rx_buffer[12]));

	if(address >= FIRMWARE_ADDRESS && (address+size) < (FIRMWARE_ADDRESS + BOOTLOADER_FLASH_SIZE) && size > 0)
	{

		am3_reset_watchdog_counter();
		write_flash(address,(uint32_t*)&(rx_buffer[16]),size/4);
	}

	AM3_Buffer* tx = get_communication_bufferS(false);
	if(tx != 0)
	{
		uint8_t* tx_buffer = (uint8_t*) tx->buffer;
		int_to_be(ID_TX_WRITE_FLASH_ACQ_MSG,tx_buffer);
		int_to_be(address,&(tx_buffer[4]));
		int_to_be(size,&(tx_buffer[8]));
		int_to_be(module,&(tx_buffer[12]));
		append_to_send_control_fifoS(tx);
	}
#endif
}

void cb_read_flash( AM3_Buffer* rx )
{
#ifdef IS_BOOTLOADER
	uint8_t* rx_buffer = (uint8_t*) rx->buffer;
	uint32_t address = FIRMWARE_ADDRESS + be_to_int(&(rx_buffer[4]));
	uint32_t size = be_to_int(&(rx_buffer[8]));
	uint32_t module = be_to_int(&(rx_buffer[12]));
	uint8_t* data = (uint8_t*)address;

	AM3_Buffer* tx = get_communication_bufferS(false);
	if(tx != 0)
	{
		uint8_t* tx_buffer 	= (uint8_t*) tx->buffer;

			if(address >= FIRMWARE_ADDRESS && (address+size) < (FIRMWARE_ADDRESS + BOOTLOADER_FLASH_SIZE) && size > 0)
			{
				am3_reset_watchdog_counter();
				int_to_be(ID_TX_READ_FLASH_ACQ_MSG,tx_buffer);
				int_to_be(address,&(tx_buffer[4]));
				int_to_be(size,&(tx_buffer[8]));
				int_to_be(module,&(tx_buffer[12]));

				uint32_t index=0;
				for(index=0 ; index < size ; index++)
					tx_buffer[16+index] = data[index];

				append_to_send_control_fifoS(tx);
		}
	}
#endif

}


void cb_erase_flash( AM3_Buffer* rx )
{
#ifdef IS_BOOTLOADER
	uint8_t* rx_buffer = (uint8_t*) rx->buffer;
	uint32_t address = be_to_int(&rx_buffer[4]);
	uint32_t module = be_to_int(&rx_buffer[8]);

	if(address >= FIRMWARE_ADDRESS && (address) < (FIRMWARE_ADDRESS + BOOTLOADER_FLASH_SIZE))
	{

			am3_reset_watchdog_counter();
			erase_sector(address,1);
	}

	AM3_Buffer* tx = get_communication_bufferS(false);
	if(tx != 0)
	{
		uint8_t* tx_buffer = (uint8_t*) tx->buffer;
		int_to_be(ID_TX_ERASE_FLASH_ACQ_MSG,tx_buffer);
		int_to_be(address,&(tx_buffer[4]));
		append_to_send_control_fifoS(tx);
	}
#endif
}


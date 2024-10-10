#include "am3_cpld_msg.h"
#include "am3_cpld_flash.h"
#include "am3_communication_generic.h"
#include "am3_tools_generic.h"
void cb_program_cpld_received(AM3_Buffer* rx)
{
#ifdef HAS_CPLD_FLASH

	AM3_Buffer* tx = get_communication_bufferS(false);
	if(tx != 0)
	{
		uint8_t* rx_buffer	= (uint8_t*) rx->buffer;
		uint8_t* tx_buffer 	= (uint8_t*) tx->buffer;

		uint32_t num = be_to_int(&(rx_buffer[4]));
		uint32_t address = be_to_int(&(rx_buffer[8]));
		uint32_t size = be_to_int(&(rx_buffer[12]));
		uint32_t module = be_to_int(&(rx_buffer[16]));

	      if(num == 0){
	        uint32_t size_total = be_to_int(&rx_buffer[20]);
	        am3_erase_cpld_flash(address,module,size_total);
	      }
		am3_program_cpld_flash(address,module,size,&rx_buffer[16]);

		int_to_be(ID_TX_CPLD_WRITE_ACQ_MSG,tx_buffer);
		int_to_be(address,&(tx_buffer[4]));
		int_to_be(size,&(tx_buffer[8]));

		append_to_send_control_fifoS(tx);
	}
	else
	  set_marker(0xFF);
#endif

#ifdef HAS_CPLD_RAM
	AM3_Buffer* tx = get_communication_bufferS(false);
    if(tx != 0)
    {
        uint8_t* rx_buffer  = (uint8_t*) rx->buffer;
        uint8_t* tx_buffer  = (uint8_t*) tx->buffer;

        uint32_t address = BOOTLOADER_FLASH_ADDRESS + be_to_int(&(rx_buffer[4]));
        uint32_t packet = be_to_int(&(rx_buffer[8]));
        uint32_t size = be_to_int(&(rx_buffer[16]));
        if(address == BOOTLOADER_FLASH_ADDRESS){
          int_to_be(0xA5A5A5A5,&rx_buffer[12]);
          am3_program_cpld_flash(address,(packet+8)/4,&rx_buffer[12]);
        }
        else
        {
          am3_program_cpld_flash(address+8,(packet/4)-1,&rx_buffer[20]);
        }

        int_to_be(ID_TX_CPLD_WRITE_ACQ_MSG,tx_buffer);
        int_to_be(address,&(tx_buffer[4]));
        int_to_be(size,&(tx_buffer[8]));

        append_to_send_control_fifoS(tx);
    }
#endif
}


void cb_read_cpld_received(AM3_Buffer* rx)
{
#ifdef HAS_CPLD_FLASH
	AM3_Buffer* tx = get_communication_bufferS(false);
		if(tx != 0)
		{
			uint8_t* rx_buffer	= (uint8_t*) rx->buffer;
			uint8_t* tx_buffer 	= (uint8_t*) tx->buffer;

			uint32_t address = be_to_int(&(rx_buffer[8]));
			uint32_t size = be_to_int(&(rx_buffer[12]));
			uint32_t module = be_to_int(&(rx_buffer[16]));

			am3_read_cpld_flash(address,module,size,&tx_buffer[16]);

			int_to_be(ID_TX_CPLD_READ_CHUNK_MSG,tx_buffer);
			int_to_be(address,&(tx_buffer[8]));
			int_to_be(size,&(tx_buffer[12]));
			int_to_be(module,&(tx_buffer[16]));

			append_to_send_control_fifoS(tx);
		}
		//palClearPad(GPIOG,GPIOG_FMC_A13);
#endif

#ifdef HAS_CPLD_RAM
    AM3_Buffer* tx = get_communication_bufferS(false);
        if(tx != 0)
        {
            uint8_t* rx_buffer  = (uint8_t*) rx->buffer;
            uint8_t* tx_buffer  = (uint8_t*) tx->buffer;

            uint32_t address = BOOTLOADER_FLASH_ADDRESS + be_to_int(&(rx_buffer[4]));
            uint32_t size = be_to_int(&(rx_buffer[8]));

            if(address == BOOTLOADER_FLASH_ADDRESS)
              am3_read_cpld_flash(address+8,size+8,&tx_buffer[16]);
            else
              am3_read_cpld_flash(address+8,size+8,&tx_buffer[16]);

            uint8_t* size2 = BOOTLOADER_FLASH_ADDRESS+4;
            uint8_t i=0;
            int_to_be(ID_TX_CPLD_READ_CHUNK_MSG,tx_buffer);
            int_to_be(address,&(tx_buffer[4]));
            int_to_be(size,&(tx_buffer[8]));

            append_to_send_control_fifoS(tx);
        }
        //palClearPad(GPIOG,GPIOG_FMC_A13);
#endif
}


void cb_erase_cpld_received(AM3_Buffer* rx)
{
#ifdef HAS_CPLD_FLASH
	AM3_Buffer* tx = get_communication_bufferS(false);
	if(tx != 0)
	{
		uint8_t* tx_buffer 	= (uint8_t*) tx->buffer;
		uint8_t* rx_buffer	= (uint8_t*) rx->buffer;
		uint32_t address = be_to_int(&(rx_buffer[4]));
		uint32_t module = be_to_int(&(rx_buffer[8]));
		am3_erase_cpld_flash(address,module,0);

		int_to_be(ID_TX_CPLD_ERASE_ACQ_MSG,tx_buffer);
		append_to_send_control_fifoS(tx);
	}
#endif

#ifdef HAS_CPLD_RAM
    AM3_Buffer* tx = get_communication_bufferS(false);
    if(tx != 0)
    {
        uint8_t* tx_buffer  = (uint8_t*) tx->buffer;
        uint8_t* rx_buffer  = (uint8_t*) rx->buffer;
        uint32_t address = be_to_int(&(rx_buffer[4]));
        am3_erase_cpld_flash(address);
        int_to_be(ID_TX_CPLD_ERASE_ACQ_MSG,tx_buffer);
        append_to_send_control_fifoS(tx);
    }
#endif
}


void cb_start_stop_cpld_received(AM3_Buffer* rx)
{
#ifdef HAS_CPLD_FLASH

#endif
}

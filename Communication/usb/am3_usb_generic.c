/**********************************************************************************************/
#include "am3_usb_generic.h"
#include "am3_board_generic.h"
#include "am3_descriptor_generic.h"
#include "am3_communication_generic.h"


/**********************************************************************************************/
#ifdef HAS_USB_COMMUNICATION

AM3_USB_Driver am3_usb_driver;
const uint8_t endpoint_port[USB_TX_ENDPOINTS_NUMBER] = USB_ENDPOINT_PORTS;


/**********************************************************************************************/
extern void append_to_send_measure_fifo(AM3_Buffer* to_send) ;
extern void check_for_new_transmissionI(AM3_TX_Channel* channel) ;


/**********************************************************************************************/
void am3_usb_tx_clear(uint8_t tx_endpoint)
{

	uint8_t endpoint_index = 0;
	for(endpoint_index = 0 ; endpoint_index < USB_TX_ENDPOINTS_NUMBER ; endpoint_index ++)
	{
		if(endpoint_port[endpoint_index] == tx_endpoint)
		{
#ifdef HAS_TX_MEASURE_CHANNEL
		    if(am3_usb_driver.tx_endpoint[endpoint_index].tx_channel->transmitting != 0)
		      append_to_send_measure_fifo(am3_usb_driver.tx_endpoint[endpoint_index].tx_channel->transmitting);
#endif
			am3_usb_driver.tx_endpoint[endpoint_index].tx_channel->transmitting = 0;
			usbInitEndpointI(am3_usb_driver.usb_driver, endpoint_port[endpoint_index], &am3_usb_driver.tx_endpoint[endpoint_index].in_endpoint_config);
			check_for_new_transmissionI(am3_usb_driver.tx_endpoint[endpoint_index].tx_channel);
		}
	}
}


/**********************************************************************************************/
void am3_usb_communication_ready(bool ready)
{
	am3_usb_driver.communication_ready(ready);
}


/**********************************************************************************************/
AM3_Buffer* am3_get_forced_buffer(void)
{
	return am3_usb_driver.usb_buffer;
}


/**********************************************************************************************/
void am3_usb_configure_endpoints(void)
{
	chSysLockFromISR();

	uint8_t endpoint_index = 0;
	for(endpoint_index = 0 ; endpoint_index < USB_TX_ENDPOINTS_NUMBER ; endpoint_index ++)
		usbInitEndpointI(am3_usb_driver.usb_driver, endpoint_port[endpoint_index], &am3_usb_driver.tx_endpoint[endpoint_index].in_endpoint_config);
	usbInitEndpointI(am3_usb_driver.usb_driver, RX_USB_ENDPOINT_CONTROL, &am3_usb_driver.rx_endpoint.out_endpoint_config);

	am3_usb_driver.usb_configured = true;

	chSysUnlockFromISR();
	am3_usb_driver.rx_endpoint.rx_channel->prepare_receive(am3_usb_driver.rx_endpoint.rx_channel);
}


/**********************************************************************************************/
void prepare_receive(void* rx_chnl)
{
	if(am3_usb_driver.usb_configured)
	{
		AM3_RX_Channel* rx_channel = (AM3_RX_Channel*) rx_chnl;
		AM3_Buffer* rx = get_communication_bufferI(true);
		if(rx != 0)
		{
			uint8_t* rx_buffer = (uint8_t*) rx->buffer;
			chSysLockFromISR();
            usbStartReceiveI(am3_usb_driver.usb_driver,RX_USB_ENDPOINT_CONTROL,rx_buffer,USB_BUFFER_SIZE);
			chSysUnlockFromISR();
			rx_channel->receiving = rx;
		}
		else
		{
			rx_channel = (AM3_RX_Channel*) rx_chnl;
		}
	}
}


/**********************************************************************************************/
void send(void* tx_channel)
{
	AM3_TX_Channel* channel = (AM3_TX_Channel*)tx_channel;
	uint8_t* tx_buffer = channel->transmitting->buffer;
	if(am3_usb_driver.usb_configured == true)
	{
		uint8_t index_endpoint = 0;
		for(index_endpoint=0 ; index_endpoint<USB_TX_ENDPOINTS_NUMBER ; index_endpoint++)
		{
			if(channel == am3_usb_driver.tx_endpoint[index_endpoint].tx_channel)
			{
#ifdef AVEC_TIME
			  add_time_measure(0x65+index_endpoint);
#endif
				usbStartTransmitI(am3_usb_driver.usb_driver,endpoint_port[index_endpoint],tx_buffer, channel->transmitting->bytes);
				break;
			}
		}
	}
	else
	{
		channel->transmitted(channel);
	}
}


/**********************************************************************************************/
void usb_data_received(USBDriver *usbp, usbep_t ep)
{
	am3_usb_driver.rx_endpoint.rx_channel->received(am3_usb_driver.rx_endpoint.rx_channel);
}


/**********************************************************************************************/
void usb_data_transmitted(USBDriver *usbp, usbep_t ep)
{
	int i=0;
	for(i=0;i<USB_TX_ENDPOINTS_NUMBER;i++)
	{
		if(endpoint_port[i] == ep)
		{
			am3_usb_driver.tx_endpoint[i].tx_channel->transmitted(am3_usb_driver.tx_endpoint[i].tx_channel);
			return;
		}
	}
}


/**********************************************************************************************/
void init_rx_endpoint(void)
{
	am3_usb_driver.rx_endpoint.out_endpoint_config.ep_mode 		= USB_EP_MODE_TYPE_BULK;
	am3_usb_driver.rx_endpoint.out_endpoint_config.setup_cb 	= NULL;
	am3_usb_driver.rx_endpoint.out_endpoint_config.in_cb		= NULL;
	am3_usb_driver.rx_endpoint.out_endpoint_config.out_cb		= usb_data_received;
	am3_usb_driver.rx_endpoint.out_endpoint_config.in_maxsize	= 0;
	am3_usb_driver.rx_endpoint.out_endpoint_config.out_maxsize	= USB_BULK_SIZE;
	am3_usb_driver.rx_endpoint.out_endpoint_config.in_state		= NULL;
	am3_usb_driver.rx_endpoint.out_endpoint_config.out_state	= &(am3_usb_driver.rx_endpoint.out_endpoint_state);
}


/**********************************************************************************************/
void am3_connect_usb(void)
{
	usbDisconnectBus(am3_usb_driver.usb_driver);
	usbStop(am3_usb_driver.usb_driver);
	disconnect_usb();
	reset_usb_phy();
    chThdSleepMilliseconds(150);

	usbStart(am3_usb_driver.usb_driver, &usb_config);

	am3_usb_driver.usb_configured = false;
	connect_usb();
	usbConnectBus(am3_usb_driver.usb_driver);
}


/**********************************************************************************************/
void release_control_buffer(void* buffer)
{
	AM3_Buffer* control_buffer = (AM3_Buffer*) buffer;
	control_buffer->free = true;
	control_buffer->timeout = 0;
	chSysLockFromISR();
	chMBPostI(am3_usb_driver.free_buffer,(msg_t)control_buffer);
	chSysUnlockFromISR();
}


/**********************************************************************************************/
void release_control_bufferS(void* buffer)
{
	AM3_Buffer* control_buffer = (AM3_Buffer*) buffer;
	control_buffer->free = true;
	control_buffer->timeout = 0;
	chSysLock();
	chMBPostS(am3_usb_driver.free_buffer,(msg_t)control_buffer,TIME_IMMEDIATE);
	chSysUnlock();
}


/**********************************************************************************************/
void am3_usb_refresh_timeout(uint32_t elapsed)
{
	uint8_t swap_buffer;
	for(swap_buffer=0 ; swap_buffer < USB_SWAP_DEPTH ; swap_buffer++)
		if(am3_usb_driver.usb_buffer[swap_buffer].free == false)
			am3_usb_driver.usb_buffer[swap_buffer].timeout += elapsed;
}


/**********************************************************************************************/
void am3_retrieve_lost_bufferS(AM3_Communication* am3_communication)
{
	uint8_t swap_buffer;
//	bool lost_fount = false;
	for(swap_buffer=0 ; swap_buffer < USB_SWAP_DEPTH ; swap_buffer++)
	{
		if((am3_usb_driver.usb_buffer[swap_buffer].free == false
						&& am3_usb_driver.usb_buffer[swap_buffer].timeout > 100000
						&& am3_communication->control_rx.receiving != &am3_usb_driver.usb_buffer[swap_buffer])
						|| am3_usb_driver.usb_buffer[swap_buffer].free == true)
		{
			am3_usb_driver.usb_buffer[swap_buffer].free = true;
			chSysLock();
			chMBPostS(am3_usb_driver.free_buffer,(msg_t)&am3_usb_driver.usb_buffer[swap_buffer],TIME_IMMEDIATE);
			chSysUnlock();
//			lost_fount = true;
		}
	}
}


/**********************************************************************************************/
void am3_retrieve_lost_bufferI(AM3_Communication* am3_communication)
{
	uint8_t swap_buffer;
	for(swap_buffer=0 ; swap_buffer < USB_SWAP_DEPTH ; swap_buffer++)
	{
		if((am3_usb_driver.usb_buffer[swap_buffer].free == false
				&& am3_usb_driver.usb_buffer[swap_buffer].timeout > 100000
				&& am3_communication->control_rx.receiving != &am3_usb_driver.usb_buffer[swap_buffer])
				|| am3_usb_driver.usb_buffer[swap_buffer].free == true)
		{
			am3_usb_driver.usb_buffer[swap_buffer].free = true;
			am3_usb_driver.usb_buffer[swap_buffer].timeout = 0;
			chSysLockFromISR();
			chMBPostI(am3_usb_driver.free_buffer,(msg_t)&am3_usb_driver.usb_buffer[swap_buffer]);
			chSysUnlockFromISR();
		}
	}
}


/**********************************************************************************************/
void am3_clear_buffer(AM3_Communication* am3_communication)
{
	uint8_t swap_buffer;
	for(swap_buffer=0 ; swap_buffer < USB_SWAP_DEPTH ; swap_buffer++)
	{
		am3_usb_driver.usb_buffer[swap_buffer].bytes = USB_BUFFER_SIZE;
		am3_usb_driver.usb_buffer[swap_buffer].buffer = (void*) &am3_usb_driver.usb_swap[swap_buffer];
		am3_usb_driver.usb_buffer[swap_buffer].released = release_control_buffer;
		if(is_transmitting(am3_usb_driver.usb_buffer[swap_buffer].buffer) == false && is_receiving(am3_usb_driver.usb_buffer[swap_buffer].buffer) == false)
		{
			(void)chMBPost(&am3_communication->free_buffer.fifo,(msg_t)&am3_usb_driver.usb_buffer[swap_buffer],TIME_IMMEDIATE);

			am3_usb_driver.usb_buffer[swap_buffer].timeout = 0;
			am3_usb_driver.usb_buffer[swap_buffer].free = true;
		}
	}
}


/**********************************************************************************************/
void init_am3_usb(AM3_Communication* am3_communication)
{
	am3_usb_driver.communication_ready = am3_communication->communication_ready;
	am3_usb_driver.free_buffer = &am3_communication->free_buffer.fifo;

	am3_clear_buffer(am3_communication);

	am3_usb_driver.rx_endpoint.rx_channel = &am3_communication->control_rx;
	am3_usb_driver.rx_endpoint.rx_channel->receiving	= 0;
	am3_usb_driver.rx_endpoint.rx_channel->prepare_receive	= prepare_receive;
	am3_usb_driver.rx_endpoint.rx_channel->init				= init_rx_endpoint;
	
	uint8_t endpoint;
	for(endpoint=0 ; endpoint < USB_TX_ENDPOINTS_NUMBER ; endpoint++)
	{
		if(endpoint == 0)
		{
			am3_usb_driver.tx_endpoint[endpoint].tx_channel = &am3_communication->control_tx;
			am3_usb_driver.tx_endpoint[endpoint].tx_channel->send				= send;
			am3_usb_driver.tx_endpoint[endpoint].tx_channel->init				= 0;
		}
		else
		{
#ifdef HAS_TX_MEASURE_CHANNEL
			am3_usb_driver.tx_endpoint[endpoint].tx_channel = &am3_communication->measure_channel_tx[endpoint-1];
			am3_usb_driver.tx_endpoint[endpoint].tx_channel->send				= send;
			am3_usb_driver.tx_endpoint[endpoint].tx_channel->init				= 0;
#endif
		}
		am3_usb_driver.tx_endpoint[endpoint].in_endpoint_config.ep_mode		= USB_EP_MODE_TYPE_BULK;
		am3_usb_driver.tx_endpoint[endpoint].in_endpoint_config.setup_cb 	= NULL;
		am3_usb_driver.tx_endpoint[endpoint].in_endpoint_config.in_cb		= usb_data_transmitted;
		am3_usb_driver.tx_endpoint[endpoint].in_endpoint_config.out_cb		= NULL;
		am3_usb_driver.tx_endpoint[endpoint].in_endpoint_config.in_maxsize	= USB_BULK_SIZE;
		am3_usb_driver.tx_endpoint[endpoint].in_endpoint_config.out_maxsize	= 0;
		am3_usb_driver.tx_endpoint[endpoint].in_endpoint_config.in_state		= &(am3_usb_driver.tx_endpoint[endpoint].in_endpoint_state);
		am3_usb_driver.tx_endpoint[endpoint].in_endpoint_config.out_state	= NULL;
	}

    am3_usb_driver.usb_driver                   = & HAS_USB_COMMUNICATION;

	chEvtObjectInit(&am3_usb_driver.evt_connected);
	chEvtObjectInit(&am3_usb_driver.evt_ejected);

	am3_connect_usb();
}

#endif

/*
 * am3_usb.h
 *
 *  Created on: 13 mars 2015
 *      Author: Electro_Laure
 */

#ifndef AM3_USB_H_
#define AM3_USB_H_

#include "ch.h"
#include "am3_board_generic.h"
#include "am3_communication_generic.h"

#ifdef HAS_USB_COMMUNICATION
typedef struct {

	USBInEndpointState 		in_endpoint_state;
	USBEndpointConfig 		in_endpoint_config;
	AM3_TX_Channel* 		tx_channel;
} InEndpointDriver;

typedef struct {

	USBOutEndpointState 	out_endpoint_state;
	USBEndpointConfig 		out_endpoint_config;
	AM3_RX_Channel* 		rx_channel;
} OutEndpointDriver;


typedef struct {
	USBDriver*				usb_driver;
	event_source_t 			evt_connected, evt_ejected;

	InEndpointDriver		tx_endpoint[USB_TX_ENDPOINTS_NUMBER];
	OutEndpointDriver		rx_endpoint;
	
	bool 					usb_configured;

    AM3_Buffer              usb_buffer[USB_SWAP_DEPTH];
    uint8_t                 usb_swap[USB_SWAP_DEPTH][USB_BUFFER_SIZE];
	void					(*communication_ready)(bool);
	mailbox_t*				free_buffer;
}AM3_USB_Driver;

void init_am3_usb(AM3_Communication* am3_communication);

void am3_usb_configure_endpoints(void);
void am3_usb_communication_ready(bool);
void am3_usb_refresh_timeout(uint32_t elapsed);
void am3_usb_tx_clear(uint8_t tx_endpoint);
void am3_connect_usb(void);
void am3_clear_buffer(AM3_Communication* am3_communication);
void am3_retrieve_lost_bufferS(AM3_Communication* am3_communication);
void am3_retrieve_lost_bufferI(AM3_Communication* am3_communication);
void release_control_bufferS(void* buffer);
AM3_Buffer* am3_get_forced_buffer(void);

#endif

#endif /* AM3_USB_H_ */

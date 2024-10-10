#include "am3_descriptor_generic.h"
#include "am3_identification_generic.h"
#include "am3_board_generic.h"
#include "am3_identification_generic.h"
#include "am3_usb_generic.h"

#ifdef HAS_USB_COMMUNICATION
#define USB_COM_CONFIG_DESC_SIZE		(9+9+(USB_TX_ENDPOINTS_NUMBER+1)*7)
/*
 * USB Device Descriptor.
 */
USBDescriptor usb_desc;

static const uint8_t am3_descriptor_data[18] = {
  USB_DESC_DEVICE       (BCD_USB,        /* bcdUSB (1.1).                    */
                         0x00,          /* bDeviceClass (CDC).              */
                         0x00,          /* bDeviceSubClass.                 */
                         0x00,          /* bDeviceProtocol.                 */
                         0x40,          /* bMaxPacketSize.                  */
						 PRODUCT_VID,        /* idVendor (ST).                   */
						 PRODUCT_PID,        /* idProduct.                       */
                         0x0200,        /* bcdDevice.                       */
                         1,             /* iManufacturer.                   */
                         2,             /* iProduct.                        */
                         3,             /* iSerialNumber.                   */
                         1)             /* bNumConfigurations.              */
};

/*
 * Device Descriptor wrapper.
 */
static const USBDescriptor am3_descriptor = {
  sizeof am3_descriptor_data,
  am3_descriptor_data
};

/* Configuration Descriptor tree for a CDC.*/
static const uint8_t am3_configuration_descriptor_data[USB_COM_CONFIG_DESC_SIZE] = {
  /* Configuration Descriptor.*/
  USB_DESC_CONFIGURATION(USB_COM_CONFIG_DESC_SIZE,            /* wTotalLength.                    */
                         0x01,          /* bNumInterfaces.                  */
                         0x01,          /* bConfigurationValue.             */
                         1,             /* iConfiguration.                  */
                         0x80,          /* bmAttributes (self powered).     */
                         0xFA),           /* bMaxPower (100mA).               */
  /* Interface Descriptor.*/
  USB_DESC_INTERFACE    (0x00,          /* bInterfaceNumber.                */
                         0x00,          /* bAlternateSetting.               */
						 USB_TX_ENDPOINTS_NUMBER+1,          /* bNumEndpoints.                   */
                         0xFF,          /* bInterfaceClass (Communications
                                           Interface Class, CDC section
                                           4.2).                            */
                         0xFF,          /* bInterfaceSubClass (Abstract
                                         Control Model, CDC section 4.3).   */
                         0xFF,          /* bInterfaceProtocol (AT commands,
                                           CDC section 4.4).                */
                         2),            /* iInterface.                      */

  USB_DESC_ENDPOINT     (TX_USB_ENDPOINT_CONTROL,
                         USB_EP_MODE_TYPE_BULK,          /* bmAttributes (Bulk). */
                         USB_BULK_SIZE,        /* wMaxPacketSize.                  */
                         0x00),         /* bInterval.                       */
  USB_DESC_ENDPOINT     (RX_USB_ENDPOINT_CONTROL,       /* bEndpointAddress.*/
						  USB_EP_MODE_TYPE_BULK,          /* bmAttributes (Bulk). */
						  USB_BULK_SIZE,        /* wMaxPacketSize.                  */
						 0x00)
#ifdef USB_TX_ENDPOINTS_NUMBER > 1
						 ,  /* bInterval.  */
USB_DESC_ENDPOINT     	(TX_USB_ENDPOINT_MEASURE_1,       /* bEndpointAddress.*/
		  	  	  	  	  USB_EP_MODE_TYPE_BULK,          /* bmAttributes (Bulk).             */
						  USB_BULK_SIZE,        /* wMaxPacketSize.                  */
						  0x00)
#if USB_TX_ENDPOINTS_NUMBER > 2
						  ,         /* bInterval.                       */
USB_DESC_ENDPOINT     	(TX_USB_ENDPOINT_MEASURE_2,    /* bEndpointAddress.*/
		  	  	  	  	  USB_EP_MODE_TYPE_BULK,          /* bmAttributes (Bulk).             */
						  USB_BULK_SIZE,        /* wMaxPacketSize.                  */
						  0x00)          /* bInterval.                       */
#endif
#if USB_TX_ENDPOINTS_NUMBER > 3
                          ,         /* bInterval.                       */
USB_DESC_ENDPOINT       (TX_USB_ENDPOINT_MEASURE_3,    /* bEndpointAddress.*/
                          USB_EP_MODE_TYPE_BULK,          /* bmAttributes (Bulk).             */
                          USB_BULK_SIZE,        /* wMaxPacketSize.                  */
                          0x00)          /* bInterval.                       */
#endif
#if USB_TX_ENDPOINTS_NUMBER > 4
                          ,         /* bInterval.                       */
USB_DESC_ENDPOINT       (TX_USB_ENDPOINT_MEASURE_4,    /* bEndpointAddress.*/
                          USB_EP_MODE_TYPE_BULK,          /* bmAttributes (Bulk).             */
                          USB_BULK_SIZE,        /* wMaxPacketSize.                  */
                          0x00)          /* bInterval.                       */
#endif
#endif
};


static const USBDescriptor am3_configuration_descriptor = {
  sizeof am3_configuration_descriptor_data,
  am3_configuration_descriptor_data
};


static const uint8_t am3_language_string[] = {
  USB_DESC_BYTE(4),                     /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  USB_DESC_WORD(0x0409)                 /* wLANGID (U.S. English).          */
};

/*
 * Vendor string.
 */
static const uint8_t am3_vendor_string[] = {
  USB_DESC_BYTE(14),                    /* bLength.                         */
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING), /* bDescriptorType.                 */
  'T', 0, 'O', 0, 'T', 0, 'O', 0,
};

/*
 * Strings wrappers array.
 */
static const USBDescriptor am3_strings[] = {
  {sizeof am3_language_string, am3_language_string},
  {sizeof am3_vendor_string, am3_vendor_string}
};


#define HARDWARE_SIZE							8
#define FIRMWARE_MICRO_SIZE						8
#define FIRMWARE_CPLD_SIZE						8
#define FIRMWARE_BOOTLOADER_SIZE				8
#define FIRMWARE_WIFI_SIZE						8
uint16_t	unicode_hardware_version[HARDWARE_SIZE+1];
uint16_t	unicode_micro_version[FIRMWARE_MICRO_SIZE+1];
uint16_t	unicode_cpld_version[FIRMWARE_CPLD_SIZE+1];
uint16_t	unicode_bootloader_version[FIRMWARE_BOOTLOADER_SIZE+1];
uint16_t	unicode_wifi_version[FIRMWARE_WIFI_SIZE+1];
static const USBDescriptor *get_descriptor(USBDriver *usbp,
                                           uint8_t dtype,
                                           uint8_t dindex,
                                           uint16_t lang) {

  (void)usbp;
  (void)lang;
  switch (dtype) {
  case USB_DESCRIPTOR_DEVICE:
	return &am3_descriptor;
  case USB_DESCRIPTOR_CONFIGURATION:
    return &am3_configuration_descriptor;
  case USB_DESCRIPTOR_STRING:
    if (dindex < 2)
      return &am3_strings[dindex];
    if(dindex == 2)
    {
    	usb_desc.ud_size = PRODUCT_SIZE*2+2;
    	usb_desc.ud_string = (uint8_t*) am3_product_properties.unicode_product_identification_string;
    	return &usb_desc;
    }
    if(dindex == 3)
    {
    	usb_desc.ud_size = SERIAL_SIZE*2+2;
    	usb_desc.ud_string = (uint8_t*) am3_product_properties.unicode_board_serial_number;
    	return &usb_desc;
    }

    if(dindex == 4)
    {
    	usb_desc.ud_size = HARDWARE_SIZE*2+2;
    	usb_desc.ud_string = (uint8_t*) am3_product_properties.unicode_hardware_version;
    	return &usb_desc;
    }

    if(dindex == 5)
    {
    	usb_desc.ud_size = FIRMWARE_MICRO_SIZE*2+2;
    	usb_desc.ud_string = (uint8_t*) am3_product_properties.unicode_micro_version;
    	return &usb_desc;
    }


    if(dindex == 6)
    {
    	usb_desc.ud_size = FIRMWARE_CPLD_SIZE*2+2;
    	usb_desc.ud_string = (uint8_t*) am3_product_properties.unicode_cpld_version;;
    	return &usb_desc;
    }

    if(dindex == 7)
    {
    	usb_desc.ud_size = FIRMWARE_BOOTLOADER_SIZE*2+2;
    	usb_desc.ud_string = (uint8_t*) am3_product_properties.unicode_bootloader_version;;
    	return &usb_desc;
    }
  }
  return NULL;
}



static void usb_event(USBDriver *usbp, usbevent_t event) {
	switch (event) {
  case USB_EVENT_RESET:
	  am3_usb_communication_ready(false);
    return;
  case USB_EVENT_ADDRESS:
    return;
  case USB_EVENT_CONFIGURED:
	  am3_usb_communication_ready(true);
	  am3_usb_configure_endpoints();
    return;
  case USB_EVENT_SUSPEND:
    return;
  case USB_EVENT_WAKEUP:
    return;
  case USB_EVENT_STALLED:
    return;
  }
  return;
}

bool requestHook(USBDriver* usbp)
{
	switch ((((uint32_t)usbp->setup[0] & (USB_RTYPE_RECIPIENT_MASK |
	                                        USB_RTYPE_TYPE_MASK)) |
	           ((uint32_t)usbp->setup[1] << 8U))) {
	case (uint32_t)USB_RTYPE_RECIPIENT_ENDPOINT | ((uint32_t)USB_REQ_CLEAR_FEATURE << 8):
	    /* Only ENDPOINT_HALT is handled as feature.*/
	    if (usbp->setup[2] != USB_FEATURE_ENDPOINT_HALT) {
	      return false;
	    }
	    /* Clearing the EP status, not valid for EP0, it is ignored in that case.*/
	    if ((usbp->setup[4] & 0x0FU) != 0U) {
	      if ((usbp->setup[4] & 0x80U) != 0U) {
		    //am3_usb_tx_clear(usbp->setup[4] & 0x0FU);
	        usb_lld_clear_in(usbp, usbp->setup[4] & 0x0FU);

	      }
	      else {
	        usb_lld_clear_out(usbp, usbp->setup[4] & 0x0FU);
	      }
	    }
	    usbSetupTransfer(usbp, NULL, 0, NULL);
	    return true;
	}
   return false;
}

const USBConfig usb_config = {
  usb_event,
  get_descriptor,
  requestHook,
  NULL
};
#endif

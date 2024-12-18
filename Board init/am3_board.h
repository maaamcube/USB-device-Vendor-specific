#ifndef AM3_BOARD_H_
#define AM3_BOARD_H_



//#define AVEC_CALCUL_DE_TEMPS                1


/**********************************************************************************************/
// Configuration de la version de plate forme
/**********************************************************************************************/
//#define V8_S                      1
//#define V8_M                      1
//#define V8_L                      1

//#define EXTERNAL_FLASH_IS_PRESENT           1


/**********************************************************************************************/
// Message d'erreur empechant la compilation en cas de selection multiple
/**********************************************************************************************/
/*#if ((V8_S && V8_M) || (V8_S && V8_L) || (V8_M && V8_L))
  "Error : le projet ne peut être compilé qu'avec un seul type de plate-forme"
#endif*/


/**********************************************************************************************/
// DEFINE REVISION et VERSION
/**********************************************************************************************/
//#define FLASH_PAGE_SIZE						0x800


#define UNIQUE_ID_ADDRESS					(uint32_t*)(0x1FFFF7AC)
#define PRODUCT_VID							0x483

#ifdef REV_1_4_0
#define HARD_REV_MAJOR		'1'
#define HARD_REV_MINOR		'4' //'3'
#define HARD_REV_SUB_MINOR  '0' //'4'
#endif

#ifdef V8_S
// S
  #define PRODUCT_PID                         0x7000
  #define PRODUCT_STRING						{ 'P','0','4','k','6',HARD_REV_MAJOR,HARD_REV_MINOR,HARD_REV_SUB_MINOR }

//  #define PRODUCT_PID                         0x7002
//  #define PRODUCT_STRING                      { 'P','5','0','k','6',HARD_REV_MAJOR,HARD_REV_MINOR,HARD_REV_SUB_MINOR }

#endif

#ifdef V8_M
// M
  #define PRODUCT_PID                         0x7001
  #define PRODUCT_STRING                      { 'P','4','9','k','6',HARD_REV_MAJOR,HARD_REV_MINOR,HARD_REV_SUB_MINOR }
#endif

#ifdef V8_L
// L
  #define PRODUCT_PID                         0x7002
  #define PRODUCT_STRING                      { 'P','5','0','k','6',HARD_REV_MAJOR,HARD_REV_MINOR,HARD_REV_SUB_MINOR }
#endif

#define MICROCONTROLLER_FIRMWARE_VERSION    FIRMWARE_VERSION
#define WIFI_FIRMWARE_VERSION               0x00010000


/**********************************************************************************************/
// DEFINE CALIBRATION ET FLASH EXTERNE
/**********************************************************************************************/
#define UNIQUE_BIAS_ADDRESS                 (uint32_t) (0x08007FE0)
#define BIAS_DEFAULT                        390

#define CALIBRATION_RESERVED                0xF350//0xA000//0x1000
#define CALIBRATION_ADDRESS                 0x08008000
#define calibration_type_t                  uint8_t

// FLASH EXTERNE
#ifdef EXTERNAL_FLASH_IS_PRESENT
  #define set_cs_flash()                      palSetPad(GPIOA,GPIOA_CS_FLASH)
  #define reset_cs_flash()                    palClearPad(GPIOA,GPIOA_CS_FLASH)
  #define EXTERNAL_CALIBRATION_ADDRESS(index) (index * (USB_BUFFER_SIZE-8))
#endif



/**********************************************************************************************/
// Positions de la carte au sein de la plate-forme
/**********************************************************************************************/
#define POSITION_0                          0
#define POSITION_1                          1
#define POSITION_2                          2


/**********************************************************************************************/
// DEFINE USB
/**********************************************************************************************/
//#define USE_USB_1_0
#define USE_USB_2_0

#define HAS_USB_COMMUNICATION               USBD1
#define HAS_TX_MEASURE_CHANNEL
#define TX_MEASURE_CHANNEL_NUMBER           2

#define USB_BUFFER_SIZE						0x1040//4160//0x440
#define USB_SWAP_DEPTH						0x04 //0x05

#define TAILLE_TRAME_ACQ_SANS_ZIP           (4096+8)
#define TAILLE_TRAME_ACQ_AVEC_ZIP           (4096+20)

#define disconnect_usb()                    palClearPad(GPIOB,GPIOB_USB_CONNECT)
#define connect_usb()                       palSetPad(GPIOB,GPIOB_USB_CONNECT)

#define usb_lld_disconnect_bus(usbp)
#define usb_lld_connect_bus(usbp)
#define reset_usb_phy()


/**********************************************************************************************/
// DEFINE ADC
/**********************************************************************************************/
#define ADC_BUFFER_NUMBER                   8

#define ADC_SAMPLING_COUNT                  8
#define ADC_HEIGHT_SENSOR_COUNT				8
#define ADC_WIDTH_SENSOR_COUNT				64

#define ADC_SAMPLING_STEP					1
#define ADC_SWAP_DEPTH						USB_SWAP_DEPTH - 1
#define ADC_CONFIG						(ADC_CFGR_DMAEN |	/*DMA enable			*/\
											ADC_CFGR_EXTEN_0|	/*Trigger on rising edge*/\
											ADC_CFGR_EXTSEL_0|	/*Event 5: External pin	*/\
											ADC_CFGR_EXTSEL_2|\
											ADC_CFGR_RES_1|		/* 8 bits et ADC_CFGR_RES_0 pour 10 bits */ \
											ADC_CFGR_DISCEN)


// Reglage du sample & hod time (duree entre le front montant du signal ADC_TRIG et le début de conversion ADC)
// Une seule des 5 lignes sans commentaires !!!
// 1.5 ADC clock cycles - defaut V7
// 31.25ns
#define ADC_SAMPLE_TIME     0

//// 2.5 ADC clock cycles
//// 31.25ns
//#define ADC_SAMPLE_TIME     ADC_SMPR1_SMP3_0
//
//// 4.5 ADC clock cycles
//// 93.75ns
//#define ADC_SAMPLE_TIME     ADC_SMPR1_SMP3_1
//
//// 7.5 ADC clock cycles
//// 156.25ns
//#define ADC_SAMPLE_TIME     ADC_SMPR1_SMP3_1 | ADC_SMPR1_SMP3_0
//
//// 19.5 ADC clock cycles
//// 406.25ns
//#define ADC_SAMPLE_TIME     ADC_SMPR1_SMP3_2


#define DMA_CONFIG						(DMA_CCR_PL |\
											DMA_CCR_MINC |\
											DMA_CCR_PSIZE_0 |\
											DMA_CCR_TCIE |\
											DMA_CCR_EN)


#define ADC_init_clock()				RCC->AHBENR |= RCC_AHBENR_ADC34EN | RCC_AHBENR_DMA2EN
#define ADC_deinit_clock()				RCC->AHBENR &= ~(RCC_AHBENR_ADC34EN | RCC_AHBENR_DMA2EN)
#define ADC_REGISTER					ADC3
#define ADC_COMMON						ADC3_4_COMMON
#define ADC_DMA							DMA2
#define ADC_clear_isr()					ADC_DMA->IFCR = ADC_DMA->IFCR | 0x00070000
#define ADC_DMA_CHANNEL					DMA2_Channel5
#define ADC_DMA_CHANNEL_IRQ				DMA2_Channel5_IRQn
#define ADC_ISR_HANDLER					adc_vector
#define ADC_CHANNEL						ADC_SQR1_SQ1_0
#define adc_sample_size_t				uint8_t


/**********************************************************************************************/
// DEFINE FPGA
/**********************************************************************************************/
#define pal_start_acquisition()			palSetPad(GPIOA,GPIOA_ENABLE);
#define pal_stop_acquisition()			palClearPad(GPIOA,GPIOA_ENABLE);

#define BOOTLOADER_FLASH_ADDRESS        0x08005A00
//#define BOOTLOADER_FLASH_SIZE           0x4000
#define BOOTLOADER_FLASH_SIZE           8192
#define HAS_CPLD_RAM
#define SPI_RAM_CPLD                    SPID1
#define set_cpld()                      palSetPad(GPIOA,GPIOA_RST)
#define reset_cpld()                    palClearPad(GPIOA,GPIOA_RST)
#define set_cs_cpld()                   palSetPad(GPIOB,GPIOB_SPI_CS)
#define reset_cs_cpld()                 palClearPad(GPIOB,GPIOB_SPI_CS)
#define CPLD_FLASH_ADDRESS              BOOTLOADER_FLASH_ADDRESS

#define PAL_CPLD_VERSION                0
#define PAL_HARDWARE_VERSION            (   (palReadPad(GPIOB,GPIOB_VER_HARD3) << 2) |\
                                            (palReadPad(GPIOB,GPIOB_VER_HARD2) << 1) )

#define set_switch_1()                  palSetPad(GPIOB,GPIOB_C3);
#define set_switch_2()                  palSetPad(GPIOB,GPIOB_C4);
#define set_switch_3()                  palSetPad(GPIOB,GPIOB_C5);

#define clear_switch_1()                palClearPad(GPIOB,GPIOB_C3);
#define clear_switch_2()                palClearPad(GPIOB,GPIOB_C4);
#define clear_switch_3()                palClearPad(GPIOB,GPIOB_C5);


/**********************************************************************************************/
// DEFINE DIVERS
/**********************************************************************************************/
#define STACK_THREAD                        256

#define WATCHDOG_DISABLE


#endif /* AM3_BOARD_H_ */

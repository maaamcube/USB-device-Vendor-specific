#include "am3_communication_generic.h"
#include "am3_calibration_generic.h"
#include "am3_msg_generic.h"
#include "am3_measurement_generic.h"
#include "am3_measurement.h"
#include "am3_watchdog.h"
#include "am3_board_status_generic.h"
#include "am3_usb_generic.h"

#include "C:\ChibiStudio/tools\GNU Tools ARM Embedded\6.3 2017q1\arm-none-eabi\include/stdio.h"
#include "C:\ChibiStudio/tools\GNU Tools ARM Embedded\6.3 2017q1\arm-none-eabi\include/string.h"


/**********************************************************************************************/
#ifdef AVEC_CALCUL_DE_TEMPS
    extern uint32_t Temps[25] ;
#endif

extern uint32_t CtrMesures ;
extern uint8_t PositionCarte ;

/**********************************************************************************************/
void check_tx_state(void);

uint32_t AdresseSeuilCalibration ;

AM3_Zip am3_zip ;

AM3_Communication am3_communication ;

#ifdef AVEC_TIME
  uint16_t time_count ;
  uint32_t time_type[500] ;
  uint32_t time_ms[500] ;
#endif


/**********************************************************************************************/
void append_to_send_measure_fifo(AM3_Buffer* to_send) ;
void check_for_new_transmissionI(AM3_TX_Channel* channel) ;


/**********************************************************************************************/
extern uint8_t ImagePreteEnvoi ;
//extern uint8_t __attribute__((section (".ram4"))) BufferAcquisition[8][512] ;
extern uint8_t BufferAcquisition[8][512] ;
extern uint8_t FlagEOC ;


/**********************************************************************************************/
#ifdef AVEC_TIME
void add_time_measure(uint32_t id)
{
  if(time_count<500)
  {
    systime_t time = chVTGetSystemTime() ;
    time_ms[time_count] = ST2US(time) ;
    time_type[time_count++] = id ;
  }
}
#endif


/**********************************************************************************************/
void AttentFinDeTransmissionUSB(void)
{
  extern AM3_USB_Driver am3_usb_driver;

// Necessaire pour rendre la main et que la transmission USB commence
  while (am3_usb_driver.tx_endpoint[1].tx_channel->transmitting != 0)
  {
    chThdSleepMicroseconds(5) ;
  }
}


/**********************************************************************************************/
void Send_V8_ZIP(void)
{
  uint32_t check ;

  if (is_measurement_started() == false) return ;

#ifdef AVEC_CALCUL_DE_TEMPS
Temps[3] = chVTGetSystemTime() ;
#endif

// Attente de la fin d'une d'eventuelle transmission USB (pour ne pas ecraser le buffer)
  AttentFinDeTransmissionUSB() ;

#ifdef AVEC_CALCUL_DE_TEMPS
Temps[4] = chVTGetSystemTime() ;
#endif

// Si aucune mesure au dessus de son seuil, quitter
//  if (CtrMesures == 0)
//  {
//    return ;
//  }

// Par defaut, il s'agit du premier bloc et il n'y en aura pas d'autre (cas le plus probable)
//  if (am3_zip.overflow == false)
//  {
//    check = 0x00010000 ;
//  }
//  else
//  {
//    check = 0x00010001 ;
//  }
    check = 0x00010000 ;
// V8 avec ZIP
  int_to_be(ID_TX_MEASURE_V8_ZIP_MSG, &am3_zip.current_com_buffer.buffer[0]) ;

// Ajout de la datation
  int_to_be(am3_acquisition.time, &am3_zip.zip_buffer[4]) ;

// Ajout de la datation
  int_to_be(am3_acquisition.time, &am3_zip.zip_buffer[12]) ;

// Mise a jour du check dans la trame
  int_to_be(check, &am3_zip.zip_buffer[8]) ;

// Ajout du nombre de mesure zipees envoyées
  int_to_be(CtrMesures, &am3_zip.zip_buffer[16]) ;

  if (CtrMesures == 0)  int_to_be(0, &am3_zip.zip_buffer[20]) ;

// Taille de la trame
  am3_zip.current_com_buffer.bytes = (CtrMesures*3)+20 ;
  am3_zip.current_com_buffer.buffer = &am3_zip.zip_buffer[0] ;
  append_to_send_measure_fifo(&am3_zip.current_com_buffer);

  am3_zip.overflow = false ;


}

/**********************************************************************************************/
void Send_V8(void)
{
  if (is_measurement_started() == false) return ;

#ifdef AVEC_CALCUL_DE_TEMPS
Temps[3] = chVTGetSystemTime() ;
#endif


// Attente de la fin d'une d'eventuelle transmission USB (pour ne pas ecraser le buffer)
  AttentFinDeTransmissionUSB() ;


#ifdef AVEC_CALCUL_DE_TEMPS
Temps[4] = chVTGetSystemTime() ;
#endif


// V8 sans ZIP
  int_to_be(ID_TX_MEASURE_V8_MSG, &am3_zip.zip_buffer[0]);

// Ajout de la datation
  int_to_be(am3_acquisition.time, &am3_zip.zip_buffer[4]) ;


// Ajout des données
#ifdef V8_S
  if (PositionCarte == POSITION_0)
  {
      memcpy(&am3_zip.zip_buffer[8], &BufferAcquisition[0][0], 4096) ;
  }
#endif

#ifdef V8_M

  if (PositionCarte == POSITION_0)
  {
      memcpy(&am3_zip.zip_buffer[8], &BufferAcquisition[0][0], 4096) ;
  }

  if(PositionCarte == POSITION_1) // PositionCarte = POSITION_1 => demi carte
  {
    uint32_t ptrRemplissage, ptrBuffer ;
    ptrRemplissage = 8 ;
    ptrBuffer = 0 ;  // pour le nouvel ordre mettre à 0 (l'ancienne rotation = 32)

    for (int t = 0 ; t < 64 ; t++)
    {
        memcpy(&am3_zip.zip_buffer[ptrRemplissage], (uint8_t *)(&BufferAcquisition[0][0] + ptrBuffer) , 32) ;
        ptrRemplissage += 32 ;
        ptrBuffer += 64 ;
    }

    // Taille de la trame : par défaut pour une S (4096), 2048 pour une M
      am3_zip.current_com_buffer.bytes = (USB_BUFFER_SIZE / 2) ;
  }
#endif

#ifdef V8_L
  if (PositionCarte == POSITION_0 ||PositionCarte == POSITION_1 || PositionCarte == POSITION_2)
   {
       memcpy(&am3_zip.zip_buffer[8], &BufferAcquisition[0][0], 4096) ;
   }
#endif

  am3_zip.current_com_buffer.buffer = &am3_zip.zip_buffer[0] ;
  append_to_send_measure_fifo(&am3_zip.current_com_buffer);
}


/**********************************************************************************************/
void check_for_new_transmissionS(AM3_TX_Channel* channel)
{
  if (channel->transmitting == 0)
  {
    msg_t msg = chMBFetchS(&channel->fifo->fifo, (msg_t*)&channel->transmitting, TIME_IMMEDIATE) ;
    if (msg == MSG_OK)
    {
      channel->send(channel) ;
    }
    else
    {
      channel->transmitting = 0 ;
    }
  }
}


/**********************************************************************************************/
bool is_receiving(AM3_Buffer* buffer)
{
  return (am3_communication.control_rx.receiving == buffer) ;
}


/**********************************************************************************************/
bool is_transmitting(AM3_Buffer* buffer)
{
  bool transmitting = false;
  uint8_t tx_measure_index;

  transmitting |= (am3_communication.control_tx.transmitting == buffer) ;
  for(tx_measure_index=0; tx_measure_index < TX_MEASURE_CHANNEL_NUMBER; tx_measure_index++)
  {
    transmitting |= (am3_communication.measure_channel_tx[tx_measure_index].transmitting == buffer) ;
  }
  return transmitting ;
}


/**********************************************************************************************/
void check_for_new_transmissionI(AM3_TX_Channel* channel)
{
  if (channel->transmitting == 0)
  {
    msg_t msg = chMBFetchI(&channel->fifo->fifo, (msg_t*)&channel->transmitting) ;
    if (msg == MSG_OK)
    {
      channel->send(channel) ;
    }
    else
    {
      channel->transmitting = 0 ;
    }
  }
}


/**********************************************************************************************/
void transmitted(void* tx_channel)
{
  AM3_TX_Channel* tx = (AM3_TX_Channel*)tx_channel ;
  if (tx->transmitting->released != 0)
  {
    tx->transmitting->released(tx->transmitting) ;
  }
  tx->transmitting = 0 ;

#ifdef AVEC_TIME
  add_time_measure(0x99) ;
#endif

  chSysLockFromISR() ;
  check_for_new_transmissionI(tx) ;
  chSysUnlockFromISR() ;
}


/**********************************************************************************************/
void control_received(void* rx_channel)
{
  AM3_RX_Channel* rx = (AM3_RX_Channel*) rx_channel ;
  chSysLockFromISR() ;
  chMBPostI(&am3_communication.control_rx.fifo.fifo,(msg_t)rx->receiving) ;
  chSysUnlockFromISR() ;
  rx->receiving = 0 ;
  rx->prepare_receive(rx) ;
}


/**********************************************************************************************/
static THD_FUNCTION( thread_communication, arg)
{
  chRegSetThreadName("Communication thread") ;
  AM3_Buffer* buffer_rx ;
  msg_t rx_msg ;

  while (1)
  {
    rx_msg = chMBFetch(&am3_communication.control_rx.fifo.fifo,(msg_t*)&buffer_rx,TIME_IMMEDIATE) ;
    if(rx_msg == MSG_OK)
    {
      parse_msg(buffer_rx);
      chMBPost(&am3_communication.free_buffer.fifo,(msg_t)buffer_rx,TIME_IMMEDIATE) ;
    }

    am3_usb_refresh_timeout(100) ;
    check_tx_state() ;
    chThdSleepMicroseconds(5) ;
  }
}

/**********************************************************************************************/
void reinit_zip(void)
{
    init_fifo(&am3_zip.fifo);

    am3_zip.size_byte = 0;

    am3_zip.current_com_buffer.bytes = USB_BUFFER_SIZE;

    am3_zip.current_com_buffer.buffer = (void*) &am3_zip.zip_buffer[0] ;

    am3_zip.current_com_buffer.released = 0;
    am3_zip.current_com_buffer.timeout = 0;

    am3_zip.current_com_buffer.free = true;
}


/**********************************************************************************************/
void communication_ready(bool ready)
{
  am3_communication.ready = ready;
}


/**********************************************************************************************/
void am3_communication_reinitialisation(void)
{
  am3_communication.ready = false;
  am3_communication.communication_ready = communication_ready;

  init_fifo(&am3_communication.free_buffer.fifo);
  init_am3_usb(&am3_communication);


  if(am3_communication.control_tx.init)
  am3_communication.control_tx.init();

  init_fifo(&am3_communication.control_fifo);
  am3_communication.control_tx.fifo = &am3_communication.control_fifo;
  am3_communication.control_tx.transmitted = transmitted;

  if(am3_communication.control_rx.init)
  am3_communication.control_rx.init();

  init_fifo(&am3_communication.control_rx.fifo);
  am3_communication.control_rx.received = control_received;

  reinit_zip();

#ifdef AVEC_TIME
  time_type[0] = 0;
#endif
  am3_clear_communication_buffer();
}


/**********************************************************************************************/
void am3_init_communication(void)
{
  am3_communication_reinitialisation() ;

// Recherche du debut des seuils dans la table des calibrations
  AdresseSeuilCalibration = 0 ;
 // uint8_t offset_key[14] = {0x00, 0x4F,0x00, 0x46,0x00, 0x46,0x00, 0x53,0x00, 0x45,0x00, 0x54,0x00, 0x00} ; penser à supprimer les deux 0 de la fin
  uint8_t offset_key[14] = {0x00, 0x4F,0x00, 0x46,0x00, 0x46,0x00, 0x53,0x00, 0x45,0x00, 0x54,0x00, 0x00} ;
  uint16_t i=0 ;
  for (i=0 ; i<CALIBRATION_RESERVED ; i++)
  {
    if (memcmp((uint8_t*)(CALIBRATION_ADDRESS+i), &offset_key[0], 14) == 0)
    {
      AdresseSeuilCalibration = (uint32_t)(CALIBRATION_ADDRESS+i+24) ;
      break;
    }
  }

#ifdef AVEC_TIME
  time_count = 0 ;
#endif

  am3_communication.communication_thread = chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(STACK_THREAD), "communication", NORMALPRIO,thread_communication, NULL) ;
}


/**********************************************************************************************/
void append_to_send_control_fifoS(AM3_Buffer* to_send)
{
  chSysLock();
  (void)chMBPostS(&am3_communication.control_tx.fifo->fifo,(msg_t)to_send,TIME_IMMEDIATE);
  chSysUnlock();
}


/**********************************************************************************************/
void append_to_send_control_fifoI(AM3_Buffer* to_send)
{
  chSysLockFromISR();
  (void)chMBPostI(&am3_communication.control_tx.fifo->fifo,(msg_t)to_send);
  chSysUnlockFromISR();
}


/**********************************************************************************************/
void append_to_send_measure_fifo(AM3_Buffer* to_send)
{
  if(is_measurement_started() == true)
  {
    chSysLockFromISR() ;
    (void)chMBPostI(&am3_communication.measure_fifo.fifo,(msg_t)to_send) ;
    chSysUnlockFromISR() ;

  // Laisser l'USB commencer l'envoi
    chThdSleepMicroseconds(5) ;
  }
  else
  {
//    to_send->released(to_send) ;
  }
}

/**********************************************************************************************/
void check_tx_state(void)
{
  chSysLock() ;
  check_for_new_transmissionS(&am3_communication.control_tx) ;

  uint8_t tx_measure_index ;
  if(is_measurement_started() == true)
  {
    for(tx_measure_index=0; tx_measure_index < TX_MEASURE_CHANNEL_NUMBER; tx_measure_index++)
    {
      check_for_new_transmissionS(&am3_communication.measure_channel_tx[tx_measure_index]);
    }
  }
  chSysUnlock() ;
}


/**********************************************************************************************/
AM3_Buffer* get_communication_bufferS(bool force)
{
  AM3_Buffer* buffer = 0;;
  msg_t msg;

  bool retrieve = false;

  while(buffer == 0 || is_receiving(buffer) || is_transmitting(buffer))
  {
    chSysLock();
    msg = chMBFetchS(&am3_communication.free_buffer.fifo,(msg_t*)&buffer,TIME_IMMEDIATE);
    chSysUnlock();

    if(msg != MSG_OK)
    {
      if(!retrieve)
      {
        am3_retrieve_lost_bufferS(&am3_communication);
        retrieve = true;
      }
      else
      {
        if(force)
        return am3_get_forced_buffer();
        return 0;
      }
    }

  }

  buffer->free = false;
  buffer->timeout = 0;
  return buffer;
}


/**********************************************************************************************/
AM3_Buffer* get_communication_bufferI(bool force)
{

  AM3_Buffer* buffer = 0;;
  msg_t msg;
  bool retrieve = false;

  while(buffer == 0 || is_receiving(buffer) || is_transmitting(buffer))
  {
    chSysLockFromISR();
    msg = chMBFetchI(&am3_communication.free_buffer.fifo,(msg_t*)&buffer);
    chSysUnlockFromISR();

    if(msg != MSG_OK)
    {
      if(!retrieve)
      {
        am3_retrieve_lost_bufferI(&am3_communication);
        retrieve = true;
      }
      else
      {
        if(force)
        return am3_get_forced_buffer();
        return 0;
      }
    }
  }
  buffer->free = false;
  buffer->timeout = 0;
  return buffer;
}


/**********************************************************************************************/
void am3_clear_communication_buffer(void)
{
  am3_clear_buffer(&am3_communication);

  init_fifo(&am3_communication.measure_fifo);
  uint8_t tx_measure_index;
  for(tx_measure_index=0; tx_measure_index < TX_MEASURE_CHANNEL_NUMBER; tx_measure_index++)
  {
    if(am3_communication.measure_channel_tx[tx_measure_index].init)
    {
      am3_communication.measure_channel_tx[tx_measure_index].init();
    }

    am3_communication.measure_channel_tx[tx_measure_index].fifo = &am3_communication.measure_fifo;
    am3_communication.measure_channel_tx[tx_measure_index].transmitted = transmitted;
  }
  am3_communication.sending_count = 0;

  reinit_zip();
}

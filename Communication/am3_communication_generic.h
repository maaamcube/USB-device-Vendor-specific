/**********************************************************************************************/
#ifndef AM3_COMMUNICATION_GENERIC_H_
#define AM3_COMMUNICATION_GENERIC_H_


/**********************************************************************************************/
#include "hal.h"
#include "am3_board_generic.h"
#include "am3_tools_generic.h"


/**********************************************************************************************/
typedef struct
{
	AM3_Fifo* 	fifo;
	AM3_Buffer* transmitting;

	void		(*init)(void);
	void 		(*send)(void*);
	void		(*transmitted)(void*);
} AM3_TX_Channel ;

typedef struct
{
	AM3_Fifo 	fifo;
	AM3_Buffer* receiving;

	void		(*init)(void);
	void 		(*prepare_receive)(void*);
	void		(*received)(void*);
} AM3_RX_Channel ;

typedef struct
{
	uint8_t msg_id[4];
	uint8_t counter[4];
} AM3_Watchdog_Reset ;

typedef struct
{
      AM3_Buffer            current_com_buffer ;
      adc_sample_size_t*    offset;
      uint32_t              size_byte;
      AM3_Fifo              fifo;
      adc_sample_size_t     zip_buffer[ZIP_BUFFER_SIZE] ;
      bool                  free[2] ;
      bool                  overflow ;
      uint32_t              lastAddress ;
} AM3_Zip ;

typedef struct
{
  AM3_TX_Channel 	control_tx;
  AM3_RX_Channel 	control_rx;
  AM3_Fifo 		free_buffer;
  AM3_Fifo 		control_fifo;

  AM3_TX_Channel	measure_channel_tx[TX_MEASURE_CHANNEL_NUMBER];
  AM3_Fifo 		measure_fifo;
  thread_t*   zip_thread;

  uint32_t sending_count;
  thread_t*	communication_thread;
  void		(*communication_ready)(bool);
  bool 		ready;
} AM3_Communication ;


/**********************************************************************************************/
void am3_init_communication(void);
void am3_communication_reinitialisation(void);
void append_to_zip_fifo(AM3_Buffer* to_zip);
void add_time_measure(uint32_t id);

void append_to_send_control_fifoS(AM3_Buffer* to_send);
void append_to_send_control_fifoI(AM3_Buffer* to_send);
AM3_Buffer* get_communication_bufferS(bool force);
AM3_Buffer* get_communication_bufferI(bool force);
void am3_clear_communication_buffer(void);
bool is_receiving(AM3_Buffer* buffer);
bool is_transmitting(AM3_Buffer* buffer);

void append_to_send_control_fifoS(AM3_Buffer* to_send);
void append_to_send_control_fifoI(AM3_Buffer* to_send);


/**********************************************************************************************/
#endif /* AM3_COMMUNICATION_GENERIC_H_ */

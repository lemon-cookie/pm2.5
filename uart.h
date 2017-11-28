#ifndef __UART_H__
#define __UART_H__ 
#include "datatype.h"  
//UART0 for config

#define UART1_RX_BUFFER_LEN 200

extern UINT8 volatile uart1_rx_buffer[UART1_RX_BUFFER_LEN];
extern UINT8 volatile  *  uart1_rx_pointer;
extern UINT8 volatile *  uart1_get_pointer;
extern BOOL uart1_rx_isfull;
//extern UINT8 resl;

extern UINT8 Uart1_GetCell(void);
extern void Uart1_PutChar(UINT8 ch);
extern void UART1_Init(void);
extern void delayms(UINT32 ms);
extern void Clear_Uart1_Rx_Buffer(void);
extern void Uart1_Init(void);
extern void Uart1_PutCell(UINT8 ch);
extern void Uart1_PutArray(UINT8 *buf, UINT16 len);
extern UINT8 Uart1_GetCell(void);
extern UINT8 Uart1_GetCell_timeout(BOOL *flag, UINT32 timeout);



#endif
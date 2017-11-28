#include "iostm8s103f3.h"
#include "datatype.h"  
#include "uart.h"



UINT8 volatile uart1_rx_buffer[UART1_RX_BUFFER_LEN];
UINT8 volatile *  uart1_rx_pointer;
UINT8 volatile *  uart1_get_pointer;
BOOL uart1_rx_isfull;


void delayms(UINT32 ms)
{
    UINT16 i,j;
    for(i = 0; i < ms; i ++)
        for(j = 0; j < 189; j ++);
}



void Clear_Uart1_Rx_Buffer(void)
{
	
    UINT16 i;
    for (i=0; i<UART1_RX_BUFFER_LEN; i++)
    {
        uart1_rx_buffer[i] = 0;	
    }

    uart1_rx_pointer = uart1_rx_buffer;
    uart1_get_pointer = uart1_rx_buffer;

    uart1_rx_isfull = 0;	
}

void UART1_Init(void)
{

    UART1_CR3  = 0x00;
    //UART1_CR2  = 0x20;
    UART1_CR2  = 0x2e;
    UART1_CR1  = 0x00;
    UART1_BRR2 = 0x03;
    UART1_BRR1 = 0x68;
    Clear_Uart1_Rx_Buffer();
} 



void Uart1_PutChar(UINT8 ch)
{
    UART1_DR = ch;
    while(!(UART1_SR&0x40));
}

void Uart1_PutCell(UINT8 ch)
{
    Uart1_PutChar(ch);
}


void Uart1_PutArray(UINT8 *buf, UINT16 len)
{
	UINT16 i;
	//cli();
	for (i=0; i<len; i++)
	{       
            asm("sim");
            Uart1_PutChar(*(buf  + i));
            asm("rim");
	}
	//sei();
}

UINT8 Uart1_GetCell(void)
{
    UINT8 c;

    while((!uart1_rx_isfull) && (uart1_get_pointer == uart1_rx_pointer));        // wait until rx_buffer is not empty
      // cli(); 
    c = *uart1_get_pointer ++;
    if(uart1_get_pointer == uart1_rx_buffer + UART1_RX_BUFFER_LEN)
        uart1_get_pointer = uart1_rx_buffer;
        
    uart1_rx_isfull = 0;
    // sei();
    return c;
}

UINT8 Uart1_GetCell_timeout(BOOL *flag, UINT32 timeout)
{
    UINT8 c;
    UINT32 count = 0;

    //SetTimeOut_T0(timeout);
     while((!uart1_rx_isfull) && (uart1_get_pointer == uart1_rx_pointer))       // wait until rx_buffer is not empty
    {
        count++;
        delayms(1);
        if (count >= timeout)
        {
        count = 0;
        *flag = 0x01;
        return 0;				
        }
    }
   // cli();   
    c = *uart1_get_pointer ++;
    if(uart1_get_pointer == uart1_rx_buffer + UART1_RX_BUFFER_LEN)
        uart1_get_pointer = uart1_rx_buffer;
        
    uart1_rx_isfull = 0;

   // sei();

    return c;
}

//UINT8 Config_GetFrame(void)
//{
//    BOOL timeout_flag = 0;
//    UINT8 temp, len_1, len_2;
//    static UINT8 tempBuf[256];
//    UINT16 i, RecCount;
//    UINT16 FrameLength, sum1, sum2;
//    FrameLength = 0;
//    RecCount = 0;
//   // memset(tempBuf, 0x00, 256);
//    temp = Uart0_GetCell_timeout(&timeout_flag, 50);
//    
//    if (timeout_flag == 1)
//    {
//        //printf("Get Header timeout\r\n");
//        return 1;
//    }
//    if (temp == 0x7e)
//    {
//        tempBuf[RecCount] = temp;
//        RecCount ++;
//        temp = Uart0_GetCell_timeout(&timeout_flag, 500); //1ms  
//        
//        if (timeout_flag == 1)
//        {
//            //Clear_Uart0_Rx_Buffer();
//            return 1;
//        } 
//        tempBuf[RecCount] = temp;
//        RecCount ++;        
//        len_1 = temp; 
// 
//        temp = Uart0_GetCell_timeout(&timeout_flag, 500); //1ms  
//        
//        if (timeout_flag == 1)
//        {
//            //Clear_Uart0_Rx_Buffer();
//            return 1;
//        }    
//        len_2 = temp;   
//        tempBuf[RecCount] = temp;
//        RecCount ++;
//        FrameLength = (len_1&0x00ff)<<8 | len_2;    
//        if (FrameLength<9 || FrameLength>70) // shortest shake hand frame length == 17
//        {
//            printf("---->Config a error length\r\n");
//            Clear_Uart0_Rx_Buffer();
//            return 1;
//        }
//        for (i=0; i<(FrameLength-3); i++)
//        {
//            temp = Uart0_GetCell_timeout(&timeout_flag, 500);  //5*200us 1ms
//            if (timeout_flag == 1)
//            {
//                Clear_Uart0_Rx_Buffer();
//                return 1;
//            }	
//            tempBuf[RecCount] = temp;
//            RecCount ++;            
//                    
//        } 
//        sum1 = Check_Sum(tempBuf, FrameLength-2);
//        sum2 = (tempBuf[FrameLength-2]&0x00ff)<<8 | (tempBuf[FrameLength-1]&0x00ff);
//        
//        if (sum1 != sum2)
//        {
//            printf("---->COnfig CheckSum Error\r\n");
//            Clear_Uart0_Rx_Buffer();
//            return 1;
//        }
//        switch (tempBuf[5])
//        {
//            case ShakeHand:
//               Config_Status = ShakeHand;
//               Make_ShakeHand_Ack();
//               printf("----> Config_ShakeHand_ACK\r\n");
//            break;  
//       
//            case ReadConfig:
//               Config_Status = ReadConfig;
//               Make_ReadConfig_Ack();
//               printf("----> Config_ReadConfig_ACK\r\n");
//            break;  
//
//            case SetConfig:
//               Config_Status = SetConfig;
//               
//               memcpy(Current_Config.ParamBuffer, &tempBuf[7], 56);
//               Set_CurrentParameterToEeprom();
//               if (Read_ConfigInfoFormEEPROM()==0)
//               {
//                   Make_SetConfig_Ack(0x01);
//                   printf("----> Config_SetConfig_ACK: OK\r\n");
//               }   
//               else
//               {
//                   Make_SetConfig_Ack(0x00);
//                   printf("----> Config_SetConfig_ACK: Error\r\n");
//               }
//               Show_CurrentParameterInfo();
//            break;  
//            
//            default:
//               Config_Status = Undefine;
//                printf("Config_Have_No_This_Cmd\r\n");                   
//            break;
//        }       
//        Clear_Uart0_Rx_Buffer();
//        return 0;
//    }
//    else
//    {
//       printf("---->Not Config header\r\n");
//       Clear_Uart0_Rx_Buffer();
//	   wdog_feed();
//       return 1; 
//    }
//}
//UINT8 resl;
#pragma vector=UART1_R_RXNE_vector
__interrupt void UART1_IRQHandler(void)
{
    UINT8 resl;
    if(!uart1_rx_isfull)
    {
        resl = UART1_DR;//uart_getchar(SERIALCOM);//UART_D_REG(UARTx[SERIALCOM]);;
       // UART1_DR = resl;
        *uart1_rx_pointer ++ = resl;
        if(uart1_rx_pointer == uart1_rx_buffer + UART1_RX_BUFFER_LEN)
        uart1_rx_pointer = uart1_rx_buffer;
        if(uart1_rx_pointer == uart1_get_pointer)
        uart1_rx_isfull = 1;
    }
}

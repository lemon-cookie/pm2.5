#include "iostm8s103f3.h"
#include "datatype.h"
#include "uart.h"


void main() 
{     
  //unsigned char ch;
     // asm("sim"); 
   CLK_CKDIVR = 0x00;
   UART1_Init();  
    //UART1_CR2 |= 0x0e; 
       //asm("rim"); 

   while (1)   
  {  
        asm("sim"); 
        delayms(1000);
   Uart1_GetCell();

      UART1_DR =  *uart1_rx_pointer ++;
      //asm("rim"); 
     //unsigned int i;        
  } 
} 

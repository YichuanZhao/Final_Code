

#include <pic18.h>
#include <math.h>
#include "function.h"
#include        "lcd_portd.h"
#include "function.c"
#include        "lcd_portd.c"
// Global Variables

unsigned char ISD_Out(unsigned char DATA)
{
    unsigned char RESULT;
    unsigned char i;
    RA2=1;
    RESULT=0;
    for(i=0;i<8; i++)
    {
        if(DATA&1)
         {
            RA3=1;
         }
       else
         {
            RA3=0;
         }
        RA2=0;
        Wait_ms(1);
        RA2=1;
        Wait_ms(1);
  
        RESULT=RESULT>>1;
        if(RA4)
        {
          RESULT=RESULT+0X80;
        }
        DATA=DATA>>1;
    }
 return(RESULT);
}

void Set_Play(unsigned char m, unsigned char n )
{


 RA0=1;
 RA2=1;
 RA3=0;

 RA0=0;

 ISD_Out(0x01);
 ISD_Out(0x00);
 
 RA0=1;
 RA2=1;
 RA3=0;
 
 RA0=0;
 ISD_Out(0x04);
 ISD_Out(0x00);

 RA0=1; 
 RA2=1;
 RA3=0;
 
 RA0=0; 
 ISD_Out(0x80);
 ISD_Out(0x00);

 ISD_Out(m);
 ISD_Out(0x00);

 ISD_Out(m);
 ISD_Out(0x00);
 ISD_Out(0x00);
 Wait_ms(100);

 RA0=1;
 RA2=1;
 RA3=0;
 
 RA0=0; 
 ISD_Out(0x07);
 ISD_Out(0x00);



}

void main()
{

TRISA0=0;
TRISA5=1;
TRISA2=0;
TRISA3=0;
TRISA4=1;

while(1)
{

if(RA5==1)
{
Set_Play(0x1B,0x28);
}

}
}
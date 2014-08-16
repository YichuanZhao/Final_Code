

#include <pic18.h>
#include <math.h>
#include "function.h"

#include "function.c"

// Global Variables

unsigned int i;
void Digital_Up(void)
{
	RD0 = 0; 
	RD1 = 1;  
	for(i=0;i<2;i++){
		RD2 = 1; 
		Wait_ms(10);
		RD2 = 0;
		Wait_ms(10);
	}
	RD0 = 1;
}
 
void Digital_Dn(void)
{
	RD0 = 0;  
	RD1 = 0;  
	for(i=0;i<2;i++){
		RD2 = 1; 
		Wait_ms(10);
		RD2 = 0;
		Wait_ms(10);
	}
	RD0 = 1;
}

void main()
{

TRISC=0x00;
TRISB=0xFF;


RC0=0;

RC1=0;

RC3=1;
if(RB0==1)
{
RC2=1;
}
if(RB1==1)
{
RC2=0;
}

}
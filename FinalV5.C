
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% X(ECE)te Code %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include <pic18.h>
#include <math.h>
#include "function.h"
#include "function.c"
// Global Variables

	// IR Variables
unsigned char x, Break;
unsigned int IR_program[120];

	// RF Variables
unsigned int A1, A2, A3, A4, A5;
unsigned int A_1_8[16] = {1, 3, 7, 11, 13, 15, 17, 19, 21, 25, 27, 29, 33, 37, 41, 45};
unsigned int A1_ON[18] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53};
unsigned int A1_OFF[18] = {1, 3, 5, 9, 11, 13, 15, 17, 19, 23, 27, 29, 33, 37, 41, 45, 49, 53};
unsigned int A2_ON[18] = {1, 3, 5, 7, 11, 13, 15, 17, 19, 23, 27, 31, 33, 37, 41, 45, 49, 53};
unsigned int A2_OFF[18] ={1, 3, 5, 9, 13, 15, 17, 19, 21, 25, 29, 31, 33, 37, 41, 45, 49, 53};
unsigned int A3_ON[18] = {1, 3, 5, 7, 9, 13, 15, 17, 19, 23, 27, 31, 35, 37, 41, 45, 49, 53};
unsigned int A3_OFF[18] ={1, 3, 5, 9, 11, 15, 17, 19, 21, 25, 29, 31, 35, 37, 41, 45, 49, 53};
unsigned int A4_ON[18] = {1, 3, 5, 7, 11, 15, 17, 19, 21, 25, 29, 33, 35, 37, 41, 45, 49, 53};
unsigned int A4_OFF[18] ={1, 3, 5, 9, 13, 17, 19, 21, 23, 27, 31, 33, 35, 37, 41, 45, 49, 53};
unsigned int A5_ON[18] = {1, 3, 7, 9, 11, 13, 15, 17, 19, 23, 25, 29, 33, 37, 41, 45, 49, 53};
unsigned int A5_OFF[18] ={1, 3, 7, 11, 13, 15, 17, 19, 21, 25, 27, 29, 33, 37, 41, 45, 49, 53};
unsigned int RF_signal_toggle[18];
unsigned int y, f;
unsigned int Time;

	// Enclosure LED
unsigned int Color, RED, BLUE, GREEN;
unsigned int Count;
unsigned char R, B, G, Dnr, Dnb, Dng;
unsigned char soothing, glower, LED;

	// Others
unsigned int i, Module;
unsigned char z0, z1, Move, Menu, Sub, Cat, Com, DO, Program;
unsigned long T;
unsigned char Volume;

unsigned char ISD_Out(unsigned char DATA);
void Set_Play(unsigned int Start, unsigned int End);




//%%%%%%%%%%%%%%%%%%%%%%%% Interrupts %%%%%%%%%%%%%%%%%%%%%%%%

void interrupt IntServe(void){

// Timer0
if (TMR0IF) // IR Program & RF Out
{
	Time=Time+1;
	if(TMR1>65000){Break=1; IR_program[119]=x;} // Break after 13 ms of no new edges
	TMR0 = -3150;
	TMR0IF = 0;
}

// Timer3
if (TMR3IF) // Enclosure LEDs
{
	Color=(Color+1)%10; 
	if (RED>Color)RB5=1;   else RB5=0;
   	if (GREEN>Color)RB6=1; else RB6=0;
	if (BLUE>Color)RB7=1;  else RB7=0;	
	TMR3IF = 0;
	TMR3 = -2500; 
}

// Capture edges for IR
if (CCP2IF)
{
	if(RC7){
		IR_program[x] = TMR1;
		TMR1=0;
		x = x + 1;
	}
	CCP2IF = 0;
}
if (CCP1IF)
{
	if(RC7){
		IR_program[x] = TMR1;
		TMR1=0;
		x = x + 1;
	}
	CCP1IF = 0;
}

// Int Interrupts
if (INT0IF) // Right Head Button
{
   Wait_ms(15);  // Debouncer
   if(RB0){
      T=0;
      do{T=T+1;}while(T<50010 && RB0);
	  if(T>50000){Set_Play(0x03B0, 0x03B1);}
	  do{for(i=0;i<10;i++);}while(RB0);
   }
   if (T>50000){z0=2;}
   else z0=1;
   INT0IF = 0;
}
if (INT1IF)  // Left Head Button
{
   Wait_ms(15);  // Debouncer
   if(RB1){
      T=0;
      do{T=T+1;}while(T<50010 && RB1);
	  if(T>50000){Set_Play(0x03B0, 0x03B1);}
	  do{for(i=0;i<10;i++);}while(RB1);
   }
   if (T>50000){z1=2;}
   else z1=1;
   INT1IF = 0;
}

}





//%%%%%%%%%%%%%%%%%%%%%%%% Subroutines %%%%%%%%%%%%%%%%%%%%%%%%

// EEPROM
void M_Write_Enable(void)
{
   SSPIF = 0;
   SSPBUF = 0x06; 
   while(!SSPIF);
   }
 
void M_Write_Address(unsigned long int Address)
{
   SSPIF = 0;
   SSPBUF = 0x02;           while(!SSPIF);
   SSPIF = 0;
   SSPBUF = Address >> 16;  while(!SSPIF);
   SSPIF = 0;
   SSPBUF = Address >> 8;   while(!SSPIF);
   SSPIF = 0;
   SSPBUF = Address;        while(!SSPIF);
   }

void M_Read_Address(unsigned long int Address)
{
   SSPIF = 0;
   SSPBUF = 0x03;          while(!SSPIF);
   SSPIF = 0;
   SSPBUF = Address >> 16; while(!SSPIF);
   SSPIF = 0;
   SSPBUF = Address >> 8;  while(!SSPIF);
   SSPIF = 0;
   SSPBUF = Address;       while(!SSPIF);
   }

void M_Write_Data(unsigned char Data)
{
   SSPIF = 0;
   SSPBUF = Data;  while(!SSPIF);
   }

unsigned int M_Read_Data(void)
{
   unsigned int Result;
   SSPIF = 0;
   SSPBUF = 0;    while(!SSPIF);
   Result = SSPBUF;
   SSPIF = 0;
   SSPBUF = 0;    while(!SSPIF);
   Result = (Result << 8) + SSPBUF;
   return(Result);
   }

void M_Write_0(unsigned long int Address, unsigned int Data)
{
    RC6 = 1;
    Wait_ms(5);
    RC6 = 0; 
    M_Write_Enable();  
    RC6 = 1;
    Wait_ms(5);  
    RC6 = 0; 
    M_Write_Address(Address);
    M_Write_Data(Data>>8);
    M_Write_Data(Data);
    RC6 = 1;
    }

void Memory_PowerDown(void)
{
   RC6 = 0;
   M_Write_Data(0xB9);
   RC6 = 1;
}

void Memory_PowerUp(void)
{
   RC6 = 0;
   M_Write_Data(0xAB);
   M_Write_Data(0x00);
   M_Write_Data(0x00);
   M_Write_Data(0x00);
   M_Write_Data(0x00);
   RC6 = 1;
}


// Infrared
void IR_program_mode(void)
{
	for(i=0;i<4000;i++);
	for(i=0;i<120;i++){IR_program[i]=0;}
	RC7 = 1;
	x=0; 
	Break=0;
	do{TMR1=10000;}while((RC1));
	do {for(i=0;i<5;i++);}while(Break==0);
	RC7 = 0;
	IR_program[1]=IR_program[1]-10100;
	Break=0;
}

void IR_Program(unsigned long int Address)
{
	TMR3IE = 0;
	TMR3ON = 0;
	IR_program_mode();
    RC6 = 1;
    Wait_ms(100);
    RC6 = 0;
	TMR0IE = 0;
	TMR0ON = 0;
    M_Write_Enable();
    RC6 = 1;
    RC6 = 0;
    for(i=0;i<120;i++){M_Write_0(((240*Address)+2*i),   IR_program[i]);}
	TMR3IE = 1;
	TMR3ON = 1;
	TMR0IE = 1;
	TMR0ON = 1;
}

void IR_out(void)
{
	RA1 = 0;
	x = 0;
	TMR1 = 0;
	do{if(TMR1>=IR_program[x]){RA1=!RA1; x=x+1; TMR1=0;}}while(x<IR_program[119]);
	RA1=0;
}	

void IR_output(unsigned long int Address) 
{ 
	TMR3IE = 0;
	TMR3ON = 0;
	TMR0IE = 0;
	TMR0ON = 0;
	for(i=0;i<120;i++){IR_program[i]=0;}
    RC6 = 0;
    M_Read_Address(Address*240);
	for(i=0;i<120;i++){IR_program[i]=M_Read_Data();}
	RC6=1;
	IR_out();
	TMR3IE = 1;
	TMR3ON = 1;
	TMR0IE = 1;
	TMR0ON = 1;
}


// RF
void wakeup(void)
{
	Time = 0;
	do{
	RC0 = 1;
	}while(Time < 17);
	do{
	RC0 = 0;
	}while(Time < 24);
}

void RF_out(void)
{
	x = 0;
	Time = 0;
	do{
	if(Time == A_1_8[x]){
	RC0 = 1;
	y = Time;
	f = 1;
	}
	if((y != Time) && (f == 1)){
	x = x + 1;
	f = 0;
	}
	if(Time != A_1_8[x]){RC0 = 0;}
	}while(Time < 47);
	x = 0;
	Time = 0;
	do{
	if(Time == RF_signal_toggle[x]){
	RC0 = 1;
	y = Time;
	f = 1;
	}
	if((y != Time) && (f == 1)){
	x = x + 1;
	f = 0;
	}
	if(Time != RF_signal_toggle[x]){RC0 = 0;}
	}while(x<17);
	RC0 = 0;
}

void RF_output(unsigned char A, unsigned int Number_ON[18], unsigned int Number_OFF[18])
{
	if(A == 1){
	for(i=0;i<18;i++){RF_signal_toggle[i] = A2_ON[i];}
	}
	if(A == 0){
	for(i=0;i<18;i++){RF_signal_toggle[i] = A2_OFF[i];}
	}
	TMR3IE = 0;
	TMR3ON = 0;
	wakeup();
	RF_out();
	for(i=0;i<20300;i++);
	wakeup();
	RF_out();
	for(i=0;i<20300;i++);
	wakeup();
	RF_out();
	for(i=0;i<20300;i++);
	wakeup();
	RF_out();
	for(i=0;i<20300;i++);
	wakeup();
	RF_out();
	for(i=0;i<20300;i++);
	wakeup();
	RF_out();
	for(i=0;i<20300;i++);
	TMR3IE = 1;
	TMR3ON = 1;
}


// LEDs
void Glower(void)
{
   for(i=0;i<40000;i++);       // Speed
   Count=(Count+1)%30;
   if(Count<10)     {RED=Count; GREEN=10-Count; BLUE=0;}
   else if(Count<20){RED=10-Count; GREEN=0; BLUE=Count;}
   else             {RED=0; BLUE=10-Count; GREEN=Count;}
}

void Soothing (void)
{
   for(i=0;i<40000;i++);       // Speed
   if((R==0&G==0&B==0)|(R==0&G==1&B==1)){RED=RED+1;}
   if(R==1&G==0&B==0){GREEN=GREEN+1;}
   if(R==0&G==1&B==0){BLUE=BLUE+1;}
   if((R==1&G==1&B==0)|(R==1&G==0&B==1)){RED=RED-1;}
   if(R==1&G==1&B==1){GREEN=GREEN-1;}
   if(R==0&G==0&B==1){BLUE=BLUE-1;}
   if(RED==10){R=1;} 
   if(GREEN==10){G=1;}
   if(BLUE==10){B=1;}
   if(RED==0){R=0;}
   if(GREEN==0){G=0;}
   if(BLUE==0){B=0;}
}


// Voice Chip
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

void Set_Play(unsigned int Start, unsigned int End)
{

   TRISB0 = 0;
   TRISB1 = 0;

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

 ISD_Out(Start);
 if      ((0xFF00 & Start)==0x0000){ISD_Out(0x00);}
 else if ((0xFF00 & Start)==0x0100){ISD_Out(0x01);}
 else if ((0xFF00 & Start)==0x0200){ISD_Out(0x02);}
 else if ((0xFF00 & Start)==0x0300){ISD_Out(0x03);}

 ISD_Out(End);
 if      ((0xFF00 & End)==0x0000){ISD_Out(0x00);}
 else if ((0xFF00 & End)==0x0100){ISD_Out(0x01);}
 else if ((0xFF00 & End)==0x0200){ISD_Out(0x02);}
 else if ((0xFF00 & End)==0x0300){ISD_Out(0x03);}

 ISD_Out(0x00);
 Wait_ms(100);

 RA0=1;
 RA2=1;
 RA3=0;
 
 RA0=0; 
 ISD_Out(0x07);
 ISD_Out(0x00);
 RA0=1; 

   Wait_ms(((End-Start)*126));
   TRISB0 = 1;
   TRISB1 = 1;
}


void ISD_Vol(unsigned char Volume) 
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
 ISD_Out(0x65);
 ISD_Out(Volume);
 ISD_Out(0x04);


 RA0=1;
 RA2=1;
 RA3=0;
 
 RA0=0; 
 ISD_Out(0x07);
 ISD_Out(0x00);
 RA0=1; 
}


//Radio
	// Digital Resistor
void Digital_Up(void)
{
	RD6 = 0; 
	RD4 = 1;  
	RD5 = 0;
	Wait_ms(10);
	for(i=0;i<8;i++){
		RD5 = 1; 
		Wait_ms(10);
		RD5 = 0;
		Wait_ms(10);
	}
	RD6 = 1;
}
 
void Digital_Dn(void)
{
	RD6 = 0;  
	RD4 = 0;  
	RD5 = 0;
	Wait_ms(10);
	for(i=0;i<8;i++){
		RD5 = 1; 
		Wait_ms(10);
		RD5 = 0;
		Wait_ms(10);
	}
	RD6 = 1;
}







//%%%%%%%%%%%%%%%%%%%%%%%% Main %%%%%%%%%%%%%%%%%%%%%%%%
void main(void)
{

// Initialize the SPI port
   SSPIE = 0;
   TRISC5 = 0;
   TRISC4 = 1;
   TRISC3 = 0;
   TRISC6 = 0;
   SSPSTAT = 0x40;
   SSPCON1 = 0x22;

// Turn on Timer0
   T0CON = 0x88;
   TMR0ON = 1;
   TMR0IE = 1;
   TMR0IP = 1;
   TMR0 = -3150;

// Turn on TMR1
   TMR1ON = 1;

// Turn on Timer3
   T3CON = 0x81;
   TMR3ON = 1;
   TMR3IE = 1;
   TMR3IP = 1;
   TMR3 = -2500;

// Capture on RC1 for rising edge
   TRISC1 = 1; 
   CCP2CON = 0x05;
   CCP2IE = 1;
   CCP2IP=1;

 // Capture on RC2 for falling edge
   TRISC2 = 1;
   CCP1CON = 0x04; 
   CCP1IE = 1;
   CCP1IP=1;

// initialize INT0 interrupts
   TRISB0 = 1;
   INT0IE = 1;
   INTEDG0 = 1; // rising edges

// initialize INT0 interrupts
   INT1IE = 1;
   INT1IP = 1;
   TRISB1 = 1;
   INTEDG1 = 1; // rising edges

   PEIE = 1;
   GIE = 1;
   ADCON1 = 0x0F;


// PORTE assignments
   TRISE0 = 0; // Power to Radio
   TRISE1 = 0;

// PORTD assignments
   TRISD0 = 0;
   TRISD1 = 0;
   TRISD2 = 0;
   TRISD3 = 0;
   TRISD4 = 0; // U/D for Digital Resistor
   TRISD5 = 0; // Inc for Digital Resistor
   TRISD6 = 0; // CS for Digital Resistor
   TRISD7 = 0;

// PORTC assignments
   TRISC0 = 0; // RF Signal Output
   TRISC1 = 1; // Capture IR edges
   TRISC2 = 1; // Capture IR edges
   TRISC3 = 0; // EEPROM SCK
   TRISC4 = 1; // EEPROM SO
   TRISC5 = 0; // EEPROM SI
   TRISC6 = 0; // EEPROM CS
   TRISC7 = 0; // LED Program Indicator Light

// PORTB assignments
   TRISB0 = 1; // Right Button (Forward:  single press & Enter:  long press)
   TRISB1 = 1; // Left Button  (Backward: single press & Cancel: long press)
   TRISB2 = 1; // Program Button (Black)
   TRISB3 = 0;
   TRISB4 = 0;
   TRISB5 = 0; // Enclosure Red   LED
   TRISB6 = 0; // Enclosure Green LED
   TRISB7 = 0; // Enclosure Blue  LED

// PORTA assignments
   TRISA0 = 0; // Voice Chip
   TRISA1 = 0; // IR Singal Output
   TRISA2 = 0; // Voice Chip
   TRISA3 = 0; // Voice Chip
   TRISA4 = 0; // Radio Reset
   TRISA5 = 0; // Radio Scan




// Assign values to variables
   Time = 0;
   x = 0;
   Color = 0;
   RED = 1;
   GREEN = 0;
   BLUE = 0;
   Count = 0;
   R=0;
   B=0;
   G=0;
   Dnr=0;
   Dnb=0;
   Dng=0;
   soothing = 0;
   glower = 0;
   z0=0;
   z1=2;
   Move=0;
   Menu=1;
   Sub=0;
   Cat=0;
   Com=0;
   Module = 0x0000;
   LED = 1;
   DO=0;
   A1=0;
   A2=0;
   A3=0;
   A4=0;
   A5=0;


   Volume = 0;
   ISD_Vol(Volume);


// Ports
   RE0 = 0;
   RE1 = 0;
   RA1 = 0;
   RC7 = 0;
   RC0 = 0;
   RA5 = 0;
   RA4 = 0;


RD6 = 0;

while(1) {

if      (LED==0){TMR3ON=0; TMR3IE=0; RB7=0; RB6=0; RB5=0;}
else if (LED==1){TMR3ON=1; TMR3IE=1; Soothing();}
else if (LED==2){TMR3ON=1; TMR3IE=1; Glower();} 
else if (LED==3){TMR3ON=1; TMR3IE=1;} 



//%%%%%%%%%%%%%%%%%%%%% Control States %%%%%%%%%%%%%%%%%%%%%
if((z0!=0)|(z1!=0)){
Move=1;
if(Menu==1){
	if(z0==1)	  {Module=(Module+0x1000); z0=0;
		switch(Module){
		case 0x6000: Module=0x0000; break;}
	}
	else if(z1==1){Module=(Module-0x1000); z1=0;
		switch(Module){
		case 0xF000: Module=0x5000; break;}
	}	
	else if(z0==2){z0=0; Menu=0; Sub=1;
		if(Module==0x2000){RA4=1;}
	}
	else if(z1==2){z1=0;}
}
else if(Sub==1){
	if(z0==1)	  {Module=(Module+0x0100); z0=0;
		switch(Module){
		case 0x0600: Module=0x0000; break;
		case 0x1500: Module=0x1000; break;
		case 0x2400: Module=0x2000; break;
		case 0x3600: Module=0x3000; break;
		case 0x4200: Module=0x4000; break;
		case 0x5500: Module=0x5000; break;}
	}
	else if(z1==1){Module=(Module-0x0100); z1=0;
		switch(Module){
		case 0xFF00: Module=0x0500; break;
		case 0x0F00: Module=0x1400; break;
		case 0x1F00: Module=0x2300; break;
		case 0x2F00: Module=0x3500; break;
		case 0x3F00: Module=0x4100; break;
		case 0x4F00: Module=0x5400; break;}
	}
	else if(z0==2){z0=0;
		switch(Module){
		case 0x0000: DO=1; break;
		case 0x0100: DO=1; break;
		case 0x0400: DO=1; break;
		case 0x0500: DO=1; break;
		case 0x1000: DO=1; break;
		case 0x1100: DO=1; break;
		case 0x1200: DO=1; break;
		case 0x1300: DO=1; break;
		case 0x1400: DO=1; break;
		case 0x2000: DO=1; break;
		case 0x2100: DO=1; break;
		case 0x2200: DO=1; break;
		case 0x2300: DO=1; break;
		case 0x3000: DO=1; break;
		case 0x3100: DO=1; break;
		case 0x3200: DO=1; break;
		case 0x3300: DO=1; break;
		case 0x3400: DO=1; break;
		case 0x3500: DO=1; break;
		case 0x4000: DO=1; break;
		case 0x4100: DO=1; break;
		case 0x5000: DO=1; break;
		case 0x5100: DO=1; break;
		case 0x5200: DO=1; break;
		case 0x5300: DO=1; break;
		case 0x5400: DO=1; break;
		default: Sub=0; Cat=1; break;
		}
	}
	else if(z1==2){z1=0; Sub=0; Menu=1; Module=(Module & 0xF000);
		if(Module==0x2000){RA4=0;}
	}
}
else if(Cat==1){
	if(z0==1)	  {Module=(Module+0x0010); z0=0;
		switch(Module){
		case 0x0240: Module=0x0200; break;
		case 0x03A0: Module=0x0300; break;}
	}
	else if(z1==1){Module=(Module-0x0010); z1=0;
		switch(Module){
		case 0x01F0: Module=0x0230; break;
		case 0x02F0: Module=0x0390; break;}
	}
	else if(z0==2){z0=0;
		switch(Module){
		case 0x0300: DO=1; break;
		case 0x0310: DO=1; break;
		case 0x0320: DO=1; break;
		case 0x0330: DO=1; break;
		case 0x0340: DO=1; break;
		case 0x0350: DO=1; break;
		case 0x0360: DO=1; break;
		case 0x0370: DO=1; break;
		case 0x0380: DO=1; break;
		case 0x0390: DO=1; break;
		default: Cat=0; Com=1; break;
		}
	}
	else if(z1==2){z1=0; Cat=0; Sub=1; Module=(Module & 0xFF00);}
}
else if(Com==1){
	if(z0==1)	  {Module=(Module+0x0001); z0=0;
		switch(Module & 0x020F){
		case 0x0205: Module=Module & 0x02F0; break;}
	}
	else if(z1==1){Module=(Module-0x0001); z1=0;
		switch(Module & 0x000F){
		case 0x000F: Module=((Module+0x0010) & 0x00F0)+0x0204; break;}
	}
	else if(z0==2){z0=0; DO=1;}
	else if(z1==2){z1=0; Com=0; Cat=1; Module=(Module & 0xFFF0);}
}
}



//%%%%%%%%%%%%%%%%%%%%% Output voice/function %%%%%%%%%%%%%%%%%%%%%
if(RB2){Move=1; Program=1; DO=0;}

if(Move==1){
  Move=0;
    if(Menu==1){
	switch(Module){
		case 0x0000: // TV
			if(Program==0){Set_Play(0x0015, 0x001A);} 
		break;
		case 0x1000: // X-10
			if(Program==0){Set_Play(0x0134, 0x0139);}
		break;
		case 0x2000: // Radio
			if(Program==0){Set_Play(0x017F, 0x0185);}
			break;
		case 0x3000: // LED Control
			if(Program==0){Set_Play(0x01B4, 0x01BC);}
			break;
		case 0x4000: // Volume
			if(Program==0){Set_Play(0x01F4, 0x01FB);}
			break;
		case 0x5000: // Comments
			if(Program==0){Set_Play(0x0214, 0x021A);}
			break;
		}
	}

	else if(Sub==1){
	switch (Module){
// TV
		case 0x0000:													 
			if((DO==0) && (Program==0)){Set_Play(0x001E, 0x0027);}
			if(DO==1){DO=0; IR_output(3);}
			if(Program==1){Program=0; IR_Program(3);}
		break;
		case 0x0100: 
			if((DO==0) && (Program==0)){Set_Play(0x0029, 0x0037);}
			if(DO==1){DO=0; IR_output(1);} 
			if(Program==1){Program=0; IR_Program(1);}
		break;
		case 0x0200: 
			if(Program==0){Set_Play(0x003A, 0x0049);}
		break;
		case 0x0300:  
			if(Program==0){Set_Play(0x007F, 0x0086);}
		break;
		case 0x0400:  
			if((DO==0) && (Program==0)){Set_Play(0x0107, 0x0113);}
			if(DO==1){DO=0; IR_output(2);}
			if(Program==1){Program=0; IR_Program(2);}
		break;
		case 0x0500: 
			if((DO==0) && (Program==0)){Set_Play(0x011E, 0x0129);}
			if(DO==1){DO=0; IR_output(3);}
			if(Program==1){Program=0; IR_Program(3);}
		break;

// X-10
		case 0x1000: 														
			if((DO==0) && (Program==0)){Set_Play(0x0145, 0x014B);}
			if(DO==1){DO=0; RF_output(A1, A1_ON[18], A1_OFF[18]); A1=(A1+1)%2;} 
		break;
		case 0x1100: 
			if((DO==0) && (Program==0)){Set_Play(0x014F, 0x0155);}
			if(DO==1){DO=0; RF_output(A2, A2_ON[18], A2_OFF[18]); A2=(A2+1)%2;} 
		break;
		case 0x1200: 	
			if((DO==0) && (Program==0)){Set_Play(0x015C, 0x0161);}		
			if(DO==1){DO=0; RF_output(A3, A3_ON[18], A3_OFF[18]); A3=(A3+1)%2;}
		break;
		case 0x1300:  
			if((DO==0) && (Program==0)){Set_Play(0x0168, 0x016D);}
			if(DO==1){DO=0; RF_output(A4, A4_ON[18], A4_OFF[18]); A4=(A4+1)%2;}
		break;
		case 0x1400:
			if((DO==0) && (Program==0)){Set_Play(0x0172, 0x0178);}
			if(DO==1){DO=0; RF_output(A5, A5_ON[18], A5_OFF[18]); A5=(A5+1)%2;} 
		break;

// Radio
		case 0x2000:  	
			if((DO==0) && (Program==0)){
			RA4=0; Wait_ms(10);												
			Set_Play(0x019B, 0x01A1);
			RA4=1; Wait_ms(10);
			}
			if(DO==1){DO=0; RA5=1; Wait_ms(25); RA5=0;}
		break;
		case 0x2100:
			if((DO==0) && (Program==0)){
			RA4=0; Wait_ms(10);
			Set_Play(0x01A8, 0x01AC);
			RA4=1; Wait_ms(10);
			}
			if(DO==1){DO=0; RE0=1; Wait_ms(25); RE0=0;}
		break;
		case 0x2200: 
			if((DO==0) && (Program==0)){
			RA4=0; Wait_ms(10);
			Set_Play(0x0225, 0x022C);
			RA4=1; Wait_ms(10);
			}
			if(DO==1){DO=0; Digital_Up();}
		break;
		case 0x2300: 
			if((DO==0) && (Program==0)){
			RA4=0; Wait_ms(10);
			Set_Play(0x0230, 0x0238);
			RA4=1; Wait_ms(10);
			}
			if(DO==1){DO=0; Digital_Dn();}
		break;


// LED Control
		case 0x3000: 														
			if((DO==0) && (Program==0)){Set_Play(0x0238, 0x0242);}
			if(DO==1){DO=0; LED = 0;}
			break;
		case 0x3100:
			if((DO==0) && (Program==0)){Set_Play(0x01D7, 0x01DE);}
			if(DO==1){DO=0; LED = 1;} 
		break;
		case 0x3200:
			if((DO==0) && (Program==0)){Set_Play(0x01E4, 0x01EB);}
			if(DO==1){DO=0; LED = 2;} 
		break;
		case 0x3300: 														
			if((DO==0) && (Program==0)){Set_Play(0x0238, 0x0242);}
			if(DO==1){DO=0; LED=3; BLUE=5; GREEN=5;}
			break;
		case 0x3400:
			if((DO==0) && (Program==0)){Set_Play(0x01D7, 0x01DE);}
			if(DO==1){DO=0; LED=3; BLUE=5; RED=5;} 
		break;
		case 0x3500:
			if((DO==0) && (Program==0)){Set_Play(0x01E4, 0x01EB);}
			if(DO==1){DO=0; LED=3; GREEN=5;} 
		break;

// Voice Volume
		case 0x4000: 														
			if((DO==0) && (Program==0)){Set_Play(0x0225, 0x022C);}
			if(DO==1){DO=0; if(Volume>0){Volume=Volume-1;} ISD_Vol(Volume);} 
		break;
		case 0x4100:
			if((DO==0) && (Program==0)){Set_Play(0x0230, 0x0238);}
			if(DO==1){DO=0; if(Volume<7){Volume=Volume+1;} ISD_Vol(Volume);} 
		break;

// Comments
		case 0x5000: 														 
			if((DO==0) && (Program==0)){Set_Play(0x024B, 0x0252);}
			if(DO==1){DO=0; Set_Play(0x0, 0x0);} 
		break;
		case 0x5100: 
			if((DO==0) && (Program==0)){Set_Play(0x0252, 0x025C);}
			if(DO==1){DO=0; Set_Play(0x0, 0x0);}  
		break;
		case 0x5200: 
			if((DO==0) && (Program==0)){Set_Play(0x026E, 0x0276);}
			if(DO==1){DO=0; Set_Play(0x0, 0x0);} 
		break;
		case 0x5300: 
			if((DO==0) && (Program==0)){Set_Play(0x027A, 0x0283);}
			if(DO==1){DO=0; Set_Play(0x0, 0x0);}  
		break;
		case 0x5400:
			if((DO==0) && (Program==0)){Set_Play(0x0285, 0x0290);}
			if(DO==1){DO=0; Set_Play(0x0, 0x0);} 
		break;
	}
	}

	else if(Cat==1){
	switch (Module){

// Channel Categories
		case 0x0200: 														 
			if(Program==0){Set_Play(0x004E, 0x0053);}
		break;
		case 0x0210:  
			if(Program==0){Set_Play(0x0057, 0x005E);}
		break;
		case 0x0220: 
			if(Program==0){Set_Play(0x0061, 0x0068);}
		break;
		case 0x0230: 
			if(Program==0){Set_Play(0x006B, 0x0072);}
		break;

// Channel Numbers
		case 0x0300:  														
			if((DO==0) && (Program==0)){Set_Play(0x0087, 0x0091);}
			if(DO==1){DO=0; IR_output(4);}
			if(Program==1){Program=0; IR_Program(4);}
		break;
		case 0x0310:
			if((DO==0) && (Program==0)){Set_Play(0x0091, 0x009B);}
			if(DO==1){DO=0; IR_output(5);}
			if(Program==1){Program=0; IR_Program(5);}
		break;
		case 0x0320: 
			if((DO==0) && (Program==0)){Set_Play(0x009B, 0x00A5);}
			if(DO==1){DO=0; IR_output(6);}
			if(Program==1){Program=0; IR_Program(6);}
		break;
		case 0x0330:
			if((DO==0) && (Program==0)){Set_Play(0x00A5, 0x00AF);}
			if(DO==1){DO=0; IR_output(7);} 
			if(Program==1){Program=0; IR_Program(7);}
		break;
		case 0x0340: 
			if((DO==0) && (Program==0)){Set_Play(0x00AF, 0x00B9);}
			if(DO==1){DO=0; IR_output(8);}
			if(Program==1){Program=0; IR_Program(8);}
		break;
		case 0x0350:
			if((DO==0) && (Program==0)){Set_Play(0x00B9, 0x00C3);}
			if(DO==1){DO=0; IR_output(9);} 
			if(Program==1){Program=0; IR_Program(9);}
		break;
		case 0x0360: 
			if((DO==0) && (Program==0)){Set_Play(0x00C3, 0x00CD);}
			if(DO==1){DO=0; IR_output(10);} 
			if(Program==1){Program=0; IR_Program(10);}
		break;
		case 0x0370:
			if((DO==0) && (Program==0)){Set_Play(0x00CD, 0x00D7);}
			if(DO==1){DO=0; IR_output(11);}
			if(Program==1){Program=0; IR_Program(11);} 
		break;
		case 0x0380:  
			if((DO==0) && (Program==0)){Set_Play(0x00D7, 0x00E1);}
			if(DO==1){DO=0; IR_output(12);}
			if(Program==1){Program=0; IR_Program(12);}
		break;
		case 0x0390: 
			if((DO==0) && (Program==0)){Set_Play(0x00E1, 0x00EB);}
			if(DO==1){DO=0; IR_output(13);}
			if(Program==1){Program=0; IR_Program(13);}
		break;
	}
	}

	else if(Com==1){
	switch (Module){	

// Sports
		case 0x0200: 														
			if((DO==0) && (Program==0)){Set_Play(0x0298, 0x029F);}
			if(DO==1){
				DO=0; 
				IR_output(10); 
				for(i=0;i<10000;i++); 
				IR_output(6); 
				for(i=0;i<10000;i++); 
				IR_output(4);
			} 
		break;
		case 0x0201: 
			if((DO==0) && (Program==0)){Set_Play(0x02A3, 0x02AC);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<10000;i++); 
				IR_output(6); 
				for(i=0;i<10000;i++); 
				IR_output(5);
			}
		break;
		case 0x0202: 
			if((DO==0) && (Program==0)){Set_Play(0x02B5, 0x02BF);}
			if(DO==1){
				DO=0;
//				IR_output(4);
//				for(i=0;i<10000;i++); 
				IR_output(5); 
				for(i=0;i<10000;i++); 
				IR_output(4);
			}
		break;
		case 0x0203: 
			if((DO==0) && (Program==0)){Set_Play(0x02C7, 0x02CF);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<10000;i++); 
				IR_output(6); 
				for(i=0;i<10000;i++); 
				IR_output(6);
			}
		break;
		case 0x0204:
			if((DO==0) && (Program==0)){Set_Play(0x02D2, 0x02D9);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<10000;i++); 
				IR_output(13); 
				for(i=0;i<10000;i++); 
				IR_output(4);
			}
		break;

// Cartoons
		case 0x0210:  														
			if((DO==0) && (Program==0)){Set_Play(0x02E3, 0x02EC);}
			if(DO==1){
				DO=0;
				IR_output(5); 
				for(i=0;i<10000;i++); 
				IR_output(4); 
				for(i=0;i<10000;i++); 
				IR_output(10);
			}
		break;
		case 0x0211:
			if((DO==0) && (Program==0)){Set_Play(0x02F0, 0x02F6);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<10000;i++); 
				IR_output(8); 
				for(i=0;i<10000;i++); 
				IR_output(6);
			} 
		break;
		case 0x0212: 
			if((DO==0) && (Program==0)){Set_Play(0x02FA, 0x0303);}
			if(DO==1){
				DO=0;
				IR_output(5); 
				for(i=0;i<10000;i++); 
				IR_output(4); 
				for(i=0;i<10000;i++); 
				IR_output(12);
			}
		break;
		case 0x0213:
			if((DO==0) && (Program==0)){Set_Play(0x0303, 0x0311);} 
			if(DO==1){
				DO=0;
				IR_output(5); 
				for(i=0;i<10000;i++); 
				IR_output(4); 
				for(i=0;i<10000;i++); 
				IR_output(11);
			}
		break;
		case 0x0214:
			if((DO==0) && (Program==0)){Set_Play(0x0315, 0x031A);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<10000;i++); 
				IR_output(7); 
				for(i=0;i<10000;i++); 
				IR_output(11);
			}
		break;
// Outdoors
		case 0x0220:  														
			if((DO==0) && (Program==0)){Set_Play(0x0320, 0x0326);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<10000;i++); 
				IR_output(11); 
				for(i=0;i<10000;i++); 
				IR_output(6);
			}
		break;
		case 0x0221:
			if((DO==0) && (Program==0)){Set_Play(0x032B, 0x0333);}
			if(DO==1){
				DO=0;
//				IR_output(4);
//				for(i=0;i<10000;i++); 
				IR_output(11); 
				for(i=0;i<10000;i++); 
				IR_output(6);
			} 
		break;
		case 0x0222:  
			if((DO==0) && (Program==0)){Set_Play(0x0336, 0x033D);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<10000;i++); 
				IR_output(9); 
				for(i=0;i<10000;i++); 
				IR_output(6);
			}
		break;
		case 0x0223:
			if((DO==0) && (Program==0)){Set_Play(0x0346, 0x034D);} 
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<10000;i++); 
				IR_output(9); 
				for(i=0;i<10000;i++); 
				IR_output(9);
			}
		break;
		case 0x0224:
			if((DO==0) && (Program==0)){Set_Play(0x0350, 0x0357);}
			if(DO==1){
				DO=0;
//				IR_output(4);
//				for(i=0;i<10000;i++); 
				IR_output(7); 
				for(i=0;i<10000;i++); 
				IR_output(9);
			}
		break;
// Local
		case 0x0230:  														
			if((DO==0) && (Program==0)){Set_Play(0x0360, 0x0367);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<10000;i++); 
				IR_output(4); 
				for(i=0;i<10000;i++); 
				IR_output(8);
			}
		break;
		case 0x0231:
			if((DO==0) && (Program==0)){Set_Play(0x036C, 0x0374);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<10000;i++); 
				IR_output(4); 
				for(i=0;i<10000;i++); 
				IR_output(11);
			} 
		break;
		case 0x0232:  
			if((DO==0) && (Program==0)){Set_Play(0x0378, 0x0382);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<10000;i++); 
				IR_output(4); 
				for(i=0;i<10000;i++); 
				IR_output(12);
			}
		break;
		case 0x0233:
			if((DO==0) && (Program==0)){Set_Play(0x0384, 0x0392);} 
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<10000;i++); 
				IR_output(5); 
				for(i=0;i<10000;i++); 
				IR_output(5);
			}
		break;
		case 0x0234:
			if((DO==0) && (Program==0)){Set_Play(0x0396, 0x03A2);}
			if(DO==1){
				DO=0;
//				IR_output(4);
//				for(i=0;i<10000;i++); 
				IR_output(9); 
				for(i=0;i<10000;i++); 
				IR_output(6);
			}
		break;
	}
	}
Program=0;
} 



}
}

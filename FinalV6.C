
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
	  if(T>50000){Set_Play(0x02D7, 0x02D8);}
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
	  if(T>50000){Set_Play(0x02D7, 0x02D8);}
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
	TMR3IE = 0;  // Disable LED PWM
	TMR3ON = 0;
	IR_program_mode();
    RC6 = 1;
    Wait_ms(100);
    RC6 = 0;
	TMR0IE = 0;
	TMR0ON = 0;  // Disable X-10 Timer count
    M_Write_Enable();
    RC6 = 1;
    RC6 = 0;
    for(i=0;i<120;i++){M_Write_0(((240*Address)+2*i),   IR_program[i]);}
	TMR3IE = 1;  // Turn back on the interrupts
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
	TMR3IE = 0;  // Turn off uneeded interrupts for higher accuracy of IR signals
	TMR3ON = 0;
	TMR0IE = 0;
	TMR0ON = 0;
	for(i=0;i<120;i++){IR_program[i]=0;}
    RC6 = 0;
    M_Read_Address(Address*240);
	for(i=0;i<120;i++){IR_program[i]=M_Read_Data();}
	RC6=1;
	IR_out();
	TMR3IE = 1;  // Turn interrupts back on
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

void RF_output(unsigned char A, unsigned int Number)
{
	if(A == 1){
		if     (Number==1){for(i=0;i<18;i++){RF_signal_toggle[i] = A1_ON[i];}}
		else if(Number==2){for(i=0;i<18;i++){RF_signal_toggle[i] = A2_ON[i];}}
		else if(Number==3){for(i=0;i<18;i++){RF_signal_toggle[i] = A3_ON[i];}}
		else if(Number==4){for(i=0;i<18;i++){RF_signal_toggle[i] = A4_ON[i];}}
		else if(Number==5){for(i=0;i<18;i++){RF_signal_toggle[i] = A5_ON[i];}}
	}
	if(A == 0){
		if     (Number==1){for(i=0;i<18;i++){RF_signal_toggle[i] = A1_OFF[i];}}
		else if(Number==2){for(i=0;i<18;i++){RF_signal_toggle[i] = A2_OFF[i];}}
		else if(Number==3){for(i=0;i<18;i++){RF_signal_toggle[i] = A3_OFF[i];}}
		else if(Number==4){for(i=0;i<18;i++){RF_signal_toggle[i] = A4_OFF[i];}}
		else if(Number==5){for(i=0;i<18;i++){RF_signal_toggle[i] = A5_OFF[i];}}
	}
	TMR3IE = 0;
	TMR3ON = 0;   // Turn off LED PWM for higher accuracey of X-10 signal
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
	TMR3ON = 1;  // Turn the LED PWM back on
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
	RE1 = 0; 
	RB3 = 1;  
	RB4 = 0;
	Wait_ms(10);
	for(i=0;i<8;i++){
		RB4 = 1; 
		Wait_ms(10);
		RB4 = 0;
		Wait_ms(10);
	}
	RE1 = 1;
}
 
void Digital_Dn(void)
{
	RE1 = 0;  
	RB3 = 0;  
	RB4 = 0;
	Wait_ms(10);
	for(i=0;i<8;i++){
		RB4 = 1; 
		Wait_ms(10);
		RB4 = 0;
		Wait_ms(10);
	}
	RE1 = 1;
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

//%%%%%%%%%%%%%%%%%%%%%%%% Pin Assignments %%%%%%%%%%%%%%%%%%%%%%%%

// PORTA assignments
   TRISA0 = 0; // Voice Chip
   TRISA1 = 0; // IR Singal Output
   TRISA2 = 0; // Voice Chip
   TRISA3 = 0; // Voice Chip
   TRISA4 = 0; // Radio Reset
   TRISA5 = 0; // Radio Scan

// PORTB assignments
   TRISB0 = 1; // Right Button (Forward:  single press & Enter:  long press)
   TRISB1 = 1; // Left Button  (Backward: single press & Cancel: long press)
   TRISB2 = 1; // Program Button (Black)
   TRISB3 = 0; // U/D for Digital Resistor
   TRISB4 = 0; // Inc for Digital Resistor
   TRISB5 = 0; // Enclosure Red   LED
   TRISB6 = 0; // Enclosure Green LED
   TRISB7 = 0; // Enclosure Blue  LED

// PORTC assignments
   TRISC0 = 0; // RF Signal Output
   TRISC1 = 1; // Capture IR edges
   TRISC2 = 1; // Capture IR edges
   TRISC3 = 0; // EEPROM SCK
   TRISC4 = 1; // EEPROM SO
   TRISC5 = 0; // EEPROM SI
   TRISC6 = 0; // EEPROM CS
   TRISC7 = 0; // LED Program Indicator Light

// PORTD assignments
   TRISD0 = 0;
   TRISD1 = 0;
   TRISD2 = 0;
   TRISD3 = 0;
   TRISD4 = 0;
   TRISD5 = 0;
   TRISD6 = 0;
   TRISD7 = 0;

// PORTE assignments
   TRISE0 = 0; // Power to Radio
   TRISE1 = 0; // CS for Digital Resistor






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
   RE1 = 1;
   RA1 = 0;
   RC7 = 0;
   RC0 = 0;
   RA5 = 0;
   RA4 = 0;





//%%%%%%%%%%%%%%%%%%%%% IR EEPROM Location Table %%%%%%%%%%%%%%%%%%%%%

// EEPROM Address             IR Function
//	0							TV Power
//  1							DVR Power
//  2							TV Volume Up
//  3							TV Volume Down
//  4							Channel 0
//  5							Channel 1
//  6							Channel 2
//  7							Channel 3
//  8							Channel 4
//  9							Channel 5
//  10							Channel 6
//  11							Channel 7
//  12							Channel 8
//  13							Channel 9




while(1) {

// Drive enclosure LEDs using PWM
if      (LED==0){TMR3ON=0; TMR3IE=0; RB7=0; RB6=0; RB5=0;}
else if (LED==1){TMR3ON=1; TMR3IE=1; Soothing();}
else if (LED==2){TMR3ON=1; TMR3IE=1; Glower();} 
else if (LED==3){TMR3ON=1; TMR3IE=1;} 



//%%%%%%%%%%%%%%%%%%%%% Control States %%%%%%%%%%%%%%%%%%%%%

if((z0!=0)|(z1!=0)){
Move=1;

// Menu
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

// Sub
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

// Sub1
else if(Cat==1){
	if(z0==1)	  {Module=(Module+0x0010); z0=0;
		switch(Module){
		case 0x0250: Module=0x0200; break;
		case 0x03A0: Module=0x0300; break;}
	}
	else if(z1==1){Module=(Module-0x0010); z1=0;
		switch(Module){
		case 0x01F0: Module=0x0240; break;
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

// Sub2
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

// Check if program button was pressed
if(RB2){Move=1; Program=1; DO=0;}   

// Check if one the head buttons was pressed
if(Move==1){
  Move=0;
    if(Menu==1){
	switch(Module){
		case 0x0000: // TV
			if(Program==0){Set_Play(0x0013, 0x0017);} 
		break;
		case 0x1000: // X-10
			if(Program==0){Set_Play(0x001A, 0x001E);}
		break;
		case 0x2000: // Radio
			if(Program==0){Set_Play(0x0020, 0x0025);}
			break;
		case 0x3000: // LED Control
			if(Program==0){Set_Play(0x0027, 0x002E);}
			break;
		case 0x4000: // Volume
			if(Program==0){Set_Play(0x0031, 0x0038);}
			break;
		case 0x5000: // Comments
			if(Program==0){Set_Play(0x003A, 0x003F);}
			break;
		}
	}

	else if(Sub==1){
	switch (Module){
// TV
		case 0x0000: // TV Power													 
			if((DO==0) && (Program==0)){Set_Play(0x0043, 0x004A);}
			if(DO==1){DO=0; IR_output(3);}
			if(Program==1){Program=0; IR_Program(3);}
		break;
		case 0x0100: // DVR Power
			if((DO==0) && (Program==0)){Set_Play(0x004B, 0x0054);}
			if(DO==1){DO=0; IR_output(1);} 
			if(Program==1){Program=0; IR_Program(1);}
		break;
		case 0x0200: // Channel Categories
			if(Program==0){Set_Play(0x0055, 0x005E);}
		break;
		case 0x0300: // Channel Numbers
			if(Program==0){Set_Play(0x0060, 0x0065);}
		break;
		case 0x0400: // TV Volume Up
			if((DO==0) && (Program==0)){Set_Play(0x006A, 0x0072);}
			if(DO==1){DO=0; IR_output(2);}
			if(Program==1){Program=0; IR_Program(2);}
		break;
		case 0x0500: // TV Volume Down
			if((DO==0) && (Program==0)){Set_Play(0x0074, 0x007F);}
			if(DO==1){DO=0; IR_output(3);}
			if(Program==1){Program=0; IR_Program(3);}
		break;

// X-10
		case 0x1000: // A1														
			if((DO==0) && (Program==0)){Set_Play(0x0082, 0x0086);}
			if(DO==1){DO=0; RF_output(A1, 1); A1=(A1+1)%2;} 
		break;
		case 0x1100: // A2
			if((DO==0) && (Program==0)){Set_Play(0x008B, 0x0091);}
			if(DO==1){DO=0; RF_output(A2, 2); A2=(A2+1)%2;} 
		break;
		case 0x1200: // A3
			if((DO==0) && (Program==0)){Set_Play(0x0092, 0x0096);}		
			if(DO==1){DO=0; RF_output(A3, 3); A3=(A3+1)%2;}
		break;
		case 0x1300: // A4
			if((DO==0) && (Program==0)){Set_Play(0x009A, 0x009F);}
			if(DO==1){DO=0; RF_output(A4, 4); A4=(A4+1)%2;}
		break;
		case 0x1400: // A5
			if((DO==0) && (Program==0)){Set_Play(0x00A0, 0x00A5);}
			if(DO==1){DO=0; RF_output(A5, 5); A5=(A5+1)%2;} 
		break;

// Radio
		case 0x2000: // Next Station
			if((DO==0) && (Program==0)){
			RA4=0; Wait_ms(10);												
			Set_Play(0x00A8, 0x00AE);
			RA4=1; Wait_ms(10);
			}
			if(DO==1){DO=0; RA5=1; Wait_ms(25); RA5=0;}
		break; 
		case 0x2100: // Reset
			if((DO==0) && (Program==0)){
			RA4=0; Wait_ms(10);
			Set_Play(0x00B0, 0x00B5);
			RA4=1; Wait_ms(10);
			}
			if(DO==1){DO=0; RE0=1; Wait_ms(25); RE0=0;}
		break;
		case 0x2200: // Radio Volume Up
			if((DO==0) && (Program==0)){
			RA4=0; Wait_ms(10);
			Set_Play(0x00BA, 0x00C0);
			RA4=1; Wait_ms(10);
			}
			if(DO==1){DO=0; Digital_Up();}
		break;
		case 0x2300: // Radio Volume Down
			if((DO==0) && (Program==0)){
			RA4=0; Wait_ms(10);
			Set_Play(0x00C2, 0x00C8);
			RA4=1; Wait_ms(10);
			}
			if(DO==1){DO=0; Digital_Dn();}
		break;


// LED Control
		case 0x3000: // Turn Off
			if((DO==0) && (Program==0)){Set_Play(0x00D3, 0x00D6);}
			if(DO==1){DO=0; LED = 0;}
			break;
		case 0x3100: // Pattern 1
			if((DO==0) && (Program==0)){Set_Play(0x00E6, 0x00EE);}
			if(DO==1){DO=0; LED = 1;} 
		break;
		case 0x3200: // Pattern 2
			if((DO==0) && (Program==0)){Set_Play(0x00F0, 0x00F6);}
			if(DO==1){DO=0; LED = 2;} 
		break;
		case 0x3300: // Light Blue														
			if((DO==0) && (Program==0)){Set_Play(0x00F8, 0x00FD);}
			if(DO==1){DO=0; LED=3; BLUE=5; GREEN=5;}
			break;
		case 0x3400: // Purple
			if((DO==0) && (Program==0)){Set_Play(0x0100, 0x0104);}
			if(DO==1){DO=0; LED=3; BLUE=5; RED=5;} 
		break;
		case 0x3500: // Green
			if((DO==0) && (Program==0)){Set_Play(0x0106, 0x010A);}
			if(DO==1){DO=0; LED=3; GREEN=5;} 
		break;

// Voice Volume
		case 0x4000: // Device Volume Up														
			if((DO==0) && (Program==0)){Set_Play(0x010C, 0x0114);}
			if(DO==1){DO=0; if(Volume>0){Volume=Volume-1;} ISD_Vol(Volume);} 
		break;
		case 0x4100: // Device Volume Down
			if((DO==0) && (Program==0)){Set_Play(0x0115, 0x011A);}
			if(DO==1){DO=0; if(Volume<7){Volume=Volume+1;} ISD_Vol(Volume);} 
		break;

// Comments
		case 0x5000: // Thirsty														 
			if((DO==0) && (Program==0)){Set_Play(0x011C, 0x0122);}
			if(DO==1){DO=0; Set_Play(0x019F, 0x01A5);} 
		break;
		case 0x5100: // Snack
			if((DO==0) && (Program==0)){Set_Play(0x0125, 0x0129);}
			if(DO==1){DO=0; Set_Play(0x01A6, 0x01B0);}  
		break;
		case 0x5200: // Watch something
			if((DO==0) && (Program==0)){Set_Play(0x012B, 0x0132);}
			if(DO==1){DO=0; Set_Play(0x01B4, 0x01BF);} 
		break;
		case 0x5300: // Uncomfortable
			if((DO==0) && (Program==0)){Set_Play(0x0134, 0x013A);}
			if(DO==1){DO=0; Set_Play(0x01C1, 0x01CA);}  
		break;
		case 0x5400: // Like this show
			if((DO==0) && (Program==0)){Set_Play(0x013B, 0x0142);}
			if(DO==1){DO=0; Set_Play(0x01CC, 0x01D5);} 
		break;
	}
	}


	else if(Cat==1){
	switch (Module){

// Channel Categories
		case 0x0200: // Sports														 
			if(Program==0){Set_Play(0x0145, 0x014A);}
		break;
		case 0x0210: // Cartoons
			if(Program==0){Set_Play(0x014B, 0x0152);}
		break;
		case 0x0220: // Outdoors
			if(Program==0){Set_Play(0x0155, 0x015B);}
		break;
		case 0x0230: // Local
			if(Program==0){Set_Play(0x015D, 0x0162);}
		break;
		case 0x0240: // Music
			if(Program==0){Set_Play(0x0020, 0x0025);}
		break;

// Channel Numbers
		case 0x0300: // 0														
			if((DO==0) && (Program==0)){Set_Play(0x0166, 0x016A);}
			if(DO==1){DO=0; IR_output(4);}
			if(Program==1){Program=0; IR_Program(4);}
		break;
		case 0x0310: // 1
			if((DO==0) && (Program==0)){Set_Play(0x016B, 0x0170);}
			if(DO==1){DO=0; IR_output(5);}
			if(Program==1){Program=0; IR_Program(5);}
		break;
		case 0x0320: // 2 
			if((DO==0) && (Program==0)){Set_Play(0x0171, 0x0175);}
			if(DO==1){DO=0; IR_output(6);}
			if(Program==1){Program=0; IR_Program(6);}
		break;
		case 0x0330: // 3
			if((DO==0) && (Program==0)){Set_Play(0x0176, 0x017A);}
			if(DO==1){DO=0; IR_output(7);} 
			if(Program==1){Program=0; IR_Program(7);}
		break;
		case 0x0340: // 4
			if((DO==0) && (Program==0)){Set_Play(0x017C, 0x0180);}
			if(DO==1){DO=0; IR_output(8);}
			if(Program==1){Program=0; IR_Program(8);}
		break;
		case 0x0350: // 5
			if((DO==0) && (Program==0)){Set_Play(0x0181, 0x0186);}
			if(DO==1){DO=0; IR_output(9);} 
			if(Program==1){Program=0; IR_Program(9);}
		break;
		case 0x0360: // 6
			if((DO==0) && (Program==0)){Set_Play(0x0187, 0x018B);}
			if(DO==1){DO=0; IR_output(10);} 
			if(Program==1){Program=0; IR_Program(10);}
		break;
		case 0x0370: // 7
			if((DO==0) && (Program==0)){Set_Play(0x018C, 0x0191);}
			if(DO==1){DO=0; IR_output(11);}
			if(Program==1){Program=0; IR_Program(11);} 
		break;
		case 0x0380: // 8  
			if((DO==0) && (Program==0)){Set_Play(0x0192, 0x0196);}
			if(DO==1){DO=0; IR_output(12);}
			if(Program==1){Program=0; IR_Program(12);}
		break;
		case 0x0390: // 9 
			if((DO==0) && (Program==0)){Set_Play(0x0197, 0x019A);}
			if(DO==1){DO=0; IR_output(13);}
			if(Program==1){Program=0; IR_Program(13);}
		break;
	}
	}

	else if(Com==1){
	switch (Module){	

// Sports
		case 0x0200: // ESPN														
			if((DO==0) && (Program==0)){Set_Play(0x01DC, 0x01E3);}
			if(DO==1){
				DO=0; 
				IR_output(10); 
				for(i=0;i<30000;i++); 
				IR_output(6); 
				for(i=0;i<30000;i++); 
				IR_output(4);
			} 
		break;
		case 0x0201: // ESPN 2
			if((DO==0) && (Program==0)){Set_Play(0x01E4, 0x01EA);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<30000;i++); 
				IR_output(6); 
				for(i=0;i<30000;i++); 
				IR_output(5);
			}
		break;
		case 0x0202: // WGN
			if((DO==0) && (Program==0)){Set_Play(0x01EC, 0x01F3);}
			if(DO==1){
				DO=0;
				IR_output(10);
				for(i=0;i<30000;i++); 
				IR_output(5); 
				for(i=0;i<30000;i++); 
				IR_output(12);
			}
		break; // Fox North
		case 0x0203: 
			if((DO==0) && (Program==0)){Set_Play(0x01F6, 0x01FC);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<30000;i++); 
				IR_output(6); 
				for(i=0;i<30000;i++); 
				IR_output(7);
			}
		break;
		case 0x0204: // TBS
			if((DO==0) && (Program==0)){Set_Play(0x01FE, 0x0204);}
			if(DO==1){
				DO=0;
				IR_output(11); 
				for(i=0;i<30000;i++); 
				IR_output(8); 
				for(i=0;i<30000;i++); 
				IR_output(6);
			}
		break;

// Cartoons
		case 0x0210: // TV Land													
			if((DO==0) && (Program==0)){Set_Play(0x0013, 0x0017);}
			if(DO==1){
				DO=0;
				//IR_output(5); 
				//for(i=0;i<30000;i++); 
				IR_output(8); 
				for(i=0;i<30000;i++); 
				IR_output(4);
			}
		break;
		case 0x0211: // Disney
			if((DO==0) && (Program==0)){Set_Play(0x020D, 0x0214);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<30000;i++); 
				IR_output(11); 
				for(i=0;i<30000;i++); 
				IR_output(5);
			} 
		break;
		case 0x0212: // Nickelodeon
			if((DO==0) && (Program==0)){Set_Play(0x0215, 0x021C);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<30000;i++); 
				IR_output(11); 
				for(i=0;i<30000;i++); 
				IR_output(6);
			}
		break;
		case 0x0213:// Cartoon Network
			if((DO==0) && (Program==0)){Set_Play(0x014B, 0x0152);} 
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<30000;i++); 
				IR_output(11); 
				for(i=0;i<30000;i++); 
				IR_output(9);
			}
		break;
		case 0x0214: // ABC Family
			if((DO==0) && (Program==0)){Set_Play(0x0268, 0x026E);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<30000;i++); 
				IR_output(11); 
				for(i=0;i<30000;i++); 
				IR_output(4);
			}
		break;
// Outdoors
		case 0x0220: // Weather														
			if((DO==0) && (Program==0)){Set_Play(0x0233, 0x0237);}
			if(DO==1){
				DO=0;
				//IR_output(10); 
				//for(i=0;i<30000;i++); 
				IR_output(5); 
				for(i=0;i<30000;i++); 
				IR_output(12);
			}
		break;
		case 0x0221: // Animal Planet
			if((DO==0) && (Program==0)){Set_Play(0x018C, 0x0191);}
			if(DO==1){
				DO=0;
				IR_output(10);
				for(i=0;i<30000;i++); 
				IR_output(12); 
				for(i=0;i<30000;i++); 
				IR_output(4);
			} 
		break;
		case 0x0222: // Discovery
			if((DO==0) && (Program==0)){Set_Play(0x0240, 0x0245);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<30000;i++); 
				IR_output(12); 
				for(i=0;i<30000;i++); 
				IR_output(7);
			}
		break;
		case 0x0223: // History
			if((DO==0) && (Program==0)){Set_Play(0x0246, 0x024B);} 
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<30000;i++); 
				IR_output(12); 
				for(i=0;i<30000;i++); 
				IR_output(10);
			}
		break;
		case 0x0224: // Travel
			if((DO==0) && (Program==0)){Set_Play(0x024E, 0x0253);}
			if(DO==1){
				DO=0;
				IR_output(10);
				for(i=0;i<30000;i++); 
				IR_output(13); 
				for(i=0;i<30000;i++); 
				IR_output(5);
			}
		break;
// Local
		case 0x0230: // Fox														
			if((DO==0) && (Program==0)){Set_Play(0x0257, 0x025A);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<30000;i++); 
				IR_output(4); 
				for(i=0;i<30000;i++); 
				IR_output(8);
			}
		break;
		case 0x0231: // KFYR
			if((DO==0) && (Program==0)){Set_Play(0x025C, 0x0265);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<30000;i++); 
				IR_output(4); 
				for(i=0;i<30000;i++); 
				IR_output(11);
			} 
		break;
		case 0x0232: // ABC
			if((DO==0) && (Program==0)){Set_Play(0x0268, 0x026E);}
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<30000;i++); 
				IR_output(4); 
				for(i=0;i<30000;i++); 
				IR_output(12);
			}
		break;
		case 0x0233: // CBS
			if((DO==0) && (Program==0)){Set_Play(0x01FE, 0x0204);} 
			if(DO==1){
				DO=0;
				IR_output(10); 
				for(i=0;i<30000;i++); 
				IR_output(5); 
				for(i=0;i<30000;i++); 
				IR_output(5);
			}
		break;
		case 0x0234: // MSNBC
			if((DO==0) && (Program==0)){Set_Play(0x0294, 0x029C);}
			if(DO==1){
				DO=0;
//				IR_output(4);
//				for(i=0;i<30000;i++); 
				IR_output(9); 
				for(i=0;i<30000;i++); 
				IR_output(8);
			}
		break;
// Music
		case 0x0240: // Hit List														
			if((DO==0) && (Program==0)){Set_Play(0x016B, 0x0170);}
			if(DO==1){
				DO=0; 
				IR_output(13); 
				for(i=0;i<30000;i++); 
				IR_output(4); 
				for(i=0;i<30000;i++); 
				IR_output(5);
			} 
		break;
		case 0x0241: // R&B Classics
			if((DO==0) && (Program==0)){Set_Play(0x0171, 0x0175);}
			if(DO==1){
				DO=0;
				IR_output(13); 
				for(i=0;i<30000;i++); 
				IR_output(0); 
				for(i=0;i<30000;i++); 
				IR_output(12);
			}
		break;
		case 0x0242: // Pop Hits
			if((DO==0) && (Program==0)){Set_Play(0x0176, 0x017A);}
			if(DO==1){
				DO=0;
				IR_output(13);
				for(i=0;i<30000;i++); 
				IR_output(6); 
				for(i=0;i<30000;i++); 
				IR_output(4);
			}
		break; // Today's Country
		case 0x0243: 
			if((DO==0) && (Program==0)){Set_Play(0x017C, 0x0180);}
			if(DO==1){
				DO=0;
				IR_output(13); 
				for(i=0;i<30000;i++); 
				IR_output(6); 
				for(i=0;i<30000;i++); 
				IR_output(13);
			}
		break;
		case 0x0244: // Contemporary Christian
			if((DO==0) && (Program==0)){Set_Play(0x0181, 0x0186);}
			if(DO==1){
				DO=0;
				IR_output(13); 
				for(i=0;i<30000;i++); 
				IR_output(7); 
				for(i=0;i<30000;i++); 
				IR_output(6);
			}
		break;
	}
	}
Program=0;
} 



}
}

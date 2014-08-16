
#include <pic18.h>
#include <math.h>
#include "function.h"
#include        "lcd_portd.h"
#include "function.c"
#include        "lcd_portd.c"
// Global Variables
	// IR Variables

/*
const unsigned char MSG[20] = "TV   ";
const unsigned char MSG0[20] ="DVR  ";
const unsigned char MSG1[20] ="X-10 ";
const unsigned char MSG2[20] ="Radio";
const unsigned char MSG3[20] ="LED  ";
const unsigned char MSG4[20] ="Vol  ";
const unsigned char MSG5[20] ="Comme";
const unsigned char MSG6[20] ="TV On";
const unsigned char MSG7[20] ="Categ";
const unsigned char MSG8[20] ="CH #'s";
const unsigned char MSG9[20] ="A1   ";
const unsigned char MSG10[20] ="A2   ";
const unsigned char MSG11[20] ="A3   ";
const unsigned char MSG12[20] ="A4   ";
const unsigned char MSG13[20] ="A5   ";
const unsigned char MSG14[20] ="R On/";
const unsigned char MSG15[20] ="Next ";
const unsigned char MSG16[20] ="Reset";
const unsigned char MSG17[20] ="Up   ";
const unsigned char MSG18[20] ="Dn   ";
const unsigned char MSG19[20] ="LED  ";
const unsigned char MSG20[20] ="C1   ";
const unsigned char MSG21[20] ="C2   ";
const unsigned char MSG22[20] ="C3   ";
const unsigned char MSG23[20] ="C4   ";
const unsigned char MSG24[20] ="C5   ";
const unsigned char MSG25[20] ="Sport";
const unsigned char MSG26[20] ="Cart ";
const unsigned char MSG27[20] ="Outdo";
const unsigned char MSG28[20] ="Local";
const unsigned char MSG29[20] ="ESPN ";
const unsigned char MSG30[20] ="ESPN2";
const unsigned char MSG31[20] ="WGN  ";
const unsigned char MSG32[20] ="Fox N";
const unsigned char MSG33[20] ="TBS  ";
const unsigned char MSG34[20] ="Boome";
const unsigned char MSG35[20] ="Disne";
const unsigned char MSG36[20] ="Nicke";
const unsigned char MSG37[20] ="Nick2";
const unsigned char MSG38[20] ="Famil";
const unsigned char MSG39[20] ="0    ";
const unsigned char MSG40[20] ="1    ";
const unsigned char MSG41[20] ="2    ";
const unsigned char MSG42[20] ="3    ";
const unsigned char MSG43[20] ="4    ";
const unsigned char MSG44[20] ="5    ";
const unsigned char MSG45[20] ="6    ";
const unsigned char MSG46[20] ="7    ";
const unsigned char MSG47[20] ="8    ";
const unsigned char MSG48[20] ="9    ";
*/
unsigned char x, IR_do, IR_count, Break, Program;
unsigned int Time;
unsigned int IR_program[120];
unsigned int TV_Ch_0[120], TV_Ch_1[120], TV_Ch_2[120], TV_Ch_3[120], TV_Ch_4[120], TV_Ch_5[120], TV_Ch_6[120];
unsigned int TV_Ch_7[120], TV_Ch_8[120], TV_Ch_9[120], TV_Pwr[120];
	// RF Variables
unsigned char A1, A2, A3, A4, A5;
unsigned char A_1_8[16] = {1, 3, 7, 11, 13, 15, 17, 19, 21, 25, 27, 29, 33, 37, 41, 45};
unsigned char A1_ON[18] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53};
unsigned int A1_OFF[18] = {1, 3, 5, 9, 11, 13, 15, 17, 19, 23, 27, 29, 33, 37, 41, 45, 49, 53};
unsigned char A2_ON[18] = {1, 3, 5, 7, 11, 13, 15, 17, 19, 23, 27, 31, 33, 37, 41, 45, 49, 53};
unsigned char A2_OFF[18] ={1, 3, 5, 9, 13, 15, 17, 19, 21, 25, 29, 31, 33, 37, 41, 45, 49, 53};
unsigned char A3_ON[18] = {1, 3, 5, 7, 9, 13, 15, 17, 19, 23, 27, 31, 35, 37, 41, 45, 49, 53};
unsigned char A3_OFF[18] ={1, 3, 5, 9, 11, 15, 17, 19, 21, 25, 29, 31, 35, 37, 41, 45, 49, 53};
unsigned char A4_ON[18] = {1, 3, 5, 7, 11, 15, 17, 19, 21, 25, 29, 33, 35, 37, 41, 45, 49, 53};
unsigned char A4_OFF[18] ={1, 3, 5, 9, 13, 17, 19, 21, 23, 27, 31, 33, 35, 37, 41, 45, 49, 53};
unsigned char A5_ON[18] = {1, 3, 7, 9, 11, 13, 15, 17, 19, 23, 25, 29, 33, 37, 41, 45, 49, 53};
unsigned char A5_OFF[18] ={1, 3, 7, 11, 13, 15, 17, 19, 21, 25, 27, 29, 33, 37, 41, 45, 49, 53};
unsigned char RF_signal_toggle[18];
unsigned char y, f;
	// Enclosure LED
unsigned int Color, RED, BLUE, GREEN;
unsigned int Count;
unsigned char R, B, G, Dnr, Dnb, Dng;
unsigned char soothing, glower, LED;
	// Others
unsigned int i, Module;
unsigned char z0, z1, Move, Menu, Sub, Cat, Com;
unsigned long T;
	// EEPROM
unsigned int E_TV_CH_6[120];


// Interrupts
void interrupt IntServe(void){
// Timer 0
if (TMR0IF) {
	//Color=(Color+1)%100;
	//if (RED>Color)RB5=1;   else RB5=0;
	//if (GREEN>Color)RB6=1; else RB6=0;
	//if (BLUE>Color)RB7=1;  else RB7=0;
	if(Program==1){
		if((IR_program[x-1]-IR_program[x-2])>800){Break=1; IR_program[119]=IR_program[x-1]; IR_program[x-1]=0;}}   // Break>800
		//if((Time-IR_program[x-1])>400){Break=1;IR_program[119]=Time;}}
TMR0 = -1000; // -400; -1000
TMR0IF = 0;
}

// Timer 1
if (TMR1IF){
Time = Time + 1;
//RA2=!RA2;
if(IR_do==1){
	if(Time == IR_program[x]){
		RA1 = !RA1;
		x = x + 1;
}
}
if (IR_count==1){TMR1=-190;}  // -190
else TMR1=-3150;
TMR1IF = 0;
}

// Capture edges for IR
if (CCP2IF){
if(RC7){
IR_program[x] = Time;
x = x + 1;
}
CCP2IF = 0;
}
if (CCP1IF){
if(RC7){
IR_program[x] = Time;
x = x + 1;
}
CCP1IF = 0;
}

// Int Interrupts
if (INT0IF) {
T=0;
do{T=T+1;}while(RB0);
if (T>300000){z0=2;}
else z0=1;
INT0IF = 0;
}
if (INT1IF) {
T=0;
do{T=T+1;}while(RB1);
if (T>300000){z1=2;}
else z1=1;
INT1IF = 0;
}

}

// Subroutines
void IR_program_mode(void){
for(i=0;i<4000;i++);
for(i=0;i<120;i++){IR_program[i]=0;}
IR_count = 1;
RC7 = 1;
do{x=0; Time=100;}while((RC1));
Break=0;
Program=1;
do{for(i=0;i<10;i++);}while(Break==0);  
Program=0; 
RC7 = 0;
IR_count = 0;
Break=0;
}

void IR_out(void){
RA1 = 0;
x = 0;
IR_count=1;
IR_do=1;
Time = 0;
do{for(i=0;i<10;i++);}while(Time<=(IR_program[119]-78)); // [119]-78
x = 0;
//for(i=0;i<1900;i++);///
Time = 45;
do{for(i=0;i<10;i++);}while(Time<=(IR_program[119]-78)); // [119]-78
x = 0;
//for(i=0;i<1900;i++); ///
Time = 45;
do{for(i=0;i<10;i++);}while(Time<=(IR_program[119]));
IR_do=0;
RA1=0;
IR_count=0;
}

void wakeup(void){
Time = 0;
do{
RC0 = 1;
}while(Time < 17);
do{
RC0 = 0;
}while(Time < 24);
}

void RF_out(void){
x = 0;
Time = 0;
do{
if(Time == A_1_8[x]){
RC0 = 1;
y = Time;
f = 1;
}
if((y != Time) & (f == 1)){
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
if((y != Time) & (f == 1)){
x = x + 1;
f = 0;
}
if(Time != RF_signal_toggle[x]){RC0 = 0;}
}while(x<17);
RC0 = 0;
}

void RF_output(unsigned char A, unsigned char Number_ON[18], unsigned char Number_OFF[18]){
if(A == 1){
for(i=0;i<18;i++){RF_signal_toggle[i] = Number_ON[i];}
}
if(A == 0){
for(i=0;i<18;i++){RF_signal_toggle[i] = Number_OFF[i];}
}
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
}

void Glower(void){
for(i=0;i<3000;i++);
Count=(Count+1)%300;
if(Count<100){RED=Count; GREEN=100-Count; BLUE=0;}
if(Count>=100&Count<200){RED=100-Count; GREEN=0; BLUE=Count;}
if(Count>=200){RED=0; BLUE=100-Count; GREEN=Count;}
}

void Soothing (void){
for(i=0;i<3000;i++);       // Speed
if((R==0&G==0&B==0)|(R==0&G==1&B==1)){RED=RED+1;}
if(R==1&G==0&B==0){GREEN=GREEN+1;}
if(R==0&G==1&B==0){BLUE=BLUE+1;}
if((R==1&G==1&B==0)|(R==1&G==0&B==1)){RED=RED-1;}
if(R==1&G==1&B==1){GREEN=GREEN-1;}
if(R==0&G==0&B==1){BLUE=BLUE-1;}
if(RED==100){R=1;}
if(GREEN==100){G=1;}
if(BLUE==100){B=1;}
if(RED==0){R=0;}
if(GREEN==0){G=0;}
if(BLUE==0){B=0;}
}

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

void SCI_Out(unsigned int DATA)
{
   while(!TRMT);  TXREG = ascii(DATA >> 12);
   while(!TRMT);  TXREG = ascii(DATA >> 8);
   while(!TRMT);  TXREG = ascii(DATA >> 4);
   while(!TRMT);  TXREG = ascii(DATA);
   while(!TRMT);  TXREG = 32;
   }
void LCD_Out(unsigned int DATA)
{
   unsigned char A[4], i;
   for (i=0; i<4; i++) {
      A[i] = DATA % 16;
      DATA = DATA / 16;
      } 
   LCD_Write(ascii(A[3]));
   LCD_Write(ascii(A[2]));
   LCD_Write(ascii(A[1]));
   LCD_Write(ascii(A[0]));
   }



void main(void)
{
// Initialize Serial Port
   TRISC = 0xD0;
   TXIE = 0;
   RCIE = 0;
   BRGH = 0;
   BRG16 = 1;
   SYNC = 0;
   SPBRG = 129;
   TXSTA = 0x22;
   RCSTA = 0x90;
// Initialize the SPI port
   SSPIE = 0;
   TRISC5 = 0;
   TRISC4 = 1;
   TRISC3 = 0;
   TRISC6 = 0;
   SSPSTAT = 0x40;
   SSPCON1 = 0x22;



// set up Timer0 for 1ms
T0CS = 0;
T0CON = 0x88;
TMR0 = -1000;
TMR0ON = 1;
TMR0IE = 1;
TMR0IP = 1;
// Turn on Timer1
TMR1CS = 0;
T1CON = 0x81;
TMR1 = -3150;
TMR1ON = 1;
TMR1IE = 1;
TMR1IP = 1;
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
//INT0IP = 1;
INTEDG0 = 1; // rising edges
// initialize INT0 interrupts
INT1IE = 1;
INT1IP = 1;
TRISB1 = 1;
INTEDG1 = 1; // rising edges



PEIE = 1;
GIE = 1;
ADCON1 = 0X0F;


// PORTD assignments (not used)
TRISD=0x00;
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
TRISB0 = 1; // Right Button (Forward: single press & Enter: double press)
TRISB1 = 1; // Left Button  (Backward: single press & Cancel: double press)
TRISB2 = 1; // Program Button (Red)
TRISB3 = 1; // IR Out Button          !!!!REMOVE LATER
TRISB4 = 1; // RF Out Button          !!!!REMOVE LATER
TRISB5 = 0; // Enclosure Red   LED
TRISB6 = 0; // Enclosure Green LED
TRISB7 = 0; // Enclosure Blue  LED
// PORTA assignments
TRISA1 = 0; // IR Singal Output
TRISA2 = 0;


// Assign values to variables
Time = 0;
IR_do = 0;
IR_count = 0;
Program=0;
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

// Ports
RA1 = 0;
RC7 = 0;
PORTD=0x00;
RA2=0;
RC6 = 0;


   LCD_Init();

while(1) {

	Color=(Color+1)%100;
	if (RED>Color)RB5=1;   else RB5=0;
	if (GREEN>Color)RB6=1; else RB6=0;
	if (BLUE>Color)RB7=1;  else RB7=0;

if (LED==1){Soothing();}
else if (LED==2){Glower();} 
	// RF
//if(RB4) {
//RF_output(A2, A2_ON[18], A2_OFF[18]);
//A2 = (A2 + 1) % 2;
//} 
	// IR
if (RB2){          //Program Button
	IR_program_mode();
	for(i=0;i<120;i++){TV_Ch_6[i]=IR_program[i];}
for(i=0;i<120;i++){E_TV_CH_6[i]=TV_Ch_6[i];}
   RC6 = 1;
   Wait_ms(100);
   RC6 = 0;
   M_Write_Enable();
   RC6 = 1;
   RC6 = 0;
         for(i=0;i<120;i++){M_Write_0(2*i,   E_TV_CH_6[i]);}

	}




if (RB3){          //IR Transmit Button
	for(i=0;i<120;i++){IR_program[i]=0;}
          RC6 = 0;
          M_Read_Address(0);
for(i=0;i<120;i++){TV_Ch_6[i]=M_Read_Data();}
RC6=1;
	for(i=0;i<120;i++){IR_program[i]=TV_Ch_6[i];}
	IR_out();
	}	


/*
// Control States
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
	else if(z0==2){z0=0; Menu=0; Sub=1;}
	else if(z1==2){z1=0;}
}
else if(Sub==1){
	if(z0==1)	  {Module=(Module+0x0100); z0=0;
		switch(Module){
		case 0x0400: Module=0x0000; break;
		case 0x1500: Module=0x1000; break;
		case 0x2300: Module=0x2000; break;
		case 0x3200: Module=0x3000; break;
		case 0x4200: Module=0x4000; break;
		case 0x5500: Module=0x5000; break;}
	}
	else if(z1==1){Module=(Module-0x0100); z1=0;
		switch(Module){
		case 0xFF00: Module=0x0300; break;
		case 0x0F00: Module=0x1400; break;
		case 0x1F00: Module=0x2200; break;
		case 0x2F00: Module=0x3100; break;
		case 0x3F00: Module=0x4100; break;
		case 0x4F00: Module=0x5400; break;}
	}
	else if(z0==2){z0=0; Sub=0; Cat=1;}
	else if(z1==2){z1=0; Sub=0; Menu=1; Module=(Module & 0xF000);} // 0x1000
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
	else if(z0==2){z0=0; Cat=0; Com=1;}
	else if(z1==2){z1=0; Cat=0; Sub=1; Module=(Module & 0xFF00);} // 0x1100
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
	else if(z1==2){z1=0; Com=0; Cat=1; Module=(Module & 0xFFF0);} // 0x1110
}
}

/*
// Output voice and maybe function
if(Move==1){
  Move=0;
LCD_Move(3,0); LCD_Out(Module);
  if(Menu==1){
	if	   (Module==0x0000){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG[i]);} // TV
	else if(Module==0x1000){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG1[i]);} // X-10
	else if(Module==0x2000){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG2[i]);} // Radio
	else if(Module==0x3000){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG3[i]);} // LED Control
	else if(Module==0x4000){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG4[i]);} // Volume Control
	else if(Module==0x5000){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG5[i]);} // Comments
  }
  else if(Sub==1){
	if      ((Module & 0xF000)==0x0000){ // TV
		     if(Module==0x0000){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG6[i]);} // TV on/off
		else if(Module==0x0100){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG0[i]);} // DVR on/off
		else if(Module==0x0200){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG7[i]);} // Channel Categories
		else if(Module==0x0300){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG8[i]);} // Channel Numbers 
	}
	else if ((Module & 0xF000)==0x1000){  // X-10
		     if(Module==0x1000){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG9[i]);}
		else if(Module==0x1100){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG10[i]);}
		else if(Module==0x1200){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG11[i]);}
		else if(Module==0x1300){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG12[i]);}
		else if(Module==0x1400){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG13[i]);}
//		     if(Module==0x1000){RF_output(A1, A1_ON[18], A1_OFF[18]); A1 = (A1 + 1) % 2;}
//		else if(Module==0x1100){RF_output(A2, A2_ON[18], A2_OFF[18]); A2 = (A2 + 1) % 2;}
//		else if(Module==0x1200){RF_output(A3, A3_ON[18], A3_OFF[18]); A3 = (A3 + 1) % 2;}
//		else if(Module==0x1300){RF_output(A4, A4_ON[18], A4_OFF[18]); A4 = (A4 + 1) % 2;}
//		else if(Module==0x1400){RF_output(A5, A5_ON[18], A5_OFF[18]); A5 = (A5 + 1) % 2;}
	}
	else if ((Module & 0xF000)==0x2000){ // Radio
		     if(Module==0x2000){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG14[i]);}
		else if(Module==0x2100){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG15[i]);}
		else if(Module==0x2200){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG16[i]);}
	}
	else if ((Module & 0xF000)==0x3000){
		     if(Module==0x3000){(LED=LED+1)%3;}
	}
	else if ((Module & 0xF000)==0x4000){
		     if(Module==0x4000){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG17[i]);} // Volume Up
	    else if(Module==0x4100){LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG18[i]);} // Volume Down
	}
	else if ((Module & 0xF000)==0x5000){
		     if(Module==0x5000){}
		else if(Module==0x5100){}
		else if(Module==0x5200){}
		else if(Module==0x5300){}
		else if(Module==0x5400){}
	}
  }
  else if(Cat==1){
	
  }

}



// Output voice and maybe function
if(Move==1){
  Move=0;
LCD_Move(3,0); LCD_Out(Module);
    if(Menu==1){
	switch(Module){
		case 0x0000: // TV
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG[i]); break;
		case 0x1000: // X-10
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG1[i]); break;
		case 0x2000: // Radio
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG2[i]); break;
		case 0x3000: // LED Control
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG3[i]); break;
		case 0x4000: // Volume
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG4[i]); break;
		case 0x5000: // Comments
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG5[i]); break;
	}
	}

//		     if(Module==0x1000){RF_output(A1, A1_ON[18], A1_OFF[18]); A1 = (A1 + 1) % 2;}
//		else if(Module==0x1100){RF_output(A2, A2_ON[18], A2_OFF[18]); A2 = (A2 + 1) % 2;}
//		else if(Module==0x1200){RF_output(A3, A3_ON[18], A3_OFF[18]); A3 = (A3 + 1) % 2;}
//		else if(Module==0x1300){RF_output(A4, A4_ON[18], A4_OFF[18]); A4 = (A4 + 1) % 2;}
//		else if(Module==0x1400){RF_output(A5, A5_ON[18], A5_OFF[18]); A5 = (A5 + 1) % 2;}

	else if(Sub==1){
	switch (Module){
		case 0x0000: 														// TV
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG6[i]); break;
		case 0x0100: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG0[i]); break;
		case 0x0200:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG7[i]); break;
		case 0x0300: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG8[i]); break;
		case 0x1000: 														// X-10
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG9[i]); 
			RF_output(A1, A1_ON[18], A1_OFF[18]); A1 = (A1 + 1) % 2; break;
		case 0x1100: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG10[i]); 
			RF_output(A2, A2_ON[18], A2_OFF[18]); A2 = (A2 + 1) % 2; break;
		case 0x1200:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG11[i]); 
			RF_output(A3, A3_ON[18], A3_OFF[18]); A3 = (A3 + 1) % 2; break;
		case 0x1300: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG12[i]); 
			RF_output(A4, A4_ON[18], A4_OFF[18]); A4 = (A4 + 1) % 2; break;
		case 0x1400:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG13[i]); 
			RF_output(A5, A5_ON[18], A5_OFF[18]); A5 = (A5 + 1) % 2; break;
		case 0x2000:  														// Radio
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG14[i]); break;
		case 0x2100:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG15[i]); break;
		case 0x2200: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG16[i]); break;
		case 0x3000: 														// LED Control
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG19[i]); break;
		case 0x4000: 														// Volume
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG17[i]); break;
		case 0x4100:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG18[i]); break;
		case 0x5000: 														// Comments
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG20[i]); break;
		case 0x5100: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG21[i]); break;
		case 0x5200:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG22[i]); break;
		case 0x5300: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG23[i]); break;
		case 0x5400:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG24[i]); break;
	}
	}

	else if(Cat==1){
	switch (Module){
		case 0x0200: 														// Channel Categories
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG25[i]); break;
		case 0x0210: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG26[i]); break;
		case 0x0220:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG27[i]); break;
		case 0x0230: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG28[i]); break;
		case 0x0300:  														// Channel Numbers
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG39[i]); break;
		case 0x0310:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG40[i]); break;
		case 0x0320: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG41[i]); break;
		case 0x0330:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG42[i]); break;
		case 0x0340: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG43[i]); break;
		case 0x0350:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG44[i]); break;
		case 0x0360: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG45[i]); break;
		case 0x0370:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG46[i]); break;
		case 0x0380: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG47[i]); break;
		case 0x0390:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG48[i]); break;
	}
	}
	else if(Com==1){
	switch (Module){	
		case 0x0200: 														// Sports
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG29[i]); break;
		case 0x0201: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG30[i]); break;
		case 0x0202:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG31[i]); break;
		case 0x0203: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG32[i]); break;
		case 0x0204:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG33[i]); break;
		case 0x0210:  														// Cartoons
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG34[i]); break;
		case 0x0211:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG35[i]); break;
		case 0x0212: 
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG36[i]); break;
		case 0x0213:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG37[i]); break;
		case 0x0214:
			LCD_Move(0,0);for (i=0; i<5; i++) LCD_Write(MSG38[i]); break;
	}
	}
}
*/
}
}

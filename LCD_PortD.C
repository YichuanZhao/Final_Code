// --------- LCD_PortD.C ---------------------
//
// This routine has LCD driver routines
//
//  LCD_INST:   send an instruction to the LCD
//  LCD_DATA:   send data to the LCD
//  LCD_INIT:   initialize the LCD to 16x4 mode
//
//--- Revision History -----------------
//     5/20/00   JSG
//     9/27/00   Modify LCD_HELLO for Jump messages
//    11/05/00   Clean up LCD routine to use less RAM

extern volatile near unsigned char	PORTF		@ 0xF7F;
extern volatile near bit	RF0		@ ((unsigned)&PORTF*8)+0;
extern volatile near bit	RF1		@ ((unsigned)&PORTF*8)+1;
extern volatile near bit	RF2		@ ((unsigned)&PORTF*8)+2;
extern volatile near bit	RF3		@ ((unsigned)&PORTF*8)+3;
extern volatile near bit	RF4		@ ((unsigned)&PORTF*8)+4;
extern volatile near bit	RF5		@ ((unsigned)&PORTF*8)+5;
extern volatile near bit	RF6		@ ((unsigned)&PORTF*8)+6;
extern volatile near bit	RF7		@ ((unsigned)&PORTF*8)+7;


void LCD_Pause(void)
{
   unsigned char x;
   for (x=0; x<20; x++);
   }


void LCD_Strobe(void)
{
   RF3 = 0;
   PORTD = PORTF;
   LCD_Pause();
   RF3 = 1;
   PORTD = PORTF;
   LCD_Pause();
   RF3 = 0;
   PORTD = PORTF;
   LCD_Pause();
   }
         
//  write a byte to the LCD in 4 bit mode 

void LCD_Inst(unsigned char c)
{
    RF2 = 0;        // send an instruction
	PORTF = (PORTF & 0x0F) |  (c & 0xF0);
	LCD_Strobe();
	PORTF = (PORTF & 0x0F) |  ((c<<4) & 0xF0);
	LCD_Strobe();
	Wait_ms(100);
}

void LCD_Move(unsigned char Row, unsigned char Col)
{
   if (Row == 0) LCD_Inst(0x80 + Col);
   if (Row == 1) LCD_Inst(0xC0 + Col);
   if (Row == 2) LCD_Inst(0x94 + Col);
   if (Row == 3) LCD_Inst(0xD4 + Col);
   }
   
void LCD_Write(unsigned char c)
{
    RF2 = 1;        // send data
	PORTF = (PORTF & 0x0F) |  (c & 0xF0);
	LCD_Strobe();
	PORTF = (PORTF & 0x0F) |  ((c<<4)  & 0xF0);
	LCD_Strobe();
	
}


void LCD_Init(void)
{
   TRISD = 0x03;       // 20x4 LCD Boards
   
   LCD_Inst(0x33);
   LCD_Inst(0x32);
   LCD_Inst(0x28);
   LCD_Inst(0x0E);
   LCD_Inst(0x01);
   LCD_Inst(0x06);
   }

   


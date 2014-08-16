// ---  COUNTABC.C -------------------
//  ECE 376 Development Board program
//------------------------------



// Global Variables


// Subroutine Declarations
#include <pic18.h>

#include <math.h>
// Bootloader Routine


// Main Routine



unsigned int z1, z2, x1, y, f, p, z, Module, beeps, Play_voice, new_mod, old_mod, mod_diff,
module_num, main_menu, do_output, IR_time;
unsigned int A1, A2, A3, A4, A5;
unsigned int A_1_8[16] = {1, 3, 7, 11, 13, 15, 17, 19, 21, 25, 27, 29, 33, 37, 41, 45};
unsigned int A1_ON[18] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53};
unsigned int A1_OFF[18] = {1, 3, 5, 9, 11, 13, 15, 17, 19, 23, 27, 29, 33, 37, 41, 45, 49, 53};
unsigned int A2_ON[18] = {1, 3, 5, 7, 11, 13, 15, 17, 19, 23, 27, 31, 33, 37, 41, 45, 49, 53};
unsigned int A2_OFF[18] = {1, 3, 5, 9, 13, 15, 17, 19, 21, 25, 29, 31, 33, 37, 41, 45, 49, 53};
unsigned int A3_ON[18] = {1, 3, 5, 7, 9, 13, 15, 17, 19, 23, 27, 31, 35, 37, 41, 45, 49, 53};
unsigned int A3_OFF[18] = {1, 3, 5, 9, 11, 15, 17, 19, 21, 25, 29, 31, 35, 37, 41, 45, 49, 53};
unsigned int A4_ON[18] = {1, 3, 5, 7, 11, 15, 17, 19, 21, 25, 29, 33, 35, 37, 41, 45, 49, 53};
unsigned int A4_OFF[18] = {1, 3, 5, 9, 13, 17, 19, 21, 23, 27, 31, 33, 35, 37, 41, 45, 49, 53};
unsigned int A5_ON[18] = {1, 3, 7, 9, 11, 13, 15, 17, 19, 23, 25, 29, 33, 37, 41, 45, 49, 53};
unsigned int A5_OFF[18] = {1, 3, 7, 11, 13, 15, 17, 19, 21, 25, 27, 29, 33, 37, 41, 45, 49, 53};
unsigned int RF_signal_toggle[18];
unsigned int Time, i, j, x;



void Wait_ms(unsigned int X)
{
   unsigned int i, j;

   for (i=0; i<X; i++)
      for (j=0; j<300; j++);
   }

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


void Set_Play(unsigned char s, unsigned char e)
{
 RC0=1; 
 RC1=1;
 RC3=0;

 RC0=0; 
 ISD_Out(0x80);
 ISD_Out(0x00);

 ISD_Out(s);
 ISD_Out(0x00);

 ISD_Out(e);
 ISD_Out(0x00);
 ISD_Out(0x00);
 Wait_ms(100);
 
 RC0=1;

}

void wakeup(void){
Time = 0;
do{
RC3 = 1;
}while(Time < 17);   // 20
do{
RC3 = 0;
}while(Time < 24);   // 28
}

void RF_out(void){
x = 0;
Time = 0;
do{
if(Time == A_1_8[x]){
RC3 = 1;
y = Time;
f = 1;
}
if((y != Time) && (f == 1)){
x = x + 1;
f = 0;
}
if(Time != A_1_8[x]){RC3 = 0;}
}while(Time < 47);
x = 0;
Time = 0;
do{
if(Time == RF_signal_toggle[x]){
RC3 = 1;
y = Time;
f = 1;
}
if((y != Time) && (f == 1)){
x = x + 1;
f = 0;
}
if(Time != RF_signal_toggle[x]){RC3 = 0;}
}while(x<17);  //Time < 55
RC3 = 0;
}




void main(void)
{


TRISA0 = 0;
TRISA2 = 0;
TRISA3 = 0;
TRISA4 = 1;
TRISA5 = 1;
TRISE0 = 1;
TRISE1 = 0;



TRISB = 0xFF;
TRISC=0x00;
TRISD=0x00;
ADCON1=15; 

unsigned char A[]={0x10,0x1B,0x28,0x34,0x40,0x4B,0x58,0x64,0x72,0x7F,0x8F,0xA0,0xAE};
/*
unsigned char X[]={0x1B,0x28,0x34,0x40};
unsigned char Y[]={0x4B,0x58,0x64,0x72};
unsigned char Z[]={0x7F,0x8F,0xA0,0xAE};
*/

unsigned int m,n,u,y,t;

unsigned char k,l;

m=0;


while(1)
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

 while((RA5==1)&& (m>=0) && (m<=11))
{

 n=m+1;
 k=A[m];
 l=A[n];
 
 RA0=0; 
 ISD_Out(0x80);
 ISD_Out(0x00);

 ISD_Out(k);
 ISD_Out(0x00);

 ISD_Out(l);
 ISD_Out(0x00);
 ISD_Out(0x00);
 Wait_ms(100);
 
 RA0=1;
 m=m+1;


}


while(RA5==1 && m>=12)
{
 m=0;
 n=m+1;
 k=A[m];
 l=A[n];
 
 RA0=0; 
 ISD_Out(0x80);
 ISD_Out(0x00);

 ISD_Out(k);
 ISD_Out(0x00);

 ISD_Out(l);
 ISD_Out(0x00);
 ISD_Out(0x00);
 Wait_ms(100);
 
 RA0=1;

}

/*
while(RB1==1 && m==0)
{
 if( u>=0 && u<=2)
{
 n=u+1;
 k=X[u];
 l=X[n];
}
else
{
 u=0;
 n=u+1;
 k=X[u];
 l=X[n];
}
 RC0=0; 
 ISD_Out(0x80);
 ISD_Out(0x00);

 ISD_Out(k);
 ISD_Out(0x00);

 ISD_Out(l);
 ISD_Out(0x00);
 ISD_Out(0x00);
 Wait_ms(100);
 
 RC0=1;

 u=u+1;

}

while(RB1==1 && m==4)
{
 if( y>=0 && y<=2)
{
 n=y+1;
 k=Y[y];
 l=Y[n];
}
else
{
 y=0;
 n=y+1;
 k=Y[y];
 l=Y[n];
}
 RC0=0; 
 ISD_Out(0x80);
 ISD_Out(0x00);

 ISD_Out(k);
 ISD_Out(0x00);

 ISD_Out(l);
 ISD_Out(0x00);
 ISD_Out(0x00);
 Wait_ms(100);
 
 RC0=1;

 y=y+1;

}
while(RB1==1 && m==8)
{
 if( t>=0 && t<=2)
{
 n=t+1;
 k=Z[t];
 l=Z[n];
}
else
{
 t=0;
 n=t+1;
 k=Z[t];
 l=Z[n];
}
 RC0=0; 
 ISD_Out(0x80);
 ISD_Out(0x00);

 ISD_Out(k);
 ISD_Out(0x00);

 ISD_Out(l);
 ISD_Out(0x00);
 ISD_Out(0x00);
 Wait_ms(100);
 
 RC0=1;

 t=t+1;

}


/*
 RC0=0;
 ISD_Out(0x02);
 ISD_Out(0x00);
*/


 RA0=1;
 RA2=1;
 RA3=0;
 
 RA0=0; 
 ISD_Out(0x07);
 ISD_Out(0x00);
 RA0=1; 



if (RE0==1)
{
RE1=1;
}
else
{
RE1=0;
}



/*
if(RB0==1) {

if(A2 == 1){
for(i=0;i<18;i++){RF_signal_toggle[i] = A2_ON[i];}
}
if(A2 == 0){
for(i=0;i<18;i++){RF_signal_toggle[i] = A2_OFF[i];}
}
A2 = (A2 + 1) % 2;
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

*/
}


}



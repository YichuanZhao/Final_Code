// ---  FUNCTION.C --------------
//  Generally useful functions
//
//  void hex2dec(unsigned int X, unsigned char *A[5])
//    separate an unsigned integer into decimal
//          A[4] * 10E4
//          A[3] * 10E3
//          A[0] * 1
//
// void Wait_ms(unsighed int X)
//    wait approximately X milliseconds and return

void hex2dec(unsigned int X, unsigned char *A[5])
{
   unsigned int Y;
   unsigned char a4, a3, a2, a1, a0;

   a4 = X / 10000;
   Y  = a4;
   X  -= 10000 * Y;

   a3 = X / 1000;
   Y  = a3;
   X  -= 1000 * Y;

   a2 = X / 100;
   Y  = a2;
   X  -= 100 * Y;

   a1 = X / 10;
   Y  = a1;
   X  -= 10 * Y;

   a0 = X;
   
   A[4] = a4;
   A[3] = a3;
   A[2] = a2;
   A[1] = a1;
   A[0] = a0;
   }

// ---- WAIT_MS ----------------

void Wait_ms(unsigned int X)
{
   unsigned int Y;

   do {
      Y = 500;
      do {
         Y -= 1;
         } while (Y > 0);
      X -= 1;
      } while (X > 0);
   }

// - ASCII ------------------------

unsigned char ascii(unsigned char c) 
{
   c = c & 0x0F;
   if (c < 10) return (c+48);
   else return(c + 55);
}

   

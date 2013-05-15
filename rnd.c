#include <stdlib.h>

float rnd(int seed)
   {
/*    linear congruential random number generator with shuffling */
/*    based on ran1 of second edition of "Numerical Recipes" */
#define min(a,b)  ((a) < (b) ? (a) : (b))
#define a 16807
#define m 2147483647  /* 2^31 - 1  */ 
#define rm 1.0/m
#define q 127773  /*  m = a*q + p  */
#define p 2836
#define n 32
#define ndiv (1 + (m-1)/n)
#define rmax (1.0 - 1.2e-7)
      static int r[n+1],r0,r1;
      int j,k;
      if (seed != 0)
/*      initialize table of random numbers  */
         {
          r1 = abs(seed);
       for (j = n+8;j>=1;j--)
         {
         k = r1/q;
         r1 = a*(r1-k*q) - p*k;
         if (r1 < 0) r1 = r1 + m;
         if (j < n) r[j] = r1;
           } 
         r0 = r[1];
         } 
/*     beginning when not initializing    */
/*     compute r1 = mod(a*r1,m) without overflows  */
       k = r1/q;
       r1 = a*(r1 - k*q) - p*k;
       if (r1 < 0) r1 = r1 + m;
       j = 1 + r0/ndiv;
       r0 = r[j];
       r[j] = r1;
       return min(rm*r0,rmax);
   }

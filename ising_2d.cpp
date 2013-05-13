#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <fstream>

using namespace std;

#define L 40 // The size of the lattice


// here goes the functions for  my 16807 generator

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
#define N 32
#define ndiv (1 + (m-1)/N)
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

// Here are the operation for the lattice sites 

int prev( int x){ 

// find the previous lattice site using periodicity

  int y;
  if (x==0)
   {y=(L-1);}
 else
   {y=x-1;}
 return(y);
} 


int next( int x){

 // find the next lattice site using periodicity

  int y;
  if (x==(L-1))
   {y=0;}
 else
   {y=x+1;}
 return(y);
} 

void initLattice()

int main(void){

  int spin[L][L], s  ;  //the ising sites
  int *site, *spin_ptr,*lsite,*rsite,*usite,*dsite,*againsite;

  int sum,n,i,j,spinsum,I,k;

  float prob,x,ex,average,Sum;

  ofstream record("ising.dat");


int Sweeps = 12000; // The number of updates

float initT = 0.0, finalT=4.0,T=0.0;

int steps= 50;

int seed = 3509;

 int Ignore=2000; //The number of sweeps between measuring

 float Dt=(finalT - initT)/steps;


 for(k=0;k<=steps;k++){
   Sum=0.0;
  //Let us make all the spins pointing up

//Take down  the address of  the very first site
   site=&spin[0][0]; 

//Using memset to clear the array first
   memset(site,0,sizeof(L));
   //Making All the sips at lattice site pointing up.
  for ( i=0; i<=(L*L-1); i++){
    *(site+i)=1;
  }; 


// Next we have to keep on flipping the spins at sites 
// sequentially and then use Metropolis algorithm to 
// update the flips

  I = seed;

  for(n=1; n<= Sweeps; n++){

    for(i=0;i<=(L-1);i++){ // i fixes the row to sweep across

      for (j=0;j<=(L-1);j++){  // sweeping now along the row

//The address of the site (i+1,j)
       spin_ptr=&spin[i][j]; 
//the address of the site to "north"
       usite=&spin[prev(i)][j]; 
//the address of the site to "south"
       dsite=&spin[next(i)][j]; 
// the address of the site to "east"
       rsite=&spin[i][next(j)]; 
//the address of the site to "west"
       lsite=&spin[i][prev(j)]; 


//find the sum of the spins of the neighbors

       sum=( *usite + *dsite + *rsite + *lsite ) ;

//Now calculate the "energy" associated with these bonds


       ex= (*spin_ptr) * sum;


 //The Boltzmann probability of flipping the spin at site (i,j) 

       prob = exp(-2.0 * ex/T);

//Metropolis says to flip the flip if the Boltzmann probalility of 
//flipping is larger than a random number between 0 and 1,
//else not.
       x= rnd(I) * 16807;
      
       if(prob > x){
         spin[i][j]=-spin[i][j];
       };
      };//swept through the row

    } //here the sweep finishes across the lattice


    //Do not record all the updates for averaging as 
    //we need measurements to be uncorrelated 

    if(n > Ignore){ 

//Next measure the magnetization over the lattice 
//which is the sum of the spins on the lattice sites

      spinsum=0;

      againsite=&spin[0][0];
      for(i=0; i<=(L*L-1);i++){
        spinsum=( spinsum + *(againsite+i) );
      }

      spinsum=abs(spinsum);
      Sum=Sum+spinsum;
    }
  };

  average=Sum/(Sweeps-Ignore);
  record << T << "       " << average << endl;


  T=T+Dt;
}
return (0);
}

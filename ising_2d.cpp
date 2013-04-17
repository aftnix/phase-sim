#include <iostream>
#include<stdlib.h>
#include<cmath>
#include<fstream>

using namespace std;

#define L 40 // The size of the lattice


// here goes the functions for  my 16807 generator

int maxmod( int I){ // defines integers modulo 2^31
  if (I < 0)
    I=I+2147483647+1;
return(I);
}

float nor( float x){ 

//rounds the functions value between 0 and 1

  if (x < 0.0)
    x=x+1.0;
  return(x);
}

// The basic 16807 generator 
 
float my_rand(int I)
{ I=maxmod(I);
 float z=I;
 float x=z/2147483647;
 x=nor(x);
 return(x);
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
        x=my_rand(I);
     	I=I*16807;

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

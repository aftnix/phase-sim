/**
 MC testing on the SHO model
 */
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <sys/time.h>
#include <time.h>
#include <cstdlib>
#include <boost/random.hpp>

using namespace std;
using namespace boost;

//typedef <double> dcmplx;


int N;
const int sampleSize =65536;
double S, SNext;
double H, HNext;
double varSum(0.0);
double sSum(0.0);
double epsilon;



ofstream output_S("S.data");
ofstream output_q("q.data");


double stepSize(0.0);
/*Array of accepted actions*/
double SArr[sampleSize], Cv[sampleSize];


mt19937 boost_rng(getpid()*time(NULL));
normal_distribution <> normdist ; // Distribution
variate_generator < mt19937&, normal_distribution<> > norm_rand(boost_rng, normdist);
                                                     
// Combination of distribution and generator
uniform_01<mt19937> number(boost_rng);
// ***********************************************	      
// * This function is used for getting the current time.
// * 
// * Useful for timing the computations 
// **********************************************

unsigned long GetCurrentTime(){
	struct timeval t;
	gettimeofday(&t,NULL);
	return t.tv_usec/1000 + t.tv_sec*1000;
}

__inline double my_rand(void){
	return(number());
}

struct phase{
  double p;
  double q;
};

//void random_init(struct phase init_phase){
//  init_phase.p=norm_rand();
//  init_phase.q=my_rand();
//}


// This is the procedure if we want to find the 
// maximal error by performing jackknife estimates 
// but removing more than one samples which done by 
// eliminate biasing in the statistical samples

void  multijack_average(double sample[ ],double &average,double &error){


  // Given the samples let us first calculate the average  of the sample

  double sum = 0.0;
  
  // first we sum the samples

  for( int i = 0; i < sampleSize; i++){
    sum += sample[i];
  }

  // So the average is 

  average=sum/sampleSize;


  double  bin_variance_squared, bin_variance, prev_variance=0.0,jacked_sum;

  double jacked_average[sampleSize];

  int k;
 
// Make equal sized  bins  from the samples with  z elements 
// The bin size should be with the range 1 <= z <= (sampleSize-1)
// but a look at the working formula 
//
//  sigma= \sqrt{ \frac{k-1}{k} \Sum^k_{i}(x -x_i)^2}
//
// shows that k >=2 to get non-zero variance. So, it is meaningless to take 
// binsizes bigger that half of the sample size.


for(int z=1;z<=sampleSize/2; z++){


 k= sampleSize / z; // This is the number of bins produced

bin_variance_squared=0;

 
  // Now pick m-th bin and then calculate the average after removing the 
  // z samples in that bin 


 for(int m=0;m<k;m++){

   // To find the sum after removing the sample in the m-th sample 
   // we remove the samples from the total sum
  
   jacked_sum=sum;
   for(int n=0;n<z;n++){
     jacked_sum -= sample[m*z +n]; 
   }
 
    // So the average after removing the m-th bin ( of size z ) is gived by

    jacked_average[m]=(jacked_sum)/(sampleSize - z);
   
 // So now we calculate the variance by summing the squares of the deviations

    bin_variance_squared += (jacked_average[m] - average)*(jacked_average[m]- average);
  
 }

 // So the variance over this bin is 

 bin_variance = sqrt((k-1)*(bin_variance_squared)/k);

//cout << "The bin variance is "<<bin_variance<< " with binsize "<< z << endl;
//cout << endl;  

//We are interested in the largest of all the variances
//
// This is found by 


  if( bin_variance >= prev_variance){

   prev_variance=bin_variance;

  }
 }
 error=prev_variance;
 // cout << " The maximal error is " << error << endl;
}

/**
 *printS() prints the selected action to files
 */
void printS(int written){
	double sum(0.0);
	//	double squareSum(0.0);
	double varTemp(0.0);
	int choppedSampleSize = written/2;
	for(int i = written/2; i < written; i++){
		sum += SArr[i];
		//squareSum += SArr[i]*SArr[i];
		//outputFile4<<theta<<"    "<<SArr[i]<<"\n"<<flush;
	}
	double mean = sum/choppedSampleSize;
	for(int i = written/2; i < written; i++){
		varTemp += pow((SArr[i] - mean), 2.0);
	}
	double variance = varTemp/choppedSampleSize;
    //double variance = pow(sigmaJack(), 2.0);
	
	varSum += variance;
	sSum += mean;
	
}



/**
 *hamiltonian() calculates the Hamiltonian of the system
 */
double hamiltonian(struct phase my_phase){
  return( 0.5*((my_phase.p)*(my_phase.p) + ( my_phase.q)*(my_phase.q)));
}

/**
 *metropolis() gives final verdict for acceptance of the new state 
 */
bool metropolis(double H, double HNext){
	double prob = my_rand();
	double min;
	if(H > HNext) min = 1.0;
	else{ 
	  min = exp(H - HNext);
	}
	return (min > prob);
}



/**
 *leapfrog() performs leap frog forward/backward in time
 */

struct phase leapfrog(struct phase phase_old, double dt){
  struct phase phase_new;
 double  p_Half = phase_old.p  - (dt / 2.0) * phase_old.q;
 phase_new.q = phase_old.q  + dt * p_Half;
 phase_new.p= phase_old.p - dt *( phase_new.q + phase_old.q )/2.0;
 return(phase_new);
}

/**
 *findAction()
 */
void findAction(){

  struct phase tmp_phase, my_new_phase;

  // pick an initial state

struct phase  init_phase={norm_rand(), 2.0*my_rand()-1.0}; 

	int num_steps;

	int s=0, k=0;

	while(s < sampleSize){

	  
// Evaluate Hamiltonian for the initial state

	  H=hamiltonian(init_phase);
	  //	  cout << H <<endl;
	  
	  // We now evolve the system to its new state so pick 
	  // direction of time and magnitude of the evolution time

	  double dt=(2*my_rand()-1.0);
	
	  // We split up the evolution in a number of steps which is 
	  // picked at random

	  num_steps = (int)(10*my_rand());
	  //store the initial state in a temporary state 
	  tmp_phase=init_phase;

	  // Carry out leapfrog for number of steps = num_steps 
	for(int j=1; j<= num_steps; j++){
	  my_new_phase=leapfrog(tmp_phase,dt/num_steps);
	  tmp_phase=my_new_phase;
	}
	my_new_phase=tmp_phase;
	//Find the  Hamiltonian for the new state
	HNext=hamiltonian(my_new_phase);
	//perform the metropolis test
	if(metropolis(H,HNext)== 1){
	  // if the test is passed
	  // record the potential and update the initial state
	  output_q << my_new_phase.q << endl;
	  SArr[s]=0.5*(my_new_phase.q)*(my_new_phase.q);
	  output_S <<SArr[s]<< endl;
	  // replace the initial state with the accepted state
	  init_phase=my_new_phase;
	  s++;
	}
	// else change the initial state 
	else{
	  init_phase.p=norm_rand();
	  init_phase.q=2*my_rand()-1;
	}
	k++;
	}
	cout << "The acceptance ratio is " << ((double)sampleSize)/k <<endl;
}

/**
 *main() the main method
 */
int main(void){
  findAction();
  double CV, S_av,S_error,diff,Cv_error;
  multijack_average(SArr,S_av,S_error);
  for(int k=0;  k< sampleSize; k++){
    diff=S_av - SArr[k];
    Cv[k]=(diff*diff);
  }
  multijack_average(Cv,CV,Cv_error);
  cout <<"S_av = "<<S_av <<" with error " << S_error<< endl;
  cout <<"C_v = "<<CV <<" with error " << Cv_error<< endl;
	return 0;
}

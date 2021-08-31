#include <omp.h>
#include <stdio.h>
#include <math.h>

#define NUMT 1
#define NUMT1 4
#define SIZE 16384     // you decide
#define NUMTRIES 15    // you decide

float A[SIZE];
float B[SIZE];
float C[SIZE];

int main(){
#ifndef _OPENMP
  fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
  return 1;
#endif

  // inialize the arrays:
	for( int i = 0; i < SIZE; i++ ){
	  A[ i ] = 1.;
		B[ i ] = 2.;
	}

  omp_set_num_threads(NUMT);
  fprintf(stderr, "\nUsing %d threads\n", NUMT);

  double maxMegaMults = 0.;
  double summflops = 0.;

  for(int t = 0; t < NUMTRIES; t++){
    double time0 = omp_get_wtime();

    #pragma omp parallel for
    for(int i = 0; i < SIZE; i++){
      C[i] = A[i] * B[i];
    }

    double time1 = omp_get_wtime();
    double megaMults = (double)SIZE/(time1-time0)/1000000.;
    summflops += megaMults;
    if(megaMults > maxMegaMults)
      maxMegaMults = megaMults;
  }

  printf("Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults);
  printf("Average Performance = %8.2lf MegaMults/Sec\n", summflops/(double)NUMTRIES);

	//note: %lf stands for "long float", which is how printf prints a "double"
	//%d stands for "decimal integer", not "double"


/************************ 4 Threads ************************/
 
  omp_set_num_threads(NUMT1);
  fprintf(stderr, "\nUsing %d threads\n", NUMT1);

  double maxMegaMults1 = 0.;
  double summflops1 = 0.;

  for(int t = 0; t < NUMTRIES; t++){
    double time01 = omp_get_wtime();

    #pragma omp parallel for
    for(int i = 0; i < SIZE; i++){
      C[i] = A[i] * B[i];
    }

    double time11 = omp_get_wtime();
    double megaMults1 = (double)SIZE/(time11-time01)/1000000.;
    summflops1 += megaMults1;
    if(megaMults1 > maxMegaMults1)
      maxMegaMults1 = megaMults1;
  }

  printf("Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults1);
  printf("Average Performance = %8.2lf MegaMults/Sec\n", summflops1/(double)NUMTRIES);



  //1 to 4
  double speedup = maxMegaMults1/maxMegaMults;
  //4 to 1
  //double speedup1 = maxMegaMults/maxMegaMults1;
  
//  printf("\nSpeed up is %f for 1-thread-to-4-thread\n", speedup);
  printf("Speed up is %f for 4-thread-to-1-thread\n", speedup);
  float FP = (4./3.)*(1.-(1./speedup));
  printf("Parallel fraction is %f\n\n", FP);
  
  return 0;
}

#include "simd.p5.h"

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>

#include <omp.h>

//'size' causes problems..
float a[Size];
float b[Size];
float c[Size];

int main(){
  int sum;
	float rando = 0.;
  
	double simdBegin = 0;
	double simdEnd = 0;
	double ownBegin = 0;
	double ownEnd = 0;
	double reductBegin = 0;
	double reductEnd = 0;
	double ownReductBegin = 0;
	double ownReductEnd = 0;
	
  //Multiplication
  //Run SSE SIMD
  //get start time
	simdBegin = omp_get_wtime();

  //multiplies a and b to get c, Size times
	SimdMul(a, b, c, Size);
	simdEnd	= omp_get_wtime();		

	//Run own code
	ownBegin = omp_get_wtime();
 
  //calculate multiplication
	for(int i = 0; i < Size; i++){
		c[i] = a[i]*b[i];
	}
 
  //get end time
	ownEnd = omp_get_wtime();
  rando = ((ownEnd-ownBegin)/(simdEnd-simdBegin));
  printf("SIMD,%.5f,%d\n",rando,Size);
  
  /**************************************************************************/
  
  //Reduction
	//run SSE SIMD Reduction
  //get start time
	reductBegin = omp_get_wtime();
	
  //gets sum of a and b, Size times
  SimdMulSum(a, b, Size);
  //get end time
	reductEnd = omp_get_wtime();
		
	//run own code reduction
  //set sum to 0 to start with clean every run
	sum = 0;
  //get start time
	ownReductBegin = omp_get_wtime();

  //calculate reduction
	for(int i = 0; i < Size; i++){
		sum+=a[i]+b[i];
	}
 
  //get end time
	ownReductEnd = omp_get_wtime();
  rando = ((ownReductEnd-ownReductBegin)/(reductEnd-reductBegin));
  printf("Reduction:,%.5f,%d\n",rando,Size);
	return 0;
}
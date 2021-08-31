#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <cmath>

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

#ifndef NUMT
#define NUMT		   2
#endif

#ifndef NUMNODES
#define NUMNODES		   1000
#endif

#define N	0.70

float Height(int, int);	// function prototype
enum Tile {Center, Edge, Corner};

Tile getTileType(int, int);

//(x^N)+(y^N)+(z^N)=1

float Height(int iu, int iv)/*iu,iv = 0 .. NUMNODES-1*/{
	float x = -1. + 2.*(float)iu/(float)(NUMNODES-1);	// -1. to +1.
	float y = -1. + 2.*(float)iv/(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow(fabs(x), (double)N);
	float yn = pow(fabs(y), (double)N);
	float r = 1. - xn - yn;
 
	if(r <= 0.)
    return 0.;
	float height = pow(r, 1./(float)N);
	return height;
}

Tile getTileType(int iv, int iu){
  if (iu > 0 && iu < NUMNODES - 1 && iv > 0 && iv < NUMNODES - 1){
    return Center;
  }
  else if ((iu == 0 || iu == NUMNODES - 1) && (iv == 0 || iv == NUMNODES - 1)){
    return Corner;
  }
  return Edge;
}


int main(int argc, char *argv[ ]){

  #ifndef _OPENMP
    fprintf(stderr, "No OpenMP support detected.\n");
    return 1;
  #endif
  
  double TotalVolume = 0;
  float height;
  double time0 = omp_get_wtime();
  
  
  omp_set_num_threads(NUMT);
  //omp_set_num_threads(NUMNODES);
  
  float dx = ((XMAX - XMIN)/(float)(NUMNODES-1));
  float dy = ((YMAX - YMIN)/(float)(NUMNODES-1));
  float fullTileArea = (dx * dy);
    
  
  double megaHeightsPerSecond = 0;
  float maxPerformance = 0;

  float maxPerformance_volume = 0;

	// the area of a single full-sized tile:
	// (not all tiles are full-sized, though)
 

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:

  /*for(int vs = 0; vs< 2; vs++){ 
    maxPerformance = -5;
    TotalVolume = 0;*/
  //printf("HI\n");
    #pragma omp parallel for collapse(2) default(none), reduction(+:TotalVolume), shared(fullTileArea)
    //vertical
    for(int iv = 0; iv < NUMNODES; iv++){
    	//horizontal
      for(int iu = 0; iu < NUMNODES; iu++){
      
        double volume = 0;
    		Tile testTile = getTileType(iv, iu);
        float dx = ((XMAX - XMIN)/(float)(NUMNODES-1));
        float dy = ((YMAX - YMIN)/(float)(NUMNODES-1));
      	float fullTileArea = (dx * dy);
       
        volume = Height(iu,iv)*fullTileArea*2;
        
        if(testTile == Center){
          //printf("Center\n");
          volume = volume;
          TotalVolume += volume;
        }
        else if(testTile == Edge){
          //printf("Edge\n");
          volume = volume/ 2.0;
          TotalVolume += volume;
        }
        else{
          //printf("Corner\n");
          volume = volume/ 4.0;
          TotalVolume += volume;
        }
    	}
    }
    double time1 = omp_get_wtime();
    
    megaHeightsPerSecond = (double) ((NUMNODES*NUMNODES)/ (time1-time0)/1000000.);
    
    if(megaHeightsPerSecond > maxPerformance){
      maxPerformance = megaHeightsPerSecond;
    }
    
    printf("Thread %d,Nodes %d,Perfomance %f,Volume %f\n", NUMT, NUMNODES, maxPerformance, TotalVolume);
 /* }
  printf("\n");*/
}
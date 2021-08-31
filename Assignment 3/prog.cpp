#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <cmath>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <omp.h>

int	NowYear;		// 2021 - 2026
int	NowMonth;		// 0 - 11
int numMonth = 1;

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population
int NowNumHuman;

const float GRAIN_GROWS_PER_MONTH =		9.0;
const float ONE_DEER_EATS_PER_MONTH =		1.0;
const float ONE_HUMAN_EATS_PER_MONTH =  .75;

const float AVG_PRECIP_PER_MONTH =		7.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				60.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;

unsigned int seed = 0;


float Ranf(unsigned int *seedp,  float low, float high){
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}

//float x = Ranf( &seed, -1.f, 1.f );

float SQR(float x){
  return x*x;
}

//keeps running from year 2021 to 2026
void Watcher(){
  while(NowYear < 2027){
    #pragma omp barrier
    #pragma omp barrier
    float ang = (30. * (float)NowMonth + 15.) * (M_PI/180.);
    float temp = AVG_TEMP - AMP_TEMP * cos(ang);
    float FtoC;
    
    NowTemp = temp + Ranf(&seed, -RANDOM_TEMP, RANDOM_TEMP);
    //NowTemp = (NowTemp - 32.)*(5./9.);
    FtoC = (NowTemp- 32.)*(5./9.);
    
    
    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
    
    NowPrecip = precip + Ranf(&seed, -RANDOM_PRECIP, RANDOM_PRECIP);
    if(NowPrecip < 0.){
      NowPrecip = 0.;
    }
    //error checking purpose
    /*printf("%d,%d,Deer %d,Human %d,Height %f,Temp %f,Percip %f\n",NowYear,NowMonth,NowNumDeer,NowNumHuman,NowHeight,FtoC,NowPrecip);*/
    
    printf("%d,%d,%d,%f,%f,%f\n",numMonth,NowNumDeer,NowNumHuman,NowHeight,FtoC,NowPrecip);
        
    if(NowMonth == 11){
      NowMonth = -1;
      NowYear++;
    }
    numMonth++;
    NowMonth++;
    #pragma omp barrier
  }
}

//growth of deer
void Deer(){
  while(NowYear < 2027){
    int NextNumDeer = NowNumDeer;
    
    NextNumDeer -= (int)((float)NowNumHuman * ONE_HUMAN_EATS_PER_MONTH);
    int carryingCapacity = (int)(NowHeight);
    
    if(NextNumDeer < carryingCapacity){
      NextNumDeer++;
    }
    else if (NextNumDeer > carryingCapacity){
      NextNumDeer--;
    }
    if(NextNumDeer < 0){
      NextNumDeer = 0;
    }
    
    #pragma omp barrier
    NowNumDeer = NextNumDeer;
    
    #pragma omp barrier
    #pragma omp barrier
  }
}

//growth of grain
void Grain(){
  while(NowYear < 2027){
    
    float nextHeight = NowHeight;
    float tempFactor = exp(-SQR((NowTemp - MIDTEMP)/10.));
    float precipFactor = exp(-SQR((NowPrecip - MIDPRECIP)/10.));
    
    nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
    nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
    
    if(nextHeight < 0.){
      nextHeight = 0.;
    }
    
    #pragma omp barrier
    NowHeight = nextHeight;
    #pragma omp barrier
    #pragma omp barrier
  }
}

/*there's a 40% chance that a human will be infected by a zombie
  so if a person is turned into a zombie then the deer population
  will grow by one. But if the zombie is defeated before it can
  infect a human than one deer is eaten*/

void Zombie(){
  while(NowYear < 2027){
    int ZombieHuman = NowNumHuman;
    int AliveDeer = NowNumDeer;
    #pragma omp barrier
    if(NowMonth % 12 == 0 || NowMonth % 12 == 6){
      int percent = rand() % 101;
      if(percent<= 40){
        //oh no! Someone became a zombie
        ZombieHuman--;
        AliveDeer++;
      }
      else if(percent>40){
        //yay time to eat an extra deer!
        ZombieHuman++;
        AliveDeer--;
      }
    }
    
    
    int foodnum = (int)((float)NowNumHuman * ONE_HUMAN_EATS_PER_MONTH);
    if(foodnum<AliveDeer){
      ZombieHuman--;
      AliveDeer++;      
    }
    #pragma omp barrier
    NowNumDeer = AliveDeer;
    NowNumHuman = ZombieHuman;
    /*if(NowNumHuman == 2){
      //make more humans!!!
      NowNumHuman = 4;
      NowNumDeer = NowNumDeer - 2;
    }*/
    #pragma omp barrier
  }
}


int main(int argc, char**argv){

  #ifndef _OPENMP
  	fprintf(stderr, "OpenMP is not available\n");
  	return 1;
  #endif

  //starting values
  //added extra humans
  NowNumHuman = 10;
  NowNumDeer = 1;
  NowHeight = 10.;
	NowMonth = 0;
	NowYear = 2021;
  
  omp_set_num_threads(4);
	#pragma omp parallel sections default(none)
	{
		
		#pragma omp section
		{
			Deer();
		}
		#pragma omp section
		{
			Grain();
		}

		#pragma omp section
		{
			Watcher();
		}
		#pragma omp section
		{
			Zombie();
		}
	}
	return 0;   

}
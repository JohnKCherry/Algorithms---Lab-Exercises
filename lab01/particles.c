/***************************************************************/
/*                   Algorithms and Complexity                 */
/*                     First Lab Exercise                      */
/*                                                             */
/* Exercise 2: Particles                                       */
/*                                                             */
/* Full Name: Kerasiotis Ioannis, Student ID: 03114951         */
/*                                                             */
/***************************************************************/



/******************Libraries & Definitions**********************/
#include <stdio.h>
#include <stdlib.h>
#define BSIZE 1<<15
/***************************************************************/


/*********************Global Variables**************************/
char buffer[BSIZE];
long long bpos = 0L, bsize = 0L;
/***************************************************************/

/********************Functions**********************************/
long long readLong();

int main(int argc, char **argv){
	
	long long int N = readLong(); /*  Number of particles */
	long long int L = readLong(); /* Length of Pipe */
	long long int K = readLong(); /* Number of collissions we want */
	
	long long int i;
	
	long long int** AArray = (long long int **)malloc(N * sizeof (long long int *));
	long long int** BArray = (long long int **)malloc(N * sizeof (long long int *)); /* Two teams' arrays allocation */
	
	
	for (i=0; i<N; i++){
		
		AArray[i] = (long long int *)malloc(3*sizeof(long long int));
		
		AArray[i][0] = readLong(); /* start time */
		AArray[i][1] = readLong(); /* velocity */
		AArray[i][2] = 1; /* a flag if it is alive */
	}
	
	for (i=0; i<N; i++){
		
		BArray[i] = (long long int *)malloc(3*sizeof(long long int));
		
		BArray[i][0] = readLong(); /* start time */
		BArray[i][1] = readLong(); /* velocity */
		BArray[i][2] = 1; /* a flag if it is alive */
	}	

	long double timeCollision; /* variables that stores the value of time when there is collision */
	
	long long int previousA, currentA; /* indexes about collisions (A) */
	long long int currentB; /* indexes about collisions of a particle (B) */
	
	currentA = 0;
	
	long long int collisions; /* number of collisions which are done */
	
	long double numerator; /* numerator for equatation are used for casting*/
	long double denominator; /* denominator for equatation are also used for casting */
	
	
	for (collisions=0; collisions<K ; collisions++){ /* for the K first collisions */
		
		do{
			
			previousA = currentA; /* set the previous A-particle */
			
			timeCollision = 2 * 1000000000; /* max time for collisons */
			
			for(i=0; i<N; i++){ 
				if(BArray[i][2]){ /* if it is not dead */
					numerator = L + AArray[currentA][0] * AArray[currentA][1] + BArray[i][0] * BArray[i][1];
					denominator = AArray[currentA][1] + BArray[i][1];
					if(timeCollision > (numerator) / (denominator)) { /* find the B-particle that collide with previousA */
						timeCollision = (numerator) / (denominator);
						currentB=i;
					}
				}
			}
			
			timeCollision = 2 * 1000000000;
			
			for(i=0; i<N; i++){
				if(AArray[i][2]){ /* in same way we find the the A particle that it collidates with currentB */
					numerator = L + AArray[i][0] * AArray[i][1] + BArray[currentB][0] * BArray[currentB][1];
					denominator = AArray[i][1] + BArray[currentB][1];
					if(timeCollision > (numerator) / (denominator)) {
						timeCollision = (numerator) / (denominator);
						currentA=i;
					}
				}
			}
				
		}while(previousA != currentA); /* if the previous is the same with the current means tha we found the 2 particles which collidate firtsly */
			
		printf("%lld %lld\n", currentA+1, currentB+1); /* output at standar output */
		
		AArray[currentA][2] = 0; /* set them inactive */
		BArray[currentB][2] = 0;
	}
			
	return 0;
	
}


long long readLong() {
	long long d = 0L, x = 0L;
	char c;

	while (1)  {
		if (bpos >= bsize) {
			bpos = 0;
			if (feof(stdin)) return x;
			bsize = fread(buffer, 1, BSIZE, stdin);
		}
		c = buffer[bpos++];
		if (c >= '0' && c <= '9') { x = x*10 + (c-'0'); d = 1; }
		else if (d == 1) return x;
	}
	return -1;
}

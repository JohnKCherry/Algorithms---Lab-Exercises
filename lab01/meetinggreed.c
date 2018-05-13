/***************************************************************/
/*                   Algorithms and Complexity                 */
/*                     First Lab Exercise                      */
/*                                                             */
/* Exercise 1: Meeting Greed                                   */
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
void initArray(long long int *array, long long int N);
void findDistances (long long int *coordinateArray,long long int N, long long int *distance);
void quickSort (long long int **array, long long int low, long long int high);
long long int partition (long long int **array, long long int low, long long int high);
void swap(long long int* a, long long int* b);
long long int** calculateDistance(long long int **array, long long int N);
void solve(long long int* distance, long long int N);
/***************************************************************/

int main(int argc, char **argv){
	
	long long int N;
	
	N = readLong(); /* Read the Number N of the Student of the Town */
	
	long long int coordinatesXArray[N], coordinatesYArray[N], i;
	
	for (i=0; i<N; i++){ /* Read the Coordinates of Students' Houses on the Grid */
		coordinatesXArray[i] = readLong();
		coordinatesYArray[i] = readLong();
	}
	
	long long int* distance = (long long int*)malloc(N*sizeof(long long int));	/* An Array that stores the distance of a house from other student houses*/
	
	initArray(distance, N); /* Initialization of Array Distance */
	
	findDistances(coordinatesXArray, N, distance); /* Find the distance of a house from other houses on X-Axis and add them at array with distance */ 
	findDistances(coordinatesYArray, N, distance); /* Find the distance of a house from other houses on Y-Axis and add them at array with distance */
	
	solve(distance, N); /* Find the the minimum distance */
	
	free(distance); /* Deallocate the previous allocated Memory */

	return 0;
}

/***************************************************************/


/********************************************************************/
/*                        readLong                                  */
/* The readLong is a function which read from std input integers    */
/* and convert them to long long integers.                          */
/* This function is given from:                                     */
/* http://www.corelab.ntua.gr/courses/algorithms/problemsets/read.c */
/********************************************************************/

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

/********************************************************************/
/*                        initArray                                 */
/* This function aims to init a one-dimensional type int array.     */
/********************************************************************/

void initArray(long long int *array, long long int N){
		
		long long int i;
		
		for (i=0; i<N; i++){
			array[i]=0;
		}
}

/********************************************************************/
/*                       findDistances                              */
/* This function takes as argument a array with coordinates and     */
/* calculate the distances of other elements from this one.         */
/* Firstly, sort the array and then use the algorithm for minimum   */
/* Manhattan Distances. Finally, the distances are added at         */
/* Distance Array.                                                  */
/********************************************************************/

void findDistances (long long int *coordinateArray, long long int N, long long int *distance){
	
	long long int i;
	long long int** array = (long long int **)malloc(N*sizeof(long long int *)); /* Allocate 2-dimensional array for temporary storing of coordination and their index */
	
	for (i=0; i<N; i++){
		array[i] = (long long int *)malloc(2*sizeof(long long int));
		}
	
	for (i=0; i<N; i++){
		array[i][0] = coordinateArray[i];
		array[i][1] = i;
	}
	
	quickSort(array, 0, N-1); /* Sort the temporary array in order its thesis */
	long long int** distArray = calculateDistance(array, N); /* Allocation of array that stores minimum Manhattan Distance */
	
	for (i=0; i<N; i++){
		distance[distArray[i][1]] += distArray[i][0]; /* Add them to overall Distance */
	}
	
	free(array); /* Deallocate the previous allocated Memory */
	free(distArray); /* Deallocate the previous allocated Memory */
	
}

/********************************************************************/
/*                quickSort, partition and swap                     */
/* The functions is for QuickSort algorithm at a two-dimensional    */
/* array, in order to swap the houses' index too                    */
/********************************************************************/

void quickSort(long long int **array,long long int low, long long int high){
	
	long long int pi;
	
	if (low<high){
		 pi=partition(array, low, high);
		 quickSort(array, low, pi-1);
		 quickSort(array, pi+1, high);
	 }
}

long long int partition (long long int **array, long long int low, long long int high){
	
	long long int pivot = array[high][0]; 
	long long int i = low - 1;
	long long int j;
	
	for (j = low; j<high;j++){
		if (array[j][0]<=pivot){
			i++; 
			swap(&array[i][0], &array[j][0]);
			swap(&array[i][1], &array[j][1]);
		}
	}
	swap(&array[i+1][0], &array[high][0]);
	swap(&array[i+1][1], &array[high][1]);

	return (i+1);
}

void swap(long long int* a,long long int* b){
    long long int t = *a;
    *a = *b;
    *b = t;
}

/********************************************************************/
/*                     calculateDistance                            */
/* This function calculate the minimum distances of a point from    */
/* other points.                                                    */
/********************************************************************/

long long int** calculateDistance(long long int **array, long long int N){
	
	int i,j;
	long long int** dest = (long long int **)malloc(N*sizeof(long  long int *));
	
	for (i=0; i<N; i++){
		dest[i] = (long long int *)malloc(2*sizeof(long long int));
		}
		
	for (i=0; i<N; i++){
		if (i == 0) {
			for (j = 1; j<N; j++){
				dest[0][0] += (array[j][0]-array[0][0]); /* Calculate the Distance from first house in order */
				
			}
		}
		else{
			 dest[i][0] = dest[i-1][0]-(N-i)*(array[i][0]-array[i-1][0])+i*(array[i][0]-array[i-1][0]); /* Calculate the other Distances */
		}
		
		dest[i][1]=array[i][1]; /* Store Indexes */
	}

	
	
	return dest; /* Deallocate the previous allocated memory */
}

/********************************************************************/
/*                             solve                                */
/* As the name says, this function finds the minimum distance and   */
/* print it at the standard output.                                 */
/********************************************************************/

void solve(long long int* distance, long long int N){
	
	int i;
	long long int min=distance[0];
	for (i=1; i<N; i++){
		if (min>distance[i]) min=distance[i];
	} 
	
	printf("%lld\n",min);
}
σ

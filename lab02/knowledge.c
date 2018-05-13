/***************************************************************/
/*                   Algorithms and Complexity                 */
/*                    Second Lab Exercise                      */
/*                                                             */
/* Exercise 2: Knowledge                                       */
/*                                                             */
/* Full Name: Kerasiotis Ioannis, Student ID: 03114951         */
/*                                                             */
/***************************************************************/


#include <stdio.h>
#include <stdlib.h>
#define BSIZE 1<<15

/*********************Global Variables**************************/
char buffer[BSIZE];
long int bpos = 0L, bsize = 0L;
/***************************************************************/

/********************Functions**********************************/
long int readLong();
void find_maxProfit(long int *c, long int N, long int L);
void swap(long **a, long **b);
/***************************************************************/

int main(int argc,char **argv){
	
	long int i;
	long int N = readLong(); // input Number of Villages
	long int L = readLong(); // input Number of Transactions
	
	long int* c = (long int* )malloc(N * sizeof(long int)); // Allocate Memory for Prices
	for(i=0; i<N; i++){
		c[i] = readLong(); // input Price of Book
	}
	
	find_maxProfit(c, N, L); // Solution of the problem
		
	free(c); // Deallocate the allocated Memory
	
	return 0;
}


/********************************************************************/
/*                        readLong                                  */
/* The readLong is a function which read from std input integers    */
/* and convert them to long long integers.                          */
/* This function is given from:                                     */
/* http://www.corelab.ntua.gr/courses/algorithms/problemsets/read.c */
/********************************************************************/

long int readLong() 
{
	long int d = 0, x = 0;
	long int sign = 1;
	char c;

	while (1)  {
		if (bpos >= bsize) {
			bpos = 0;
			if (feof(stdin)) return x;
			bsize = fread(buffer, 1, BSIZE, stdin);
		}
		c = buffer[bpos++];
		if (c == '-') sign = -1;
		if (c >= '0' && c <= '9') { x = x*10 + (c-'0'); d = 1; }
		else if (d == 1) return sign * x;
	}
	return -1;
}

/********************************************************************/
/*                      find_maxProfit                              */
/* find_maxProfit is a function that takes as arguments the number  */
/* of Villages, the number of transactions and the array with the   */
/* price of the boooks. It creates an array in order to solve the   */
/* problem with use of Dynamic Programming, solving the simplier    */
/* subproblem.                                                      */
/********************************************************************/

void find_maxProfit(long int *c, long int N, long int L){
	
	long int i,j;
	
	/* Allocation the main array that is used for find the solutions */
	/* of subproblems. Firstly, for the solution there was a use of  */
	/* a 2d-array with (L+1) x (N+1) dimensions. Then, I decide to   */
	/* the dimensions of Array to 2 x (N+1), due to using only the   */
	/* 2 vectors are used. In this way, there was reducing of not    */
	/* only the memory, but also the time, reduced by 0.3 sec. If    */
	/* there's need of reducing time we could declare the arrays     */
	/* statically instead of allocating dynammically the memory.     */
	
	long int** profit = (long int **)malloc(2 * sizeof(long int *)); 
	for(i=0; i<2; i++){
		profit[i] = (long int *)malloc((N+1) * sizeof(long int));
	}
	
	for (i = 0; i < 2; i++){
        profit[i][0] = 0; // If there is not any village, there is not profit
	}
	
	for (i = 0; i < N+1; i++){
		profit[0][i] = 0; // If there is not any transaction, there is also not profit
	}
	
	
	for (i=1; i<L+1; i++){ // For L+1 transactions
		long int tempMax = profit[0][0]-c[0];
		for (j=1; j < N; j++){
			if((profit[0][j-1] - c[j-1]) > tempMax) tempMax = profit[0][j-1] - c[j-1]; 
			if((tempMax + c[j]) > profit[1][j-1]) profit[1][j] = tempMax + c[j]; // Find for these condittions the maximum profit
			else profit[1][j] = profit[1][j-1]; 
		}
		swap(profit, profit+1); // Q'(t) = Q(t-1)
	}
	printf("%ld\n", profit[0][N-1]); // When I used (L+1) x (N-1), the result was the elemene profit[L][N-1]
	free(profit); // Deallocate the allocated memory
}	

void swap(long int **a, long int **b){
	long int *temp = *a;
	*a = *b;
	*b = temp;
}

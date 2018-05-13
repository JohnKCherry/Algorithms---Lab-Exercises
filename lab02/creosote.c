/***************************************************************/  
/*                   Algorithms and Complexity                 */  
/*                    Second Lab Exercise                      */  
/*                                                             */  
/* Exercise 2: Creosote                                        */  
/*                                                             */  
/* Full Name: Kerasiotis Ioannis, Student ID: 03114951         */  
/*                                                             */  
/***************************************************************/  
  
#include <stdio.h>  
#include <stdlib.h>  
#define BSIZE 1<<15  
  
/*********************Global Variables**************************/  
char buffer[BSIZE];  
long long int bpos = 0L, bsize = 0L;  
  
/***************************************************************/  
  
/********************Functions**********************************/  
long long int readLong();  
long long int calc(long long int x, long long int a, long long int b, long long int c);  
/**************************************************************/  
  
/***************************************************************/  
/* For this exercise I used the hint given by Konstantinos     */  
/* Ameranis at 9th of November about 'Convex Hull Trick and    */  
/* this site https://wcipeg.com/wiki/Convex_hull_trick         */ 
/***************************************************************/  
  
  
int main(int argc, char **argv){  
      
    long long int i , j;  
  
    long long int N = readLong(); //input of number of dishes  
    long long int a = readLong(); //input of a   
    long long int b = readLong(); //input of b  
    long long int c = readLong(); //input of c  
      
    long long int* x = (long long int *)malloc(N * sizeof(long long int)); //allocate memory of Array x (enjoyments)  
      
    for(i=0; i<N; i++)   x[i] = readLong(); //input of x  
      
    long long int* MIAM = (long long int *)malloc((N+1) * sizeof(long long int)); //allocate memory for MIAM (optimal selection's result)  
    long long int sum, totalEnjoyment;    
      
    MIAM[0] = 0; // if dishes are 0, enjoyment is 0 :(  
      
    for(i=1; i<N+1; i++){  // for every element of MIAM  
          
        sum = x[i-1]; // obviously the first plate contains only the dish x[i-1]  
        totalEnjoyment = calc(x[i-1], a, b, c) + MIAM[i-1];   
        for(j=i-1; j>0; j--){ //calculate the max possible pleassure for the current dishes  
            sum += x[j-1];   
            if(totalEnjoyment < (calc(sum, a, b, c) + MIAM[j-1])) totalEnjoyment = (calc(sum, a, b, c) + MIAM[j-1]); // calculate the best possible option  
        }  
        MIAM[i] = totalEnjoyment; // the best option for current dish  
    }  
      
    printf("%lld\n", MIAM[N]); //output of best option  
      
    free(MIAM); //Deallocate the allocated memory  
    free(x);  
      
    return 0;  
}  
  
  
/********************************************************************/  
/*                        readLong                                  */  
/* The readLong is a function which read from std input integers    */  
/* and convert them to long long integers.                          */  
/* This function is given from:                                     */  
/* http://www.corelab.ntua.gr/courses/algorithms/problemsets/read.c */  
/********************************************************************/  
  
  
long long int readLong() {  
    long long int d = 0L, x = 0L;  
    char c;  
    int sign_flag=1;  
  
    while (1)  {  
        if (bpos >= bsize) {  
            bpos = 0;  
            if (feof(stdin)) return x;  
            bsize = fread(buffer, 1, BSIZE, stdin);  
        }  
        c = buffer[bpos++];  
        if (c == '-') sign_flag= -1; // a small modification for negative integers   
        if (c >= '0' && c <= '9') { x = (x*10 + (c-'0')); d = 1; }  
        else if (d == 1) return sign_flag * x;  
    }  
    return 0;  
}  
  
  
/********************************************************************/  
/*                        readLong                                  */  
/* Just a function that calculates the sum a * (x^2) + b * x + c    */  
/********************************************************************/  
  
long long int calc(long long int x, long long int a, long long int b, long long int c){  
      
    long long int result = a * x * x + b *x + c;  
    return result;  
}  

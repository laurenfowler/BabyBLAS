/**********************************************************************
 *
 * ITERATIVE LINEAR SOLVER
 *
 * Andrew J. Pounds, Ph.D.
 * Spring 2018
 *
 * Unless otherwise noted, all code and methods belong to the author.
 * Equations for the Jacoby iterative solver we adapted from Golub
 * and van Loan, "Matrix Computations", Johns Hopkins University press,
 * 1996. 
 *
 **********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
    void ils_( int *threads, int *len,  double *a, double *b, double *x );
#ifdef __cplusplus
}
#endif

#include <math.h>  
#include <stdio.h>  
#include <stdlib.h>
#include <pthread.h>


// Need the following prototype in case of a zero along the diagonal
void  dls_( int *threads, int *len, double *a, double *b, double *x );

// Prototype for code to check for zeros along the diagonal
int zerosAlongDiagonal ( int N, double *a );

// Prototype for code to check for convergence
int converged( int N, double *a, double *b);

void *ils_thread_worker();

struct args{
	int N;
	int startRow;
	int stopRow;
	int iter;
	double *aPtr;
	double *bPtr;
	double *xPtr;
	double *x0ptr;
};

void ils_( int *threads, int *len,  double *a, double *b, double *x ){

    int i, j, k, N, iteration;
    double sum1, sum2;
    double ZERO = 0.0;
    int ITERATION_MAX = 2000;
    double *x0;

	int num_threads = *threads;
	int start, stop;	
	int *num_rows;
	pthread_t *thread_id;
	struct args *thread_args;

    N = *len;

	thread_id = (pthread_t *) malloc (num_threads * sizeof(pthread_t));
	num_rows = (int *) malloc (num_threads * sizeof(int));

	if ( ! zerosAlongDiagonal( N, a ) ) {

			x0 = malloc( N * sizeof(double) );

			for (i=0;i<N;i++) *(x+i) = 0.0;

			for (i=0;i<N;i++) *(x0+i) = *(b+i);

			ITERATION_MAX = fmax(ITERATION_MAX, N/3);

			iteration = 0;

			for (i=0;i<N;i++) *(x0+i) = *(x+i);

			for(i=0; i<num_threads; i++){
				*(num_rows+i) = N/num_threads;
			}
			for(i=0; i<N%num_threads; i++){
				*(num_rows + i) = *(num_rows+i) + 1;
			}

			stop=0;
			for(i=0; i<num_threads; i++){
				start = stop;
				stop = start+ *(num_rows+i);
				thread_args = (struct args *) malloc (sizeof(struct args));
				thread_args -> N = N;
				thread_args ->stopRow = stop;
				thread_args -> startRow =start;
				thread_args -> aPtr = a;
				thread_args -> bPtr = b;
				thread_args -> xPtr = x;
				thread_args -> x0ptr = x0;
				thread_args -> iter = iteration;

				pthread_create(thread_id+i, NULL, &ils_thread_worker, thread_args);
			}

			for(i=0; i<num_threads; i++){
				pthread_join(*(thread_id + i), NULL);
			}


			// the initial value array is no longer needed
			free(x0);

		}

		else {

			printf(" *** FOUND A ZERO ELEMENT ALONG MATRIX DIAGONAL ***\n");
			printf(" ***  SWITCHING TO DIRECT SOLVER FOR PIVOTING   ***\n");
			dls_( threads, len, a, b, x );

		}


	free(num_rows);
	free(thread_id);

}

void *ils_thread_worker(struct args *thread_args){
    int i, j, k, N, iteration;
    double sum1, sum2;
    double ZERO = 0.0;
    int ITERATION_MAX = 2000;
    double *x0;
	int start, stop;
	int *threads;
	double *a, *b, *x;
	int *len = &N;

	N = thread_args -> N;
	start = thread_args-> startRow;
	stop = thread_args -> stopRow;
	a = thread_args -> aPtr;
	b = thread_args -> bPtr;
	x = thread_args -> xPtr;
	x0 = thread_args -> x0ptr;	
	iteration = thread_args -> iter;

	while ( !converged(N,x,x0) && iteration < ITERATION_MAX ) {

	// copy last result to initial values

		// start the reduction process  (ref: Golub and van Loan, Chapter 10)
		for (i=start;i<stop;i++) { 
			sum1 = 0.0;
			for (j=start;j<stop-1;j++) sum1+= *(a+i*N+j)* *(x0+j); 
			sum2 = 0.0; 
			for (j=start+1;j<stop;j++) sum2+= *(a+i*N+j)* *(x0+j); 
			*(x+i) = ( *(b+i) - sum1 - sum2 ) / *(a+i*N+i);
		}

		iteration++;

	}
	
	if ( iteration == ITERATION_MAX) {
		printf(" *** ITERATIVE SOLVER FAILED TO REACH CONVERGENCE AFTER  ***\n");
		printf(" *** %d ITERATIONS, SWITCHING TO DIRECT SOLVER ***\n", iteration);
		dls_( threads, len, a, b, x );
	}


	free(thread_args);
	pthread_exit(NULL);

}


// Code to check for zeros along the diagonal
int zerosAlongDiagonal ( int N, double *a ) {

    double ZERO;
    int i;
    int foundZero;

    foundZero = 0;
    for (i=0;i<N;i++) { 
        if (!foundZero)  
            foundZero = fabs(*(a+i*N+i)) == ZERO;
    }
    return foundZero;
}

// Code to check for convergence (A. Pounds, 2018)
int converged( int N, double *a, double *b) {

    // Compute the distance between the vectors and see if the 2-Norm is
    // within tolerance

    double const TOL = 5.0e-15;
    double sum, maxb;
    int i;

    // find max in array b for tolerance scaling while computing sum

    maxb=*(b+0); 
    sum = 0.0; 
    for (i=0; i<N; i++) {
        maxb = fmax(maxb,fabs(*(b+i)));
        sum += (*(a+i)-*(b+i))*(*(a+i)-*(b+i));
    }
    sum = sqrt(sum);

    // by dividing by the largest value in the b matrix we effectively
    // scale the 2-Norm so it can achieve machine precision
    return (sum/maxb < TOL);    

}


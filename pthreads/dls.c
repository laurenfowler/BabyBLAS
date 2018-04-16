
/**********************************************************************
 *
 * DIRECT LINEAR SOLVER
 *
 * Andrew J. Pounds, Ph.D.
 * Spring 2018
 *
 * Unless otherwise noted, all code and methods belong to the author.
 * Equations for Gaussian Elimnation with Partial Pivoting and the the
 * LU Decomposition we adapted from Golub and van Loan,
 * "Matrix Computations", Johns Hopkins University press, 1996.
 *
 **********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
void dls_( int *threads, int *len,  double *a, double *b, double*x );
#ifdef __cplusplus
}
#endif

#include <math.h>  
#include <stdio.h>  
#include <stdlib.h>
#include <pthread.h>


/* Function prototype for code used in dls */
int strictlyDiagonallyDominant( int N, double *a ); 

void *dls_thread_worker();

struct args{
	int N;
	int startRow;
	int stopRow;
	int k;
	double *aPtr;
	double *bPtr;
	double *xPtr;
};

void dls_( int *threads, int *len,  double *a, double *b, double *x ){

    int i, j, k, N, u;
    int singular, iPivot, rows, rows2;
    double pivotMax, tmp, *y;
    double sum;
    double ZERO = 0.0;
    int *p;
	int num_threads = *threads;

    N = *len;

    // Check A for strict diagonal dominance to see if we can reduce the matrix without 
    // doing any row interchanges.   We could also check for positive definiteness to
    // achieve the same thing.

    if ( ! strictlyDiagonallyDominant( N, a ) ) {

        // Do Gaussian Elimination with Partial Pivoting 
        //   (modified from Golub and van Load, Chapter 3) 

        // Create an array to hold pivot swaps 

        p = malloc( (N-1) * sizeof(int) );

        for (k=0;k<N-1;k++) *(p+k)=k;

        // Search for largest value in the column and swap the 
        // entire row containing that value with the current
        // pivot row.

        for (k=0;k<N-1;k++) {
            pivotMax = *(a+k*N+k);
            iPivot = k; 
            for (u=k;u<N;u++) {
                if ( fabs(*(a+u*N+k)) > fabs(pivotMax) ) {
                    pivotMax = *(a+u*N+k);
                    iPivot = u;
                }
            }
            // If a greater pivot value was found, swap the rows.
            if ( iPivot != k ) {
                u = iPivot; 
                for (j=k;j<N;j++) {
                    tmp = *(a+k*N+j);
                    *(a+k*N+j) = *(a+u*N+j);
                    *(a+u*N+j)=tmp;
                }
            }


            // Now do block reduction
            *(p+k) = iPivot;
            if ( *(a+k*N+k) != ZERO ) {

			//start pthreads optimization
			int *num_rows;
			int startRow, stopRow;
			pthread_t *thread_id;
			struct args *thread_args;						
	
			thread_id = (pthread_t *) malloc (num_threads * sizeof(pthread_t));
			num_rows = (int *) malloc (num_threads * sizeof(int));

			for(i=0; i<num_threads; i++){
				*(num_rows+i) = (N-k)/num_threads;
			}
			for(i=0; i<(N-k)%num_threads; i++){
				*(num_rows+i) = *(num_rows+i) + 1;
			}

			stopRow=k;
			for(i=0; i<num_threads; i++){
				startRow=stopRow;
				stopRow = startRow + *(num_rows+i);
				thread_args = (struct args *) malloc(sizeof(struct args));
				thread_args -> N = N;
				thread_args ->startRow = startRow;
				thread_args -> stopRow = stopRow;
				thread_args -> aPtr = a;
				thread_args -> bPtr = b;
				thread_args -> xPtr = x;
				thread_args -> k = k;

				pthread_create(thread_id+i, NULL, &dls_thread_worker, thread_args);
			}

			for(i=0; i<num_threads; i++){
				pthread_join( *(thread_id+i), NULL);
			}

			free(num_rows);
			free(thread_id);

            }

            else {

                /* Handle the case of a zero pivot element, singular matrix */

                printf( "Element a[%d][%d} = %f\n", k, k, *(a+k*N+k)); 
                printf( " *** MATRIX A IS SINGULAR *** \n");
                printf( "    -- EXECUTION HALTED --\n");
                exit(1);
            }



        }
        // Now that we know we have reduced the matrices, start the 
        // back substitution process to solve for vector x.


        /* We now need to arrange b so that it has undergone the same 
         * operations as the matrix a.  This will then form
         * the vector y for the solution of Ux=y where U is the 
         * upper-triangular matrix formed in the elimination process
         * above. 
         */

        for (k=0; k<N-1; k++ ) {
            // Swap rows x with p(k) 
            tmp = *(b+k);
            *(b+k) = *(b+ *(p+k));
            *(b+ *(p+k)) = tmp;

            for (j=k+1;j<N;j++) 
                *(b+j)= *(b+j) - *(b+k) * *(a+N*j+k);  
        } 

        // Now do the backward substitution to get the solution
        // vector x

        *(b+N-1) = *(b+N-1) / *(a+N*(N-1)+(N-1));
        for (i=N-2;i>=0;i--){
            tmp = 0.0;
            for (j=i+1;j<N;j++) {
                tmp = tmp + *(a+i*N+j) * *(b+j);
            }
            *(b+i) = ( *(b+i) - tmp ) / *(a+i*N+i); 
        }

        for (i=0;i<N;i++){
			 *(x+i) = *(b+i);
			 //printf("%f", *(x+i) = *(b+i));
			 //printf("\n");
		}

        // At this point the solution to the system should be in vector x 


        free(p);
    }

    else {

        // Since we know the matrix is strictly diagonally dominant, verify
        // that none of the pivot elements are equal to zero

        singular = 1; 
        i=0;
        while ( i<N  && singular ) {
            singular = *(a+i*N+i) == ZERO;   
            i++;
        }

        if ( singular ) {
            printf( " *** MATRIX A IS SINGULAR *** \n");
            printf( "    -- EXECUTION HALTED -- \n");
            exit(1);
        }

        // We know at this point that we have a strictly diagonally dominant matrix that is
        // not singular -- so it sould be possible to do the LU factorization.
        //   (modified from Golub and van Loan, Chapter 3)

        for (k=0; k<N-1; k++) {
            for (rows=k+1;rows<N;rows++) {
                *(a+rows*N+k) = *(a+rows*N+k) / *(a+k*N+k);

                for (rows2=k+1;rows2<N;rows2++) { 
                    *(a+rows*N+rows2) = *(a+rows*N+rows2) - 
                        *(a+rows*N+k) * *(a+k*N+rows2) ;
                }
            }
        }

        // At this point the LU factorizaton should be done and we have to do two
        // triangular back substitutions.  The solution to Ax=b is solved by first 
        // solving Ly=b for y and then Ux=y for the solution vector x.

        // Solving lower-triangular (Ly=b) first, overwriting b with y

        for (k=0; k<N-1; k++ ) {
            for (j=k+1;j<N;j++) 
                *(b+j)= *(b+j) - *(b+k) * *(a+N*j+k);  
        } 

        // Now we can do the backward substitution to get the solution
        // vector x for the upper-triangular system (Ux=y) overwriting y (stored in b)
        // with x

        *(b+N-1) = *(b+N-1) / *(a+N*(N-1)+(N-1));
        for (i=N-2;i>=0;i--){
            tmp = 0.0;
            for (j=i+1;j<N;j++) {
                tmp = tmp + *(a+i*N+j) * *(b+j);
            }
            *(b+i) = ( *(b+i) - tmp ) / *(a+i*N+i); 
        }

        for (i=0;i<N;i++) *(x+i) = *(b+i);

        // At this point the solution to the system should be in vector x 

	
    }

}
//end of main

void *dls_thread_worker(struct args *thread_args){
	int i, j, rows, col, k;
	int start, stop, N;
	double *a, *b, *x;
	
		//unpack arguments; i passed in k as well
		N = thread_args -> N;
		start = thread_args -> startRow;
		stop = thread_args -> stopRow;
		a = thread_args -> aPtr;
		b = thread_args -> bPtr;
		x = thread_args -> xPtr;
		k = thread_args -> k; 

		//printf("k:  %d   start:  %d  stop:   %d\n",k, start+1, stop);

		for (rows=start+1;rows<stop;rows++) { 
			*(a+rows*N+k) = *(a+rows*N+k) / *(a+k*N+k);

			for (col=k+1;col<N;col++) { 
				*(a+rows*N+col) = *(a+rows*N+col) - 
					*(a+rows*N+k) * *(a+k*N+col) ;
			}
		}

		free(thread_args);
		pthread_exit(NULL);
}

int strictlyDiagonallyDominant( int N, double *a ) {

    double sum;
    int i, testPassed, row;

    testPassed = 1;
    row = 0;
    sum = 0.0;
    for (row=0;row<N;row++) { 
        if (testPassed) {
            sum = 0.0;
            for (i=0;i<N;i++) sum+=*(a+row*N+i);
            sum-=fabs(*(a+row*N+row)); 
            testPassed = fabs(*(a+row*N+row)) > sum;
        }
    }

    return testPassed;
}


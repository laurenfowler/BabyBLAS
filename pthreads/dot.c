#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// Computes the dot product of two vectors
// pthreads version

#ifdef __cplusplus
extern "C" {
#endif

double dot_(int *num_threads, int *N, double *vec1, double *vec2);

#ifdef __cplusplus
}
#endif

void *dot_thread_worker();

struct args{
	int N;
	int startRow;
	int stopRow;
	double final_sum;
	double *ansvec;
	double *v1ptr;
	double *v2ptr;
};

double dot_(int *num_threads, int *N, double *vec1, double *vec2){

	int i;

	//pthread variables
	pthread_t *thread_id;
	struct args *thread_args;
	int len = *N;
	int *num_rows;
	int numThreads = *num_threads;
	int startRow, stopRow;
	double *ansvec;
	double sum=0;

	// Malloc an array for thread id's
	thread_id = (pthread_t *) malloc (numThreads * sizeof(pthread_t));

	// Malloc array to keep up with num of rows for each thread
	num_rows = (int *) malloc(numThreads * sizeof(int));

	// Malloc solution vector for later use
	ansvec = (double *) malloc(len * sizeof(double));

	//determine number rows
	for (int i=0; i<numThreads; i++){
		*(num_rows+i) = len/numThreads;
	}
	for (int i=0; i< len%numThreads; i++){
		*(num_rows+i) = *(num_rows+i)+1;
	}

	// malloc memery for struct
	stopRow=0;
	for(int i=0; i < numThreads; i++){
		startRow=stopRow;
		stopRow= startRow + *(num_rows+i);
		thread_args->N = len;
		thread_args->startRow = startRow;
		thread_args->stopRow = stopRow;
		thread_args->v1ptr = vec1;
		thread_args->v2ptr = vec2;
		thread_args->ansvec = ansvec;

		pthread_create( thread_id+i, NULL, &dot_thread_worker, thread_args );
	}


	for(int i=0; i < numThreads; i++){
		pthread_join((*thread_id+i), NULL);
	}

	ansvec = thread_args ->ansvec;
	

	for(int i=0; i<len; i++){
		sum += *(ansvec+i);
	}

	return sum;
	
	free (ansvec);
	free(num_rows);
	free(thread_id);
}


void *dot_thread_worker( struct args *thread_args){


	int i, startRow, stopRow, len;
	double *vec1, *vec2, *ans;
	double sum;
	
	//unpack thread_args struct into normal variables
	len = thread_args->N;
	startRow = thread_args->startRow;
	stopRow = thread_args->stopRow;
	vec1 = thread_args->v1ptr;
	vec2 = thread_args->v2ptr;
	ans = thread_args-> ansvec;
	
	sum = 0;	

	for (i=startRow; i<stopRow; i++){
		*(ans+i) = *(vec1 + i) * *(vec2+i);
	}
	
	free(thread_args);
	pthread_exit(NULL);
}		


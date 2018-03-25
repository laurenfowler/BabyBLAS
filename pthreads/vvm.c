#ifdef __cplusplus
extern "C" {
#endif
    void vvm_(  int *len, double *va, double *vb, double *ma);
#ifdef __cplusplus
    }
#endif

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//function prototype
void *vvm_thread_worker();

struct args{
	int N;
	int startRow;
	int stopRow;
	double *vec1;
	double *vec2;
	double *matrix;
};

// Computes the tensor product of two vectors 

void  vvm_(int *num_threads, int *len, double *va, double *vb, double *ma){

	int threads = *num_threads;
	int size = *len;
	int *num_rows;
	int start, stop;
	pthread_t *thread_id;
	struct args *thread_args;
	double matrix = *ma;

	if(size < numThreads){

		for (i=0; i<alength; i++) {
			for (j=0; j<alength; j++) {
				*(ma+(alength*i)+j) = *(va+i) * *(vb+j);
			}
		}

	}
	else{

		//start of parallel
	
		//malloc array for thread id's
		thread_id = (pthread_t *) malloc (threads * sizeof(pthread_t));

		//malloc array for how many rows to work on in each thread
		num_rows = (int *) malloc( num_threads * sizeof(int));

		//determine number of rows over which each thread will works
		

	}
}

void *vvm_thread_worker(struct args *thread_args){

	int i, j;
	int alength = *len;

	for (i=0; i<alength; i++) {
		for (j=0; j<alength; j++) {
			*(ma+(alength*i)+j) = *(va+i) * *(vb+j);
		}
	}

}

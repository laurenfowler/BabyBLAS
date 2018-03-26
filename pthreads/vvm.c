#ifdef __cplusplus
extern "C" {
#endif
    void vvm_(int *num_threads, int *len, double *va, double *vb, double *ma);
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

void vvm_(int *num_threads, int *len, double *va, double *vb, double *ma){

	int i, j;
	int threads = *num_threads;
	int size = *len;
	int *num_rows;
	int start, stop;
	pthread_t *thread_id;
	struct args *thread_args;
	double matrix;

	if(size < threads){

		for (i=0; i<size; i++) {
			for (j=0; j<size; j++) {
				*(ma+(size*i)+j) = *(va+i) * *(vb+j);
			}		
		}

	}
	else{

		//start of parallel
	
		//malloc array for thread id's
		thread_id = (pthread_t *) malloc (threads * sizeof(pthread_t));

		//malloc array for how many rows to work on in each thread
		num_rows = (int *) malloc( threads * sizeof(int));

		//determine number of rows over which each thread will works
		for(int i=0; i<threads; i++){
			*(num_rows+i) = size/threads;
		}	
		for(int i=0; i<size%threads; i++){
			*(num_rows+i) = *(num_rows+i) + 1;
		}

		stop=0;
		for(int i=0; i<threads; i++){
			start = stop;
			stop = start + *(num_rows+i);
			thread_args = (struct args *) malloc(sizeof(struct args));
			thread_args -> N = size;
			thread_args -> startRow = start;
			thread_args -> stopRow = stop;
			thread_args -> vec1 = va;
			thread_args -> vec2 = vb;
			thread_args -> matrix = ma;

			pthread_create(thread_id+i, NULL, &vvm_thread_worker, thread_args);
		}
	
		for(int i=0; i<threads; i++){
			pthread_join((*thread_id+i), NULL);
		}

//		ma = thread_args -> matrix;

		free(num_rows);
		free(thread_id);

	}
}

void *vvm_thread_worker(struct args *thread_args){

	printf("in worker function\n");
	int i, j, start, stop, N;
	double *vec1, *vec2, *matrix;
	//unpack thread_args
	N = thread_args -> N;
	start = thread_args -> startRow;
	stop = thread_args -> stopRow;
	vec1 = thread_args -> vec1;
	vec2 = thread_args -> vec2;
	matrix = thread_args -> matrix;
		
	//perform calculations
	for (i=start; i<stop; i++) {
		for (j=0; j<N; j++) {
			*(matrix+(N*i)+j) = *(vec1+i) * *(vec2+j);
		}
	}
	
	free(thread_args);
	pthread_exit(NULL);	

}




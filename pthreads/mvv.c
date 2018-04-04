#ifdef __cplusplus
extern "C"{
#endif
	void mvv_(int *num_threads, int *N, double *mat, double *vec, double *vresults);
#ifdef __cplusplus
	}
#endif

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//prototype
void *mvv_thread_worker();

//struct
struct args{
	int N;
	int startRow;
	int stopRow;
	double *mat_ptr;
	double *vec_ptr;
	double *ansvec_ptr;
};

//computes matrix vector multiplication which results in a vector

void mvv_(int *num_threads, int *N, double *mat, double *vec, double *vresults){

    //Lets put an end of line marker here to make sure it gets printed
	printf("in mvv function\n");
	int threads = *num_threads;
	int size = *N;
	int *num_rows;
	int start, stop;
	pthread_t *thread_id;
	struct args *thread_args;

    printf("completed initializations, threads = %d, size = %d\n", threads, size);
	if(size<threads){
		int i, j;

		for(i=0; i<size; i++){
			for(j=0; j<size; j++){
				*(vresults+i) += *(mat+(size*i)+j) * *(vec+j); 
			}
		}
	}
	else{

	printf("about to malloc thread_id array\n");
	//Malloc thread_id array 
	thread_id = (pthread_t *) malloc (threads * sizeof(pthread_t));

	//malloc num_rows array
	num_rows = (int *) malloc(threads* sizeof(int));

	printf("about to allocate rows per thread\n");
	//determine num rows each thread gets
	for (int i=0; i<threads; i++){
		*(num_rows+i) = size/threads;
	}
	for (int i=0; i<size%threads; i++){
		*(num_rows+i) = *(num_rows + i) + 1;
	}

	printf("about to malloc struct data\n");
	//malloc struct data
	stop =0;
	for(int i=0; i<threads; i++){
		start = stop;
		stop = start+*(num_rows+i);
		thread_args = (struct args *) malloc(sizeof(struct args));
		thread_args-> N = size;
		thread_args-> startRow = start;
		thread_args-> stopRow = stop;
		thread_args-> mat_ptr = mat;
		thread_args-> vec_ptr = vec;
		thread_args-> ansvec_ptr = vresults;
		
		pthread_create(thread_id+i, NULL, &mvv_thread_worker, thread_args);
		}

    printf("all threads created\n");

	for(int i=0; i< threads; i++){
		pthread_join( *(thread_id+i), NULL);
	}
	
    printf("all threads joined\n");

	free(num_rows);
	free(thread_id);

	}
}

void *mvv_thread_worker(struct args *thread_args){	

	printf("in worker funciton\n");
	int i, j, start, stop, N;
	double *ansvec, *vec, *mat;

	//unpack thread_args
	N = thread_args -> N;
	start = thread_args -> startRow;
	stop = thread_args -> stopRow;
	ansvec = thread_args -> ansvec_ptr;
	vec = thread_args -> vec_ptr;
	mat = thread_args -> mat_ptr;

    printf(" start = %d, stop = %d\n", start, stop);    

	for(i=start; i<stop; i++){
		for(j=0; j<N; j++){
			*(ansvec+i) += *(mat+(N*i)+j) * *(vec+j); 
		}
	}

	free(thread_args);
	pthread_exit(NULL);
}




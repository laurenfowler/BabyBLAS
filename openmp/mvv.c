#ifdef __cplusplus
extern "C"{
#endif
	void mvv_(int *num_threads, int *N, double *mat, double *vec, double *vresults);
#ifdef __cplusplus
	}
#endif

#include <omp.h>
#include <stdio.h>

//computes matrix vector multiplication which results in a vector

void mvv_(int *num_threads, int *N, double *mat, double *vec, double *vresults){

	int i, j;
	int iter = *N;

	omp_set_num_threads(*num_threads);

	#pragma omp parallel shared(iter) private (i)
	{
		#pragma omp for
		for(i=0; i<iter; i++){
			int n= omp_get_thread_num();
			//printf("thread num %d\n", n);
			for(j=0; j<iter; j++){
				*(vresults+i) += *(mat+(iter*i)+j) * *(vec+j); 
			}
		}

	}
}


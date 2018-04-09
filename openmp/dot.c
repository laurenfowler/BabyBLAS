#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
// Computes the dot product of two vectors

#ifdef __cplusplus
extern "C" {
#endif

double dot_(int *num_threads, int *N, double *vec1, double *vec2);

#ifdef __cplusplus
}
#endif

double dot_(int *num_threads, int *N, double *vec1, double *vec2){

	int i, len;
	double sum;
	double *ansvec;

	len = *N;
	sum = 0;	
	ansvec = (double *) malloc (len * sizeof(double));

	printf("in openmp dot");
	omp_set_num_threads(*num_threads);

	#pragma omp for 
	for (i=0; i<len; i++){
		sum = sum + (*(vec1 + i) * *(vec2 + i));
	}

	return sum;

}		


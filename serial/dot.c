#include <stdio.h>
//#include "dot.h"
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

	len = *N;
	sum = 0;	

	for (i=0; i<len; i++){
		sum = sum + (*(vec1 + i) * *(vec2 + i));
	}

	return sum;

}		


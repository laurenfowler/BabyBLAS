#include <stdio.h>
//#include "dot.h"
// Computes the dot product of two vectors

#ifdef __cplusplus
extern "C" {
#endif

double dot_(int num_threads, int N, double *vec1, double *vec2);

#ifdef __cplusplus
}
#endif

double dot_(int num_threads, int N, double *vec1, double *vec2){

	int i, num_col;
	double sum;

	//check for proper row/columns
	num_col = (*vec1+1) - *vec1;
	printf("%d", num_col);

}		


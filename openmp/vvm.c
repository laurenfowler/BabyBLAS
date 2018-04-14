#ifdef __cplusplus
extern "C" {
#endif
    void vvm_(int *num_threads, int *len, double *va, double *vb, double *ma);
#ifdef __cplusplus
    }
#endif

#include <stdio.h>
#include <omp.h>

// Computes the tensor product of two vectors 

void vvm_(int *num_threads, int *len, double *va, double *vb, double *ma){

	int i, j;

	int alength = *len;

	omp_set_num_threads(*num_threads);

	#pragma omp parallel shared(alength) private(i)
	{
		#pragma omp for
		for (i=0; i<alength; i++) {
			//int n = omp_get_thread_num();
			//printf("thread num %d\n", n);
			//printf("index: %d\n", i);
			for (j=0; j<alength; j++) {
				*(ma+(alength*i)+j) = *(va+i) * *(vb+j);
			}
		}
	}
}

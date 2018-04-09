#ifdef __cplusplus
extern "C"{
#endif
	void mvv_(int *num_threads, int *N, double *mat, double *vec, double *vresults);
#ifdef __cplusplus
	}
#endif

//computes matrix vector multiplication which results in a vector

void mvv_(int *num_threads, int *N, double *mat, double *vec, double *vresults){

	int i, j;
	int iter = *N;

	for(i=0; i<iter; i++){
		sum = 0;
		for(j=0; j<iter; j++){
			*(vresults+i) += *(mat+(N*i)+j) * *(vec+j); 
		}
	}
}


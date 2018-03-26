#include <stdio.h>
#include <stdlib.h>
//test program

double dot_(int *num_threads, int *N, double *vec1, double *vec2);
void vvm_(int *num_threads, int *N, double *vec1, double *vec2, double *ma);

int main(){

	double arr1[] = {1, 2, 3};
	double arr2[] =  {1, 2, 3};
	int size = 3;
	int threads = 2;
	
	double *vec1 = arr1;
	double *vec2 = arr2;
	double *ma; 
	int *N = &size;
	int *num_threads = &threads;

	//malloc answer array
	ma = (double *) malloc (size * size * sizeof(double));

	vvm_(num_threads, N, vec1, vec2, ma);

	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			printf("%f", *(ma+i+j));
			printf(" ");
		}
		printf("\n");	
	}
	

return 0;
}


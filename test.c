#include <stdio.h>
#include <stdlib.h>
//test program

double dot_(int *num_threads, int *N, double *vec1, double *vec2);
void vvm_(int *num_threads, int *N, double *vec1, double *vec2, double *ma);
void mvv_(int *num_threads, int *N, double *mat, double *vec, double *vresults);

int main(){

	
	//double arr1[2][2] = { {1, 2}, {3, 4}};
	double arr2[] =  {1, 1};
	int size = 2;
	int threads = 2;
	
	double *mat = (double *) malloc(size * size * sizeof(double));
	double *vec2 = arr2;
	double *vresults; 
	int *N = &size;
	int *num_threads = &threads;


	//testing mvv
	
	//create array
	*(mat+(size*0) + 0) = 1.00;
	*(mat+(size*0) + 1) = 2.00;
	*(mat+(size*1) + 0) = 3.00;
//	printf("test");
	*(mat+(size*1) + 1) = 4.00;


	 for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			printf("%f", *(mat+(i*size) + j));
			printf(" ");
		}
		printf("\n");	
	} 

	printf("going to function");
	mvv_(num_threads, N, mat, vec2, vresults);
	printf("back from function");

	for(int i=0; i<size; i++){
		printf("%f", *(vresults + i));
	}

	free(mat);


/*
 *  Testing vvm 
	//malloc answer array
	ma = (double *) malloc (size * size * sizeof(double));

	vvm_(num_threads, N, vec1, vec2, ma);

	 for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			printf("%f", *(ma+(i*size) + j));
			printf(" ");
		}
		printf("\n");	
	} */
	

return 0;
}


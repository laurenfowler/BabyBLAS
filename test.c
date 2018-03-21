#include <stdio.h>
//test program

double dot_(int *num_threads, int *N, double *vec1, double *vec2);

int main(){

	double ans = 0;
	double arr1[] =  {1, 1, 1};
	double arr2[] =  {1, 1, 1};
	int size = 3;
	int threads = 2;
	
	double *vec1 = arr1;
	double *vec2 = arr2;
	int *N = &size;
	int *num_threads = &threads;
	double *sum = &ans;

	ans = dot_(num_threads, N, vec1, vec2);
	
	printf("%f\n", sum);

return 0;
}


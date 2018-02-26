//test progra

double dot(int num_threads, int N, double *vec1, double *vec2);

int main(){

	double ans;
	double arr1[] =  {1, 1, 1};
	double arr2[] =  {1, 1, 1};

	double *vec1 = &arr1[0];
	double *vec2 = &arr2[0];

	ans = dot(1, 1, vec1, vec2);
	


return 0;
}


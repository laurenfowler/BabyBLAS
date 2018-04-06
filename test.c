#include <stdio.h>
#include <stdlib.h>
#include <papi.h>
//test program

double dot_(int *num_threads, int *N, double *vec1, double *vec2);
void vvm_(int *num_threads, int *N, double *vec1, double *vec2, double *ma);
void mvv_(int *num_threads, int *N, double *mat, double *vec, double *vresults);
void mmm_(int *num_threads, int*N, double *a, double *b, double *c);
double walltime_();
double cputime_();
int atoi(const char *str);


		int main(int argc, char *argv[]){

			int i, j, size;
			int *N;
			double ans, x;
			double *vec1, *vec2;
			double wall, cpu;

			// PAPI Variables
			float rtime, ptime, mflops;
			long long flpops;
			long long dp_ops[2];
			int check;			

			//thread
			j=1;
			int *thread = &j;

			//assign size arguments
			size = atoi(argv[1]);
			N = &size;

		#ifdef DOT
			//malloc vectors
			vec1 = (double *) malloc (size * sizeof(double));
			vec2 = (double *) malloc (size * sizeof(double));
	
			x=1.0;
			//generate vectors
			for(i=0; i<size; i++){
				*(vec1 + i) = x;
				*(vec2 + i) = 1.0/x;
				x = x + 1.0;
			}
			//end if dot
		#endif

		#ifdef VVM
			double *ma;

			//malloc vectors
			vec1 = (double *) malloc (size * sizeof(double));
			vec2 = (double *) malloc (size * sizeof(double));
			ma = (double *) malloc (size * size * sizeof(double));

			//create vectors
			x = 2.0;
			for(i=0; i<size; i++){
				*(vec1+i) = x;
				*(vec2+i) = 1.0/x;
			}
			//end if VVM
		#endif

		#ifdef PAPI

			//initialize low level papi
			int retval;

			 retval = PAPI_library_init(PAPI_VER_CURRENT);
		     if (retval != PAPI_VER_CURRENT && retval > 0) {
        		 fprintf(stderr,"PAPI library version mismatch!\n");
        		 exit(1);
         	 } 

			 if (retval < 0) {
         		 printf("PAPI initialization error.\n");
         		 exit(retval);
         	 }
     		
			 retval = PAPI_is_initialized();
     		 if (retval != PAPI_LOW_LEVEL_INITED) {
         	 	printf("PAPI low level initialization failed.\n");
          		exit(retval);
     		 }
			
			//Create PAPI event set
			int eventSet = PAPI_NULL;

     		if (PAPI_create_eventset(&eventSet) != PAPI_OK ) {
         		printf("Could not create PAPI event set.\n");
         		exit(1);
         	}

			//Add the particular events to be counted to each event set
			if (PAPI_add_event(eventSet, PAPI_DP_OPS) != PAPI_OK ) {
		        printf("Could not create PAPI_DP_OPS event.\n");
       			exit(1);
        	 }
			
		 	if (PAPI_start(eventSet) != PAPI_OK ) {
         		printf("Could not start PAPI_DP_OPS counter.\n");
         		exit(1);
         	}

			//timings
			wall = walltime_();
			cpu = cputime_();
			
			//printf("%f\n", cpu);

			//Read set and set array back to zero
			if (PAPI_accum(eventSet, dp_ops ) != PAPI_OK ) {
	        	printf("Could not read first event set.\n");
         		exit(1);
        	 }
		//end of papi
		#endif

		#ifdef DOT
			//call dot function
			ans = dot_(thread, N, vec1, vec2);
			printf("%f\n", ans);
		#endif

		#ifdef VVM
			vvm_(thread, N, vec1, vec2, ma);
		#endif

		#ifdef MVV
			double *ma = (double *) malloc (size * size * sizeof(double));
			mvv_(thread, N, ma, vec1, vec2); 
		#endif
		
		#ifdef PAPI	
			if (PAPI_read(eventSet, dp_ops) != PAPI_OK ) {
		    	printf("Could not read first event set.\n");
         		exit(1);
        	 }

			//second call to PAPI_flops to recover value since last call
			check = PAPI_flips(&rtime, &ptime, &flpops, &mflops);

			//caculate wall and cpu time
			wall = walltime_() - wall;
			cpu = cputime_() - cpu;

			printf("%f\n", cpu);

			//estimated megaflops
			printf("Estimated megaflops = %f\n", (2*(double)(size*size*size)/cpu)/1000000.0);

			//Papi flops
			printf(" DP_OPS Count from PAPI= %15lld\n", dp_ops[0]);

			//compute and print megaflops based on PAPI counters
			mflops = (dp_ops[0]/cpu)/1000000.0;
			printf(" PAPI megaflops = %15lld\n", mflops);

		#endif
	
		#ifdef DOT
			free(vec1);
			free(vec2);	
		#endif

		#ifdef VVM
			free(vec1);
			free(vec2);
			free(ma);		
		#endif
	

			return 0;
		}
		

	
	//double arr1[2][2] = { {1, 2}, {3, 4}};
/*	double arr2[] =  {1, 1};
	int size = 2;
	int threads = 2;
	
	double *mat = (double *) malloc(size * size * sizeof(double));
	double *vec2 = arr2;

    //need to malloc space for results too
	double *vresults = (double *) malloc( size * sizeof(double)); 
	
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

    // Let's add an end of line character to see if it puses the errors
    // to the screen.
	printf("going to function\n");
	mvv_(num_threads, N, mat, vec2, vresults);
	printf("back from function\n");

	for(int i=0; i<size; i++){
		printf("%f", *(vresults + i));
	}

	free(mat);

*/

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
	



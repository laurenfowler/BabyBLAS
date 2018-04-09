#include <stdio.h>
#include <stdlib.h>
#include <papi.h>
#include <time.h>
//test program

double dot_(int *num_threads, int *N, double *vec1, double *vec2);
void vvm_(int *num_threads, int *N, double *vec1, double *vec2, double *ma);
void mvv_(int *num_threads, int *N, double *mat, double *vec, double *vresults);
void mmm_(int *num_threads, int*N, double *a, double *b, double *c);
void dls_(int *num_threads, int *N, double *a, double *b, double *c);
void ils_(int *num_threads, int *N, double *a, double *b, double *c);
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

		#ifdef MVV
			vec1 = (double *) malloc (size * sizeof(double));
			//answer vec
			vec2 = (double *) malloc (size * sizeof(double));
			//initilze to 0
			double *im = (double *) calloc (size * size, sizeof(double));
			
			x = 1.0;
			//create identity matrix
			for(i=0; i<size; i++){
				*(im + (size * i) + i) = 1.0;
			}

			//create vector
			for(i=0; i<size; i++){
				*(vec1 + i) = x;
				x = x + 1.0;
			}

		//endif MVV
		#endif

		#ifdef MMM
			double *im = (double *) calloc (size * size, sizeof(double));
			double *ma = (double *) malloc (size * size * sizeof(double));
			double *ansma = (double *) malloc (size * size * sizeof(double));
			
			x = 1.0;
			//create identity matrix and other matrix
			for(i=0; i<size; i++){
				*(im + (size * i) + i) = 1.0;
				for(j=0; j<size; j++){
					*(ma + (size * i) + j) = x;	
					x = x + 1.0;
				}
			}

		//endif MMM
		#endif

		#ifdef DLS
			double *im = (double *) malloc(size * size * sizeof(double));
			vec1 = (double *) malloc(size * sizeof(double));
			vec2 = (double *) malloc(size * sizeof(double));
	
			//create random matrix and vector each time 
			srand(time(NULL));
			for(i=0; i<size; i++){
				*(vec1 + i) = (double) rand();
				for(j=0; j<size; j++){
					*(im + (size*i) + j) =(double) rand();
				}
			}

		#endif

		#ifdef ILS
			double *im = (double *) malloc(size * size * sizeof(double));
			vec1 = (double *) malloc(size * sizeof(double));
			vec2 = (double *) malloc(size * sizeof(double));

			
			//create diagonally dominant matrix and random vector
			srand(time(NULL));
			for(i=0; i<size; i++){
				*(vec1 + i) = (double) rand();
				for(j=0; j<size; j++){
					if( i == j){
						*(im + (size *i) + j) = (double) size;
					}
					else {
						*(im + (size * i) + j) = 1.0;
					}
				}
			}
			//end dls
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
			//printf("%f", ans);
		#endif

		#ifdef VVM
			vvm_(thread, N, vec1, vec2, ma);
			//for(i=0; i<size; i++){
			//	for(j=0; j<size; j++){
			//		printf("%f", *(ma +(size*i) +j));
			//		printf(" ");
			//	}
			//	printf("\n");
			//}
		#endif

		#ifdef MVV
			mvv_(thread, N, im, vec1, vec2);
		#endif

		#ifdef MMM
			mmm_(thread, N, im, ma, ansma);
			
			/*for(i=0; i<size; i++){
				for(j=0; j<size; j++){
					printf("%f", *(ansma + (size * i) + j));
					printf(" ");
				}
				printf("\n");
			} */

		#endif

		#ifdef DLS
			dls_(thread, N, im, vec1, vec2);
			/*for(i=0; i<size; i++){
				printf("%f\n", *(vec2 + i));
			}*/
		#endif

		#ifdef ILS
			ils_(thread, N, im, vec1, vec2);
			/*for(i=0; i<size; i++){
				printf("%f\n", *(vec1 +i),  *(vec2 + i));
			}*/
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

			//estimated megaflops
			//printf("Estimated megaflops = %f\n", (2*(double)(size*size*size)/cpu)/1000000.0);

			//Papi flops
			//printf(" DP_OPS Count from PAPI= %15lld\n", dp_ops[0]);

			//compute and print megaflops based on PAPI counters
			mflops = (double) (dp_ops[0]/cpu)/1000000.0;
			printf("%d %f %f %f\n", size, mflops, cpu, wall);

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

		#ifdef MVV
			free(vec1);
			free(vec2);
			free(im);
		#endif

		#ifdef MMM
			free(ma);
			free(im);
			free(ansma);
		#endif

		#if defined(DLS) || defined(ILS)
			free(im);
			free(vec1);
			free(vec2);
		#endif 
	

			return 0;
		}
		


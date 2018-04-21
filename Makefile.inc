# Define the makefile variables that will be used in building system

F90 = gfortran 
CC  = gcc
FFLAGS = -cpp -g -DLS_TEST 
CFLAGS = -g -fopenmp -O2

#MYLIBS =  -L./openmp -lopenmp_bblas -L./serial -lserial_bblas -L./pthreads -lpthreads_bblas -L./lbstime -llbstime 
#MYLIBS = -L./serial -lserial_bblas -L./openmp -lopenmp_bblas -L./pthreads -lpthreads_bblas -L./lbstime -llbstime 
MYLIBS =  -L./pthreads -lpthreads_bblas -L./serial -lserial_bblas -L./openmp -lopenmp_bblas -L./lbstime -llbstime 

SYSLIBS = -lgomp -lpthread

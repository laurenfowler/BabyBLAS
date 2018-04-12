# Define the makefile variables that will be used in building system

F90 = gfortran 
CC  = gcc
GCOV = -fprofile-arcs -ftest-coverage
FFLAGS = -cpp -O2 -DACCURACY_TEST -DLS_TEST 
CFLAGS = -g -fopenmp
MYLIBS = -L./pthreads -lpthreads_bblas -L./openmp -lopenmp_bblas -L./serial -lserial_bblas -L./lbstime -llbstime 
SYSLIBS = -lgomp -lpthread

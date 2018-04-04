# Define the makefile variables that will be used in building system

F90 = gfortran 
CC  = gcc
FFLAGS = -cpp -O2 -DACCURACY_TEST -DLS_TEST 
CFLAGS = -g -fopenmp
MYLIBS = -L./pthreads -lpthreads_bblas\
         -L./serial -lsingle_bblas -L./lbstime -llbstime 
SYSLIBS = -lpthread

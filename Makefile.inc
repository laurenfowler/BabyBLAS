# Define the makefile variables that will be used in building system

F90 = gfortran 
CC  = gcc
FFLAGS = -cpp -g -DACCURACY_TEST -DLS_TEST -DITERATIVE -DDIAGDOM
CFLAGS = -g -fopenmp 
MYLIBS = -L./pthreads -lpthreads_bblas -L./serial -lserial_bblas -L./lbstime -llbstime 

#-L./openmp -lopenmp_bblas 
SYSLIBS = -lgomp -lpthread

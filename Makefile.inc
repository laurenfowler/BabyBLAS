# Define the makefile variables that will be used in building system

F90 = gfortran
CC  = gcc
FFLAGS = -O3
CFLAGS = -O3
DOPT = -DVVM 
LDLIBS = -L./serial -lsingle_bblas -L./pthreads -lpthreads_bblas -L./lbstime -llbstime  
SYSLIBS = -lpthread -lpapi 


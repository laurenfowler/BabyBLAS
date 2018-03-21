# Define the makefile variables that will be used in building system

F90 = gfortran
CC  = gcc
FFLAGS = -O3
CFLAGS = -O3
LDLIBS = -L./single_thread -L./pthreads -lpthreads_bblas -lsingle_bblas -L./lbstime -llbstime
SYSLIBS = -lpthread


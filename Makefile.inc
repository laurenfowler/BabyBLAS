# Define the makefile variables that will be used in building system

F90 = gfortran
CC  = gcc
FFLAGS = -O3
CFLAGS = -O2
DOPT = -DDLS
LDLIBS = -L./serial -lsingle_bblas -L./lbstime -llbstime  
SYSLIBS = -lpthread -lpapi 


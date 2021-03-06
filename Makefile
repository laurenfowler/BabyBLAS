# Top level makefile
#
include Makefile.inc

#all : driver pthreads serial openmp lbstime 
all : driver serial pthreads  openmp lbstime 
#all : driver openmp pthreads serial lbstime 

#driver: driver.o serial openmp pthreads lbstime  
driver: driver.o pthreads serial openmp lbstime  
#driver: driver.o openmp pthreads serial lbstime  
	$(F90) driver.o -o driver $(MYLIBS) $(SYSLIBS)  

driver.o: driver.f90
	$(F90) $(FFLAGS) driver.f90 -c  

serial: 
	cd serial && $(MAKE)

pthreads: 
	cd pthreads && $(MAKE)

openmp: 
	cd openmp && $(MAKE)

lbstime: 
	cd lbstime && $(MAKE)

clean:
	cd serial && $(MAKE) clean
	cd pthreads && $(MAKE) clean
	cd openmp && $(MAKE) clean
	cd lbstime && $(MAKE) clean
	rm *.o
	touch *.f90

pristine:
	cd serial && $(MAKE) pristine 
	cd pthreads && $(MAKE) pristine 
	cd openmp && $(MAKE) pristine 
	cd lbstime && $(MAKE) pristine
	rm *.o	
	rm driver
	touch *.f90

#This next target get "made" every time
.PHONY: serial openmp pthreads lbstime

#Top Level Makefile

#Lauren Fowler
#Baby BLAS project
#2/25/2018

include Makefile.inc

all : testprog serial pthreads lbstime

#Tried liking library to test.o file???
testprog: test.o serial pthreads lbstime
	$(CC) test.o -o testprog $(LDLIBS) $(SYSLIBS)

test.o: test.c
	$(CC) $(DOPT) test.c -c

serial: 
	cd serial && $(MAKE)

pthreads: 
	cd pthreads && $(MAKE)

lbstime: 
	cd lbstime && $(MAKE)

clean:
	cd pthreads && $(MAKE) clean 	
	cd serial && $(MAKE) clean
	cd lbstime && $(MAKE) clean
	rm *.o
	rm testprog

pristine:
	cd pthreads && $(MAKE) pristine
	cd serial && $(MAKE) pristine
	cd lbstime && $(MAKE) pristine
	rm *.o

#This next target gets "made" every time
.PHONY: serial pthreads lbstime	

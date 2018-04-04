#Top Level Makefile

#Lauren Fowler
#Baby BLAS project
#2/25/2018

include Makefile.inc

all : testprog pthreads single_thread lbstime

#Tried liking library to test.o file???
testprog: test.o pthreads single_thread lbstime
	$(CC) test.o -o testprog $(LDLIBS) $(SYSLIBS)

test.o: test.c
	$(CC) test.c -c

single_thread: 
	cd single_thread && $(MAKE)

pthreads: 
	cd pthreads && $(MAKE)

lbstime: 
	cd lbstime && $(MAKE)

clean:
	cd pthreads && $(MAKE) clean 	
	cd single_thread && $(MAKE) clean
	cd lbstime && $(MAKE) clean
	rm *.o
	rm testprog

pristine:
	cd pthreads && $(MAKE) pristine
	cd single_thread && $(MAKE) pristine
	cd lbstime && $(MAKE) pristine
	rm *.o

#This next target gets "made" every time
.PHONY: pthreads single_thread lbstime	

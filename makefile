#Top Level Makefile

#Lauren Fowler
#Baby BLAS project
#2/25/2018

include Makefile.inc

all : testprog single_thread lbstime

#Tried liking library to test.o file???
testprog: test.o single_thread lbstime
	$(CC) test.o -o testprog $(LDLIBS)

test.o: test.c
	$(CC) test.c -c

single_thread: 
	cd single_thread && $(MAKE)

lbstime: 
	cd lbstime && $(MAKE)

clean: 	
	cd single_thread && $(MAKE) clean
	cd lbstime && $(MAKE) clean
	rm *.o
	rm testprog

pristine:
	cd single_thread && $(MAKE) pristine
	cd lbstime && $(MAKE) pristine
	rm *.o

#This next target gets "made" every time
.PHONY: single_thread lbstime	

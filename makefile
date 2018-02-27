#Top Level Makefile

#Lauren Fowler
#Baby BLAS project
#2/25/2018

include Makefile.inc

all : test single_thread lbstime

#Tried liking library to test.o file???
test: test.o single_thread lbstime
	$(CC) test.o -L. -lsingle_bblas -o test $(LDLIBS) 

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
	rm test

pristine:
	cd single_thread && $(MAKE) pristine
	cd lbstime && $(MAKE) pristine
	rm *.o

#This next target gets "made" every time
.PHONY: single_thread lbstime	

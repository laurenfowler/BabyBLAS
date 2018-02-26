#Top Level Makefile

#Lauren Fowler
#Baby BLAS project
#2/25/2018

include Makefile.inc

all : test dot_prod lbstime

test: test.o dot_prod lbstime
	$(CC) test.o -o test $(LDLIBS) 

test.o: test.c
	$(CC) test.c -c

dot_prod: 
	cd dot_prod && $(MAKE)

lbstime: 
	cd lbstime && $(MAKE)

clean: 	
	cd dot_prod && $(MAKE) clean
	cd lbstime && $(MAKE) clean
	rm *.o
	rm test
	

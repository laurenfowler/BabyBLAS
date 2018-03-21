include ../Makefile.inc

all: lib

dot.o: dot.c
	$(CC) -c dot.c

lib: dot.o
	ar -rc libpthreads_bblas.a dot.o
	ranlib libpthreads_bblas.a

#Default targets for cleaning up enviroment

clean:
	rm *.o
	touch *.c

pristine: 
	make clean
	rm *.a

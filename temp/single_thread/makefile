include ../Makefile.inc

all: lib

dot.o: dot.c
	$(CC) -c dot.c

lib: dot.o
	ar -rc libsingle_bblas.a dot.o
	ranlib libsingle_bblas.a

#Default targets for cleaning up enviroment

clean:
	rm *.o
	touch *.c

pristine: 
	make clean
	rm *.a

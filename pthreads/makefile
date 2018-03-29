include ../Makefile.inc

all: lib

dot.o: dot.c
	$(CC) -c dot.c

vvm.o: vvm.c
	$(CC) -c vvm.c

mvv.o: mvv.c
	$(CC) -c mvv.c

lib: dot.o vvm.o mvv.o
	ar -rc libpthreads_bblas.a dot.o vvm.o mvv.o
	ranlib libpthreads_bblas.a

#Default targets for cleaning up enviroment

clean:
	rm *.o
	touch *.c

pristine: 
	make clean
	rm *.a

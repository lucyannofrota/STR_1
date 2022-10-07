# test: test
all: aux_libs problems clean

LIBS = -lpthread -lrt

aux_libs: pthread_aux.o

problems: problema_1

# aux_libs

pthread_aux.o: aux_libs/pthread_aux.c
	gcc -Wall -g3 -c aux_libs/pthread_aux.c -o pthread_aux.o

# problema_1

problema_1: problema_1.o
	gcc -Wall -O2 -g3 problema_1.c func/func.o pthread_aux.o -o problema_1 ${LIBS}
	# gcc -Wall -g3 problema_1.c func/func.o pthread_aux.o -o problema_1 ${LIBS}

problema_1.o: problema_1.c
	gcc -Wall -g3 -c problema_1.c -o problema_1.o


# test

test: test.o
	gcc -Wall -g3 test.c -o test ${LIBS}

test.o: test.c
	gcc -Wall -g3 -c test.c -o test.o






clean:
	-rm *.o
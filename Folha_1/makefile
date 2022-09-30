all: aux_libs problems clean

LIBS = -lpthread -lrt

aux_libs: pthread_info.o

problems: problema_1

# aux_libs

pthread_info.o: aux_libs/pthread_info.c
	gcc -Wall -c aux_libs/pthread_info.c -o pthread_info.o

# problema_1

problema_1: problema_1.o
	gcc -Wall -O2 -D_REENTRANT problema_1.c func/func.o pthread_info.o -o problema_1 ${LIBS}

problema_1.o: problema_1.c
	gcc -Wall -c problema_1.c -o problema_1.o








clean:
	-rm *.o
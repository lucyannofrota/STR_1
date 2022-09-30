#include <stdio.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>

#include <time.h>
// #include <sys/time.h>

#include "func/func.h"


void* time_calc_thread(void* arg){
    // [int arg1,int arg2,void (*myfunc)(int,int)]


    // Fazendo cast do input
    // int *args = (int*) arg;

    int args[] = {4,2};

    void (*myfunc)(int,int) = arg;



    // Alocando memoria para o return
    int *ret = malloc(2*sizeof(int));

    // Processo
    ret[0] = args[0] + args[1];
    ret[1] = args[0] * args[1];

    myfunc(0,1);


    // Return
    pthread_exit(ret);
}


// pthread_t launch_thread(void (*myfunc)(int,int), void* arg){
//     // myfunc      - Ponteiro para uma função que recebe um ponteiro void como argumento e retona void. Esse parametro é utilizado para dar funcionalidade ao processo filho lançado
//     // arg         - Ponteiro que indica o argumento de entrada da função apontada por 'myfunc'


//     printf("%i\n",((int *)arg)[0]);
//     printf("%i\n",((int *)arg)[1]);
//     myfunc(((int *)arg)[0],((int *)arg)[1]);

//     pthread_t thr1;

//     pthread_create(&thr1,NULL,f1, &args);

//     return thr1;
// }

int main(void){

    // Argumentos do thread

    pthread_t thr1;

    pthread_create(&thr1,NULL,time_calc_thread, f1);


    int *ret; // Variavel para receber o retorno do thread

    // Aguardando termino do thread
    pthread_join(thr1,(void**)&ret);

    printf("Soma: %i\nProduto: %i\n",ret[0],ret[1]);

    printf("\n");
    return 0;
}
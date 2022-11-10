#define N_FUNCTIONS 3
#define N_SAMPLES 128
#define PRINT_MULTP 1000/1000000000.0
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <pthread.h>
// #include <sched.h>
// #include <sys/time.h>
// #include <sys/resource.h>

#include "func/func.h"
#include "aux_libs/pthread_aux.h"
// #include "aux_libs/print_aux.h"
#include "aux_libs/aux_functions.h"


#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

// From https://man7.org/linux/man-pages/man2/clock_gettime.2.html
// struct timespec {
//     time_t   tv_sec;        /* seconds */
//     long     tv_nsec;       /* nanoseconds */
// };


static void *thread_start(void *arg){

    struct timespec (*tab)[N_FUNCTIONS][N_SAMPLES] = arg;

    printf("thread attr:\n"); display_thread_attr(pthread_self(), "\t"); printf("\n");

    calc_func_ripple(f1,N_SAMPLES,(*tab)[0]);
    calc_func_ripple(f2,N_SAMPLES,(*tab)[1]);
    calc_func_ripple(f3,N_SAMPLES,(*tab)[2]);

    return NULL;
}

int main(){

    printf("Process PID: %i\n",getpid());

    // Verificando atributos iniciais do thread main

    int s;

    pthread_t main_thread = pthread_self();

    cpu_set_t cpu_set;


    // Alterando atributos do thread main

    CPU_ZERO(&cpu_set);

    CPU_SET(0,&cpu_set);

    printf("##################\n");
    printf("##Thread configs##\n");
    printf("##################\n\n\n");

    printf("main_thread attr Initial:\n"); display_thread_attr(pthread_self(), "\t"); printf("\n");

    s = pthread_setaffinity_np(main_thread,sizeof(cpu_set), &cpu_set);
    if (s != 0) handle_error_en(s, "pthread_setaffinity_np");

    pthread_attr_t attr;
    
    thread_configs(&attr,1,SCHED_FIFO,0);

    pthread_t thr1;

    printf("main_thread attr Changed:\n"); display_thread_attr(pthread_self(), "\t"); printf("\n");

    struct timespec time_table[N_FUNCTIONS][N_SAMPLES];

    pthread_create(&thr1,&attr,thread_start, time_table);

    pthread_attr_destroy(&attr);
    if (s != 0) handle_error_en(s, "pthread_attr_destroy");

    // Aguardando o termino do thread
    pthread_join(thr1,NULL);

    printf("###### 1 ######\n");

    printf("\n\t###########\n");
    printf("\t##Results##\n");
    printf("\t###########\n\n\n");

    print_table((struct timespec*)time_table,N_FUNCTIONS,N_SAMPLES,"\t");

    printf("Ending main thread\n");

    return 0;
}

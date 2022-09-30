#define N_SAMPLES 100

#define _GNU_SOURCE

#define CLASS 2
#define GROUP 4

#include <stdio.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>

// #include <time.h>

#include "func/func.h"

#include <sys/time.h>
#include <sys/resource.h>

#include "aux_libs/pthread_aux.h"

#include <sched.h>
#include <assert.h>


#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

// From https://man7.org/linux/man-pages/man2/clock_gettime.2.html
// struct timespec {
//     time_t   tv_sec;        /* seconds */
//     long     tv_nsec;       /* nanoseconds */
// };


// sec e nsec devem ser utilizados em conjunto
struct timespec calc_func_ripple(void (*func)(int,int)){
    // int i;
    // time_t max_dtime_nsec= 0, time_1, time_2;
    // struct timespec time_1 = {0,0}, time_2 = {0,0}, max_dtime = {0,0};
    struct timespec max_dtime = {0,0};
    // for(i = 0; i < N_SAMPLES; i++){
    //     clock_gettime(CLOCK_MONOTONIC, &time_1);
    //     func(CLASS,GROUP);
    //     clock_gettime(CLOCK_MONOTONIC, &time_2);
    //     max_dtime_sec = time_2.tv_sec - time_1.tv_sec;
    //     max_dtime_nsec = time_2.tv_nsec - time_1.tv_nsec;
    //     if(max_dtime_nsec < time_2.tv_nsec - time_1.tv_nsec)
    //         max_dtime_nsec = time_2.tv_nsec - time_1.tv_nsec;
    // }

    return max_dtime;
}

void bin(unsigned n, size_t size)
{
    unsigned i;
    for (i = 1 << (unsigned) (size-1); i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
}

int main(int argc, char *argv[]){

    // Verificando atributos iniciais do thread main

    int s;

    pthread_t main_thread = pthread_self();

    cpu_set_t *cpu_set = malloc(sizeof(cpu_set_t));

    printf("main_thread:\n");

    printf("\tDefault state:\n");

    display_pthread_affinity(main_thread, "\t\t");

    // // Alterando atributos do thread main

    printf("\tChanged state:\n");

    CPU_ZERO(cpu_set);

    CPU_SET(0,cpu_set);

    // Utilizar for para alocar mais cpu's

    s = pthread_setaffinity_np(main_thread,sizeof(*cpu_set), cpu_set);

    display_pthread_affinity(main_thread, "\t\t");




    printf("main_thread attr:\n"); display_thread_sched_attr(pthread_self(), "\t");

    return 0;
}
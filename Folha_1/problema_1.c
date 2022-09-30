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

#include "aux_libs/pthread_info.h"

#include <sched.h>
#include <assert.h>


#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

// From https://man7.org/linux/man-pages/man2/clock_gettime.2.html
// struct timespec {
//     time_t   tv_sec;        /* seconds */
//     long     tv_nsec;       /* nanoseconds */
// };

long calc_func_ripple(void (*func)(int,int)){
    int i;
    // time_t max_dtime = 0, time_1, time_2;
    struct timespec time_1 = {0,0}, time_2 = {0,0};
    long max_dtime = 0;
    for(i = 0; i < N_SAMPLES; i++){
        clock_gettime(CLOCK_MONOTONIC, &time_1);
        func(CLASS,GROUP);
        clock_gettime(CLOCK_MONOTONIC, &time_2);
        max_dtime = time_2.tv_nsec - time_1.tv_nsec;
        if(max_dtime < time_2.tv_nsec - time_1.tv_nsec)
            max_dtime = time_2.tv_nsec - time_1.tv_nsec;
    }

    return max_dtime;
}

void bin(unsigned n, size_t size)
{
    unsigned i;
    for (i = 1 << (unsigned) (size-1); i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
}

int main(int argc, char *argv[]){

    cpu_set_t *setp;

    int num_cpus = 20; size_t size;

    setp = CPU_ALLOC(num_cpus);

    if(setp == NULL){
        perror("CPU_ALLOC()");
        exit(EXIT_FAILURE);
    }


    // CPU_FREE(cpusetp);

    size = CPU_ALLOC_SIZE(num_cpus);

    printf("Size: %i\nN cpus %d\n",(int) size,CPU_COUNT_S(size,setp));

    CPU_ZERO(setp);
    CPU_FREE(setp);

    printf("Size: %i\nN cpus %d\n",(int) size,CPU_COUNT_S(size,setp));

    // // num_cpus = atoi(argv[1]);

    // cpusetp = CPU_ALLOC(num_cpus);
    // if (cpusetp == NULL) {
    //     perror("CPU_ALLOC");
    //     exit(EXIT_FAILURE);
    // }

    // size = CPU_ALLOC_SIZE(num_cpus);

    // printf("Size: %d\n",(int) size);

    // CPU_ZERO_S(size, cpusetp);
    // for (int cpu = 0; cpu < num_cpus; cpu += 2)
    //     CPU_SET_S(cpu, size, cpusetp);

    // printf("CPU_COUNT() of set:    %d\n", CPU_COUNT_S(size, cpusetp));

    // CPU_FREE(cpusetp);
    // exit(EXIT_SUCCESS);
    
    printf("Main thread attr:\n"); display_thread_attributes(pthread_self(), "\t");


    // int n_aff = sched_getaffinity(pid_t pid, size_t cpusetsize,cpu_set_t *mask);

    // setpriority(PRIO_PROCESS, 0, -20);

    // int i;

    // for(i = 0; i < 20; i++){
    //     printf("f%i| Time: %.4f (ms) || ",1,calc_func_ripple(f1)/(1000.0*1000.0));
    //     printf("f%i| Time: %.4f (ms) || ",2,calc_func_ripple(f2)/(1000.0*1000.0));
    //     printf("f%i| Time: %.4f (ms)\n",3,calc_func_ripple(f3)/(1000.0*1000.0));
    // }

    return 0;
}
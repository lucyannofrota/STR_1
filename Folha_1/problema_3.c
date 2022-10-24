#define N_FUNCTIONS 3
#define N_SAMPLES 6

#define PRINT_MULTP 1000/1000000000.0

#define _GNU_SOURCE

#define CLASS 2
#define GROUP 4

#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "func/func.h"

#include <sys/time.h>
#include <sys/resource.h>

#include "aux_libs/pthread_aux.h"

#include <sched.h>
#include <assert.h>

#include "aux_libs/print_aux.h"


#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

// From https://man7.org/linux/man-pages/man2/clock_gettime.2.html
// struct timespec {
//     time_t   tv_sec;        /* seconds */
//     long     tv_nsec;       /* nanoseconds */
// };

struct thread_arg{
    int thread_number;
    u_int16_t ms_period;
    void (*func)(int,int);
    struct timespec *time_table;
    pthread_barrier_t *barrier;
};

void clk_wait(double ms_sec){
    // https://stackoverflow.com/questions/20332382/linux-sleeping-with-clock-nanosleep
    if(ms_sec <= 0) return;
    struct timespec deadline;
    clock_gettime(CLOCK_REALTIME, &deadline);

    // Add the time you want to sleep
    deadline.tv_nsec += (long) ceil(ms_sec*1000000000/1000.0);

    // Normalize the time to account for the second boundary
    if(deadline.tv_nsec >= 1000000000) {
        deadline.tv_nsec -= 1000000000;
        deadline.tv_sec++;
    }
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &deadline, NULL);
}

void add_timespec(const struct timespec *tim_1,const struct timespec *tim_2,struct timespec *result){
    // from <sys/time.h>
    // define timeradd(a, b, result)
    result->tv_sec = tim_1->tv_sec + tim_2->tv_sec;
    result->tv_nsec = tim_1->tv_nsec + tim_2->tv_nsec;
    if(result->tv_nsec >= 1e9){
        result->tv_sec++;
        result->tv_nsec -= 1e9;
    }
}

double dtime_ms(const struct timespec *tim_1,const struct timespec *tim_2){
    // from <sys/time.h>
    // # define timersub(a, b, result)
    struct timespec result;
    result.tv_sec = tim_1->tv_sec - tim_2->tv_sec;
    result.tv_nsec = tim_1->tv_nsec - tim_2->tv_nsec;
    if(result.tv_nsec < 0 && result.tv_sec > 0){
        result.tv_sec --;
        result.tv_nsec += 1e9;
    }
    return (result.tv_sec*1e9 + ((long int)result.tv_nsec))/1000000.0;
}

void *thread_start(void *arg){
    struct thread_arg *thr_arg = arg;

    printf("thread %i attr:\n",thr_arg->thread_number); display_thread_attr(pthread_self(), "\t"); printf("\n");

    printf("Th%i Wainting other threads\n",thr_arg->thread_number);

    pthread_barrier_wait(thr_arg->barrier);
    clock_gettime(CLOCK_REALTIME, &(thr_arg->time_table[0]));
    struct timespec period = {0,thr_arg->ms_period*1e6};
    printf("Th%i Running...\n",thr_arg->thread_number);
    int i;
    for(i = 0; i < N_SAMPLES; i++){
        if(i==0) add_timespec(&(thr_arg->time_table[0]),&period,&(thr_arg->time_table[1+i*2]));
        else add_timespec(&(thr_arg->time_table[1+(i-1)*2]),&period,&(thr_arg->time_table[1+i*2]));
    }

    for(i = 0; i < N_SAMPLES; i++){
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &(thr_arg->time_table[1+i*2]), NULL); // 1,3,5,7,9,11
        thr_arg->func(CLASS,GROUP);
        clock_gettime(CLOCK_REALTIME, &(thr_arg->time_table[2+2*i])); // 2,4,6,8,10,12
    }

    printf("Th%i done\n",thr_arg->thread_number);

    return NULL;
}

int main(){

    printf("Process PID: %i\n",getpid());

    int s;
    cpu_set_t cpu_set;
    pthread_t main_thread = pthread_self();

    // Alterando atributos do thread main

    CPU_ZERO(&cpu_set);
    CPU_SET(0,&cpu_set);

    // // Utilizar for para alocar mais cpu's
    
    printf("##################\n");
    printf("##Thread configs##\n");
    printf("##################\n\n\n");

    printf("main_thread attr Initial:\n"); display_thread_attr(pthread_self(), "\t"); printf("\n");

    s = pthread_setaffinity_np(main_thread,sizeof(cpu_set), &cpu_set);
    if (s != 0) handle_error_en(s, "pthread_attr_destroy");

    pthread_attr_t attr1,attr2,attr3;
    
    thread_configs(&attr1,0); thread_configs(&attr2,-5); thread_configs(&attr3,-10);

    printf("main_thread attr Changed:\n"); display_thread_attr(pthread_self(), "\t"); printf("\n");

    struct timespec time_table[N_FUNCTIONS][N_SAMPLES*2+1];
    
    double time_table_dbl[N_FUNCTIONS][N_SAMPLES*2+1];

    pthread_t thr[3];

    pthread_barrier_t barrier;

    s = pthread_barrier_init(&barrier,NULL,3);
    if (s != 0) handle_error_en(s, "pthread_barrier_init");


    struct thread_arg thr_arg[3] = {
        {1,100,f1,time_table[0],&barrier},
        {2,200,f2,time_table[1],&barrier},
        {3,300,f3,time_table[2],&barrier}
    };

    // https://docs.oracle.com/cd/E19455-01/806-5257/6je9h032r/index.html

    pthread_create(&thr[0],&attr1,thread_start, &thr_arg[0]);
    pthread_create(&thr[1],&attr2,thread_start, &thr_arg[1]);
    pthread_create(&thr[2],&attr3,thread_start, &thr_arg[2]);

    pthread_join(thr[0],NULL);
    pthread_join(thr[1],NULL);
    pthread_join(thr[2],NULL);

    s = pthread_attr_destroy(&attr1);
    if (s != 0) handle_error_en(s, "pthread_attr_destroy");
    s = pthread_attr_destroy(&attr2);
    if (s != 0) handle_error_en(s, "pthread_attr_destroy");
    s = pthread_attr_destroy(&attr3);
    if (s != 0) handle_error_en(s, "pthread_attr_destroy");
    s = pthread_barrier_destroy(&barrier);
    if (s != 0) handle_error_en(s, "pthread_barrierattr_destroy");


    // Aguardando o termino do thread

    printf("\n###### 3 ######\n");

    printf("\n\t###########\n");
    printf("\t##Results##\n");
    printf("\t###########\n\n\n");

    for(int i = 0; i < N_FUNCTIONS; i++){
        for(int j = 0; j < N_SAMPLES*2+1; j++){
            time_table_dbl[i][j] = dtime_ms(&(time_table[i][j]),&(time_table[0][0]));
        }
    }

    print_table_double(N_FUNCTIONS,N_SAMPLES*2+1,time_table_dbl,"\t");

    printf("Ending main thread\n");

    return 0;
}
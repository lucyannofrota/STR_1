#define N_FUNCTIONS 3
#define N_SAMPLES 60 // 60
#define N_TIME 6000 // ms

#define PRINT_MULTP 1000/1000000000.0

#define I_TIME_S 3

#define _GNU_SOURCE

#define CLASS 2
#define GROUP 4

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "func/func.h"
#include "aux_libs/pthread_aux.h"
#include "aux_libs/aux_functions.h"


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
    struct timespec *time_ref;
    struct timespec *sched_times;
    struct timespec *time_table;
};

void *thread_start(void *arg){
    struct thread_arg *thr_arg = arg;

    printf("thread %i attr:\n",thr_arg->thread_number); display_thread_attr(pthread_self(), "\t"); printf("\n");

    printf("Th%i Wainting other threads\n",thr_arg->thread_number);

    int i;
    const struct timespec period = {0,thr_arg->ms_period*1e6}, i_period = {I_TIME_S,0*1e6};
    add_timespec(thr_arg->time_ref,&i_period,&(thr_arg->sched_times[0]));
    for(i = 0; i < N_SAMPLES; i++) add_timespec(&(thr_arg->sched_times[i]),&period,&(thr_arg->sched_times[i+1]));
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &(thr_arg->sched_times[0]), NULL);
    printf("Th%i [Running...]\n",thr_arg->thread_number);

    for(i = 0; i < N_SAMPLES; i++){
        if(N_TIME <= i*thr_arg->ms_period) {
            const struct timespec zer = {0,0};
            thr_arg->time_table[2*i] = zer;
            thr_arg->time_table[2*i+1] = zer;
        }
        else{
            clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &(thr_arg->sched_times[i+1]), NULL);
            clock_gettime(CLOCK_REALTIME, &(thr_arg->time_table[2*i]));
            thr_arg->func(CLASS,GROUP);
            clock_gettime(CLOCK_REALTIME, &(thr_arg->time_table[1+2*i]));
        }
    }

    printf("Th%i [Done]\n",thr_arg->thread_number);

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
    if (s != 0) handle_error_en(s, "pthread_setaffinity_np");

    pthread_attr_t attr1,attr2,attr3;
    
    thread_configs(&attr1,1,SCHED_FIFO,0); thread_configs(&attr2,1,SCHED_FIFO,-1); thread_configs(&attr3,1,SCHED_FIFO,-2);

    printf("main_thread attr Changed:\n"); display_thread_attr(pthread_self(), "\t"); printf("\n");

    struct timespec time_table[N_FUNCTIONS][N_SAMPLES*2];
    struct timespec sched_table[N_FUNCTIONS][N_SAMPLES+1];
    
    double time_table_dbl[N_FUNCTIONS][N_SAMPLES*2];
    double sched_table_dbl[N_FUNCTIONS][N_SAMPLES+1];

    pthread_t thr[3];

    struct timespec time_ref;

    clock_gettime(CLOCK_REALTIME, &time_ref);

    struct thread_arg thr_arg[3] = {
        {1,100,f1,&time_ref,sched_table[0],time_table[0]},
        {2,200,f2,&time_ref,sched_table[1],time_table[1]},
        {3,300,f3,&time_ref,sched_table[2],time_table[2]}
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

    // Aguardando o termino do thread

    printf("\n###### 3 ######\n");

    printf("\n\t###########\n");
    printf("\t##Results##\n");
    printf("\t###########\n\n\n");

    int lens[N_FUNCTIONS] = {0,0,0};

    for(int i = 0; i < N_FUNCTIONS; i++){
        for(int j = 0; j < N_SAMPLES+1; j++){
            if(sched_table[i][j].tv_sec == 0 && sched_table[i][j].tv_nsec == 0) sched_table_dbl[i][j] = 0;
            else sched_table_dbl[i][j] = dtime_ms(&(sched_table[i][j]),&(sched_table[0][0]));
        }
        for(int j = 0; j < N_SAMPLES; j++){
            if(time_table[i][j*2].tv_sec == 0 && time_table[i][j*2].tv_nsec == 0) time_table_dbl[i][j*2] = 0;
            else{
                time_table_dbl[i][j*2] = dtime_ms(&(time_table[i][j*2]),&(sched_table[0][0]));
                time_table_dbl[i][j*2+1] = dtime_ms(&(time_table[i][j*2+1]),&(sched_table[0][0]));
                lens[i]++;
            }
        }
    }

    report_times(N_FUNCTIONS,N_SAMPLES+1,lens,sched_table_dbl,time_table_dbl,NULL,"\t\t");

    printf("Ending main thread\n");

    return 0;
}
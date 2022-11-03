#define N_FUNCTIONS 3
#define N_SAMPLES 128

#define PRINT_MULTP 1000/1000000000.0

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
#include "aux_libs/print_aux.h"


#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

// From https://man7.org/linux/man-pages/man2/clock_gettime.2.html
// struct timespec {
//     time_t   tv_sec;        /* seconds */
//     long     tv_nsec;       /* nanoseconds */
// };

void clk_wait(double m_sec){
    // https://stackoverflow.com/questions/20332382/linux-sleeping-with-clock-nanosleep

    struct timespec deadline;
    clock_gettime(CLOCK_REALTIME, &deadline);

    // Add the time you want to sleep
    deadline.tv_nsec += (long) ceil(m_sec*1000000000/1000.0);

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

struct timespec sub_timespec(struct timespec tim_1,struct timespec tim_2){
    struct timespec result;
    result.tv_sec = tim_1.tv_sec - tim_2.tv_sec;
    result.tv_nsec = tim_1.tv_nsec - tim_2.tv_nsec;
    if(result.tv_nsec < 0){
        result.tv_sec -= 1;
        result.tv_nsec += 1000000000;
    }
    return result;
}

void calc_func_ripple(void (*func)(void), struct timespec dtime_spec[N_SAMPLES]){
    int i;
    struct timespec time1 = {0,0}, time2 = {0,0};

    for(i = 0; i < N_SAMPLES; i++){
        clock_gettime(CLOCK_REALTIME, &time1);
        func();
        clock_gettime(CLOCK_REALTIME, &time2);
        dtime_spec[i] = sub_timespec(time2,time1);
        clk_wait(5); // 5 ms
    }
}

void sim_func(int time_us,int faction){
    struct timespec period = {0,time_us*1e3}, time;
    for(int i = 0; i < time_us/faction; i++){
        // print_timespec(time,"\t\t");
        clock_gettime(CLOCK_REALTIME, &time);
        add_timespec(&time,&period,&time);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &period, NULL);
    }
}

void f1_(void){sim_func(35e6,1000);}
void f2_(void){sim_func(55e6,1000);}
void f3_(void){sim_func(85e6,1000);}


static void *thread_start(void *arg){

    struct timespec (*tab)[N_FUNCTIONS][N_SAMPLES] = arg;

    printf("thread attr:\n"); display_thread_attr(pthread_self(), "\t"); printf("\n");

    calc_func_ripple(f1_,(*tab)[0]);
    calc_func_ripple(f2_,(*tab)[1]);
    calc_func_ripple(f3_,(*tab)[2]);

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

    printf("###### 5B ######\n");

    printf("\n\t###########\n");
    printf("\t##Results##\n");
    printf("\t###########\n\n\n");

    print_table((struct timespec*)time_table,N_FUNCTIONS,N_SAMPLES,"\t");

    printf("Ending main thread\n");

    return 0;
}

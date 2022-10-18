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

// sec e nsec devem ser utilizados em conjunto

struct thread_arg{
    int num;
    u_int16_t ms_period;
    void (*func)(int,int);
    struct timespec *time_table;
    pthread_barrier_t *barrier;
};

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

struct timespec diff_timespec(const struct timespec *time1,
    const struct timespec *time0) {
  assert(time1 && time1->tv_nsec < 1000000000);
  assert(time0 && time0->tv_nsec < 1000000000);
  struct timespec diff = {.tv_sec = time1->tv_sec - time0->tv_sec, .tv_nsec =
      time1->tv_nsec - time0->tv_nsec};
  if (diff.tv_nsec < 0) {
    diff.tv_nsec += 1000000000;
    diff.tv_sec--;
  }
  return diff;
}


int setup_periodic_timer(unsigned int ms_period, sigset_t *alarm_sig){
    // https://github.com/csimmonds/periodic-threads/blob/master/itimer.c
	int ret;
	struct itimerval tim;

	/* Block SIGALRM in this thread */
	sigemptyset(alarm_sig);
	sigaddset(alarm_sig, SIGALRM);
	pthread_sigmask(SIG_BLOCK, alarm_sig, NULL);

	tim.it_value.tv_sec = ms_period / 1000;
	tim.it_value.tv_usec = (ms_period % 1000) * 1000;
    // printf("tim %i, %i\n",tim.it_value.tv_sec,tim.it_value.tv_usec);
	tim.it_interval.tv_sec = ms_period / 1000;
	tim.it_interval.tv_usec = (ms_period % 1000) * 1000;
	ret = setitimer(ITIMER_REAL, &tim, NULL);
	if (ret != 0)
		perror("Failed to set timer");
	return ret;
}

void *thread_start(void *arg){

    struct thread_arg *thr_arg = arg;


    printf("thread attr:\n"); display_thread_attr(pthread_self(), "\t"); printf("\n");

    printf("Th%i Wainting other threads\n",thr_arg->num);
    pthread_barrier_wait(thr_arg->barrier);
    printf("Th%i Running...\n",thr_arg->num);

    int s, num;

    sigset_t alarm_sig;

    
    // setup_periodic_timer(thr_arg->ms_period,&alarm_sig);

    int i;
    for(i = 0; i < N_SAMPLES; i++){
        clock_gettime(CLOCK_REALTIME, &(thr_arg->time_table[2*i]));
        thr_arg->func(CLASS,GROUP);
        clock_gettime(CLOCK_REALTIME, &(thr_arg->time_table[1+2*i]));


        // printf("Dif: %6.2f | t = %i ms| sub: %6.2f\n",timespec_to_double_ms(sub_timespec(thr_arg->time_table[1+2*i],thr_arg->time_table[2*i])),thr_arg->ms_period,thr_arg->ms_period-timespec_to_double_ms(sub_timespec(thr_arg->time_table[1+2*i],thr_arg->time_table[2*i])));
        clk_wait(thr_arg->ms_period-timespec_to_double_ms(sub_timespec(thr_arg->time_table[1+2*i],thr_arg->time_table[2*i])));
        // s = sigwait(&alarm_sig,&num);
        // if (s != 0) handle_error_en(s, "sigwait");
    }

    // for(i = N_SAMPLES*2-1; i >= 0; i--){
    //     thr_arg->time_table[i] = sub_timespec(thr_arg->time_table[i],thr_arg->time_table[0]);
    // }

    // for(i = N_SAMPLES-1; i >= 0; i--){
    //     thr_arg->time_table[]
    //     // double aux = sub_timespec(thr_arg->time_table[1+2*i],thr_arg->time_table[2*i]);
    //     thr_arg->time_table[2*i] = sub_timespec(thr_arg->time_table[1+2*i],thr_arg->time_table[2*i]);
    //     thr_arg->time_table[2*i+1] = sub_timespec(thr_arg->time_table[2*i+2],thr_arg->time_table[2*i+1]);
    // }

    printf("Th%i done\n",thr_arg->num);

    return NULL;
}

int main(){

    // signal(SIGTSTP, catch_SIGTSTP);

    // Verificando atributos iniciais do thread main
    sigset_t alarm_sig;
    sigemptyset(&alarm_sig);
	sigaddset(&alarm_sig, SIGALRM);
	sigprocmask(SIG_BLOCK, &alarm_sig, NULL);

    printf("Process PID: %i\n",getpid());

    int s;

    pthread_t main_thread = pthread_self();

    cpu_set_t cpu_set;


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

    pthread_attr_t attr;
    
    thread_configs(&attr);

    printf("main_thread attr Changed:\n"); display_thread_attr(pthread_self(), "\t"); printf("\n");

    struct timespec time_table[N_FUNCTIONS][N_SAMPLES*2];

    pthread_t thr[3];

    pthread_barrier_t barrier;

    s = pthread_barrier_init(&barrier,NULL,3);
    if (s != 0) handle_error_en(s, "pthread_barrier_init");


    struct thread_arg thr_arg[3] = {
        {1,100,f2,time_table[1],&barrier},
        {2,200,f3,time_table[2],&barrier},
        {3,300,f1,time_table[0],&barrier}
    };

    // https://docs.oracle.com/cd/E19455-01/806-5257/6je9h032r/index.html




    pthread_create(&thr[0],&attr,thread_start, &thr_arg[0]);
    change_thread_priority(&attr,-1);
    pthread_create(&thr[1],&attr,thread_start, &thr_arg[1]);
    change_thread_priority(&attr,-2);
    pthread_create(&thr[2],&attr,thread_start, &thr_arg[2]);

    pthread_join(thr[0],NULL);
    pthread_join(thr[1],NULL);
    pthread_join(thr[2],NULL);

    s = pthread_attr_destroy(&attr);
    if (s != 0) handle_error_en(s, "pthread_attr_destroy");
    s = pthread_barrier_destroy(&barrier);
    if (s != 0) handle_error_en(s, "pthread_barrierattr_destroy");


    // Aguardando o termino do thread

    printf("\n###### 3 ######\n");

    printf("\n\t###########\n");
    printf("\t##Results##\n");
    printf("\t###########\n\n\n");


    for(int i = 0; i < N_FUNCTIONS; i++){
        for(int j = 0; j < N_SAMPLES*2; j++){
            time_table[i][j] = sub_timespec(time_table[i][j],time_table[0][0]);
        }
    }

    print_table((struct timespec*)time_table,N_FUNCTIONS,N_SAMPLES*2,"\t");

    printf("Ending main thread\n");



    // Libertando a memoria alocada pelo thread


    return 0;
}
#define N_SAMPLES 32

#define PRINT_MULTP 1000/1000000000.0

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

void clk_wait(long n_sec){
    // https://stackoverflow.com/questions/20332382/linux-sleeping-with-clock-nanosleep

    struct timespec deadline;
    clock_gettime(CLOCK_REALTIME, &deadline);

    // Add the time you want to sleep
    deadline.tv_nsec += n_sec;

    // Normalize the time to account for the second boundary
    if(deadline.tv_nsec >= 1000000000) {
        deadline.tv_nsec -= 1000000000;
        deadline.tv_sec++;
    }
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &deadline, NULL);
}

void print_timespec(struct timespec t,char *prefix){
    printf("%s sec: %i\n%snsec: %ld\n",prefix,(int)t.tv_sec,prefix,t.tv_nsec);
}

struct timespec sub_timespec(struct timespec tim_1,struct timespec tim_2){
    struct timespec result;
    result.tv_sec = tim_1.tv_sec - tim_2.tv_sec;
    result.tv_nsec = tim_1.tv_nsec - tim_2.tv_nsec;
    if(result.tv_nsec < 0){
        result.tv_sec -= 1;
        result.tv_nsec += 1000000000;
    }
    // print_timespec(result,"");
    return result;
}

double timespec_to_double_ms(struct timespec time){
    return ((time.tv_sec*1000000000+time.tv_nsec)/1000000000.0)*1000; // ms
}

void calc_func_ripple(void (*func)(int,int), long *dtime, struct timespec *dtime_spec){
    int i;
    // double dtime = 0;
    // time_t max_dtime_nsec= 0, time_1, time_2;
    // struct timespec time_1 = {0,0}, time_2 = {0,0}, max_dtime = {0,0};
    struct timespec time1 = {0,0}, time2 = {0,0};

    for(i = 0; i < N_SAMPLES; i++){
        // clock_gettime(CLOCK_MONOTONIC, &time1);
        clock_gettime(CLOCK_REALTIME, &time1);
        // CLOCK_REALTIME
        func(CLASS,GROUP);
        // clock_gettime(CLOCK_MONOTONIC, &time2);
        clock_gettime(CLOCK_REALTIME, &time2);
        dtime[i] = (((long)time2.tv_sec)-((long)time1.tv_sec))*1000000000+time2.tv_nsec-time1.tv_nsec;
        dtime_spec[i] = sub_timespec(time2,time1);
        // if(max_dtime < dtime[i]) max_dtime = dtime[i];
        clk_wait((long)5*1000000000/1000); // 3 ms
        // {
        //     // printf("sec: %ld\n",(long)max_dtime.tv_sec);
        //     if()
        //     max_dtime.tv_sec = time2.tv_sec-time1.tv_sec;
        //     if(max_dtime.tv_nsec < time2.tv_nsec-time1.tv_nsec){
        //         max_dtime.tv_nsec = time2.tv_nsec-time1.tv_nsec;
        //     }
        // }
        // max_dtime_sec = time_2.tv_sec - time_1.tv_sec;
        // max_dtime_nsec = time_2.tv_nsec - time_1.tv_nsec;
        // if(max_dtime_nsec < time_2.tv_nsec - time_1.tv_nsec)
        //     max_dtime_nsec = time_2.tv_nsec - time_1.tv_nsec;
        // clock_nanosleep()
    }

    // for(i = 0; i < N_SAMPLES; i++){
    //     clock_gettime(CLOCK_MONOTONIC, &time_1);
    //     func(CLASS,GROUP);
    //     clock_gettime(CLOCK_MONOTONIC, &time_2);
    //     max_dtime_sec = time_2.tv_sec - time_1.tv_sec;
    //     max_dtime_nsec = time_2.tv_nsec - time_1.tv_nsec;
    //     if(max_dtime_nsec < time_2.tv_nsec - time_1.tv_nsec)
    //         max_dtime_nsec = time_2.tv_nsec - time_1.tv_nsec;
    // }

    // long dtime = ((long)max_dtime.tv_sec)*1000000000+max_dtime.tv_nsec;
    // if(max_dtime.tv_nsec < 0){
    //     dtime -= 1000000000;
    // }
    // long dtime = ((long)max_dtime.tv_sec)*1000000000+max_dtime.tv_nsec;

    // printf("Nsec: %ld\n",dtime);

    // return max_dtime;
    // return (float) dtime/1000000000.0;
}

// void bin(unsigned n, size_t size)
// {
//     unsigned i;
//     for (i = 1 << (unsigned) (size-1); i > 0; i = i / 2)
//         (n & i) ? printf("1") : printf("0");
// }

// void* calc_thread(void* arg){
//     printf("asdasd\n");
//     // Inicializando ponteiro de retorno
//     int *ret = malloc(2*sizeof(int));

//     // Ponteiro para receber o argumento de entrada
//     int *args = (int*) arg;

//     printf("Os valores recebidos pelo thread são: %i, %i\n",args[0],args[1]);

//     ret[0] = args[0] + args[1];
//     ret[1] = args[0] * args[1];

//     printf("thread attr:\n"); display_thread_attr(pthread_self(), "\t");

//     pthread_exit(ret);
// }

static void *thread_start(void *arg){
    // int rep = 16, pot = 4;

    printf("thread attr:\n"); display_thread_attr(pthread_self(), "\t");

    long dtime_1[N_SAMPLES], dtime_2[N_SAMPLES], dtime_3[N_SAMPLES];

    struct timespec dtime_spec_1[N_SAMPLES], dtime_spec_2[N_SAMPLES], dtime_spec_3[N_SAMPLES];

    // long t1 = calc_func_ripple(f1,dtime_1,dtime_spec_1), t2 = calc_func_ripple(f2,dtime_2,dtime_spec_2), t3 = calc_func_ripple(f3,dtime_3,dtime_spec_3);
    calc_func_ripple(f1,dtime_1,dtime_spec_1);
    calc_func_ripple(f2,dtime_2,dtime_spec_2);
    calc_func_ripple(f3,dtime_3,dtime_spec_3);

    // int i;

    // for(i = 0; i < rep; i++){
    //     t1 = calc_func_ripple(f1);
    //     t2 = calc_func_ripple(f2);
    //     t3 = calc_func_ripple(f3);

    //     printf("f1 = %2.5f | f2 = %2.5f | f3 = %2.5f\n",(float) t1/1000000000.0,(float) t2/1000000000.0,(float) t3/1000000000.0);
    // }


    printf("Times (%i samples):",N_SAMPLES);
    for(int i = 0; i < N_SAMPLES; i++){
        printf("\t1 | f1 = %2.5f | f2 = %2.5f | f3 = %2.5f\n",dtime_1[i]*PRINT_MULTP,dtime_2[i]*PRINT_MULTP,dtime_3[i]*PRINT_MULTP);
        printf("\t2 | f1 = %2.5f | f2 = %2.5f | f3 = %2.5f\n",timespec_to_double_ms(dtime_spec_1[i]),timespec_to_double_ms(dtime_spec_2[i]),timespec_to_double_ms(dtime_spec_3[i]));
    }

    // printf("Max times (%i samples): \n\tf1 = %2.5f | f2 = %2.5f | f3 = %2.5f\n",N_SAMPLES,(float) 1000*t1/1000000000.0,(float) 1000*t2/1000000000.0,(float) 1000*t3/1000000000.0);

    // printf("Sec: %f\n",calc_func_ripple(f1));

    return NULL;
}

int *ret;

int main(){

    // Verificando atributos iniciais do thread main

    int s;

    pthread_t main_thread = pthread_self();

    cpu_set_t cpu_set;


    // Alterando atributos do thread main

    CPU_ZERO(&cpu_set);

    CPU_SET(0,&cpu_set);

    // // Utilizar for para alocar mais cpu's

    printf("main_thread attr Initial:\n"); display_thread_attr(main_thread, "\t");

    s = pthread_setaffinity_np(main_thread,sizeof(cpu_set), &cpu_set);
    if (s != 0) handle_error_en(s, "pthread_attr_destroy");

    pthread_attr_t attr;
    
    thread_configs(&attr);

    pthread_t thr1;

    printf("main_thread attr Changed:\n"); display_thread_attr(pthread_self(), "\t");


    pthread_create(&thr1,&attr,thread_start, NULL);


    // Aguardando o termino do thread
    // pthread_join(thr1,(void**)&ret);
    pthread_join(thr1,NULL);

    printf("Ending main thread\n");



    // Libertando a memoria alocada pelo thread
    free(ret);


    return 0;
}
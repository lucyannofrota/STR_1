#include "pthread_aux.h"

void thread_configs(pthread_attr_t *attr,int setaffinity, int sched_type,int priority_mod){
    // https://man7.org/linux/man-pages/man7/sched.7.html

    pthread_attr_init(attr);

    // Affinity

    if(setaffinity == 1){
        cpu_set_t cpu_set;

        CPU_ZERO(&cpu_set);
        CPU_SET(0,&cpu_set);

        pthread_attr_setaffinity_np(attr, sizeof(cpu_set_t), &cpu_set);
    }
    // Inheritance

    pthread_attr_setinheritsched(attr,PTHREAD_EXPLICIT_SCHED);

    /* Policy options:
    //  SCHED_FIFO
    //  SCHED_RR
    //  SCHED_OTHER
    */

    pthread_attr_setschedpolicy(attr,sched_type);

    struct sched_param param;

    param.sched_priority = sched_get_priority_max(sched_type) + priority_mod;

    pthread_attr_setschedparam(attr, &param);

}

void change_thread_priority(pthread_t thread,int priority){
    int s;
    struct sched_param param;
    param.sched_priority = priority;

    s = pthread_setschedparam(thread, SCHED_FIFO,&param);
    if (s != 0) handle_error_en(s, "pthread_setschedparam");
}



void display_thread_attr(pthread_t thread, char *prefix){
    // https://man7.org/linux/man-pages/man3/pthread_getattr_np.3.html
    int s;
    pthread_attr_t attr;
    // pthread_attr_init(&attr);

    s = pthread_getattr_np(thread, &attr);
    if (s != 0) handle_error_en(s, "pthread_getattr_np");

    display_attr(attr, prefix);






    // // https://man7.org/linux/man-pages/man3/pthread_attr_init.3.html
    // // https://man7.org/linux/man-pages/man3/pthread_attr_destroy.3.html


    s = pthread_attr_destroy(&attr);
    if (s != 0) handle_error_en(s, "pthread_attr_destroy");
}

void display_attr(pthread_attr_t attr, char *prefix){
    // https://man7.org/linux/man-pages/man3/pthread_getattr_np.3.html
    int s;




    // https://man7.org/linux/man-pages/man3/pthread_attr_init.3.html
    // https://man7.org/linux/man-pages/man3/pthread_attr_destroy.3.html
    int i;
    struct sched_param sp;
    


    cpu_set_t cpu_set;


    s = pthread_attr_getaffinity_np(&attr, sizeof(cpu_set), &cpu_set);
    if (s != 0) handle_error_en(s, "pthread_attr_getaffinity_np");
    printf("%sThread affinity = %d\n", prefix, CPU_COUNT(&cpu_set));

    s = pthread_attr_getinheritsched(&attr, &i);
    if (s != 0) handle_error_en(s, "pthread_attr_getinheritsched");

    printf("%sInherit scheduler   = %s\n", prefix,
            (i == PTHREAD_INHERIT_SCHED)  ? "PTHREAD_INHERIT_SCHED" :
            (i == PTHREAD_EXPLICIT_SCHED) ? "PTHREAD_EXPLICIT_SCHED" :
            "???");

    s = pthread_attr_getschedpolicy(&attr, &i);
    if (s != 0) handle_error_en(s, "pthread_attr_getschedpolicy");
    printf("%sScheduling policy   = %s\n", prefix,
            (i == SCHED_OTHER) ? "SCHED_OTHER" :
            (i == SCHED_FIFO)  ? "SCHED_FIFO" :
            (i == SCHED_RR)    ? "SCHED_RR" :
            "???");

    s = pthread_attr_getschedparam(&attr, &sp);
    if (s != 0) handle_error_en(s, "pthread_attr_getschedparam");
    printf("%sScheduling priority = %d\n", prefix, sp.sched_priority);
}
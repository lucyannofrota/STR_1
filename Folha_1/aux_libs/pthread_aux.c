#include "pthread_aux.h"

// void set_pthread_affinity(pthread_t thread){
//     // pthread_setaffinity_np(thread,sizeof(*cpu_set),cpu_set);
// }

// void set_pthread_sched_attr(pthread_t thread, pthread_attr_t * attr){

// }





void thread_configs(pthread_attr_t *attr){
    // https://man7.org/linux/man-pages/man7/sched.7.html

    pthread_attr_init(attr);

    // Affinity

    cpu_set_t cpu_set;

    CPU_ZERO(&cpu_set);
    CPU_SET(0,&cpu_set);

    pthread_attr_setaffinity_np(attr, sizeof(cpu_set_t), &cpu_set);

    // Inheritance

    pthread_attr_setinheritsched(attr,PTHREAD_EXPLICIT_SCHED);

    /* Policy options:
    //  SCHED_FIFO
    //  SCHED_RR
    //  SCHED_OTHER
    */

    pthread_attr_setschedpolicy(attr,SCHED_FIFO);

    struct sched_param param;

    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    // if(opt == 0) param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    // else param.sched_priority = sched_get_priority_min(SCHED_FIFO);


    pthread_attr_setschedparam(attr, &param);

    // sched_get_priority_max()
    // sched_get_priority_min()

}

void change_thread_priority(pthread_attr_t *attr,int priority_mod){
    struct sched_param param;

    // printf("Max: %i\n",sched_get_priority_max(SCHED_FIFO)+priority_mod);

    param.sched_priority = sched_get_priority_max(SCHED_FIFO)+priority_mod;

    pthread_attr_setschedparam(attr, &param);
}

// void display_pthread_affinity(pthread_t thread, char *prefix){

//     int s;

//     cpu_set_t cpu_set;

//     printf("%sthread affinity:\n",prefix);

//     // Alterando atributos do thread main

//     s = pthread_getaffinity_np(thread,sizeof(cpu_set), &cpu_set);

//     if (s != 0) handle_error_en(s, "pthread_getaffinity_np");
    
//     printf("%s\tcpu count = %i\n", prefix, CPU_COUNT(&cpu_set));

// }



void display_thread_attr(pthread_t thread, char *prefix){
    // https://man7.org/linux/man-pages/man3/pthread_getattr_np.3.html
    int s;
    pthread_attr_t attr;
    // pthread_attr_init(&attr);

    s = pthread_getattr_np(thread, &attr);
    if (s != 0) handle_error_en(s, "pthread_getattr_np");

    display_attr(attr, prefix);

    // // thread_configs(&attr);

    // // display_pthread_attr(&attr, prefix);




    // // https://man7.org/linux/man-pages/man3/pthread_attr_init.3.html
    // // https://man7.org/linux/man-pages/man3/pthread_attr_destroy.3.html
    // int i;
    // struct sched_param sp;
    
    // // s = pthread_attr_getdetachstate(attr, &i);
    // // if (s != 0)
    // //     handle_error_en(s, "pthread_attr_getdetachstate");
    // // printf("%sDetach state        = %s\n", prefix,
    // //         (i == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
    // //         (i == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" :
    // //         "???");

    // // s = pthread_attr_getscope(attr, &i);
    // // if (s != 0)
    // //     handle_error_en(s, "pthread_attr_getscope");
    // // printf("%sScope               = %s\n", prefix,
    // //         (i == PTHREAD_SCOPE_SYSTEM)  ? "PTHREAD_SCOPE_SYSTEM" :
    // //         (i == PTHREAD_SCOPE_PROCESS) ? "PTHREAD_SCOPE_PROCESS" :
    // //         "???");

    // cpu_set_t cpu_set;

    // s = pthread_attr_getaffinity_np(&attr, sizeof(cpu_set_t), &cpu_set);
    // if (s != 0) handle_error_en(s, "pthread_attr_getaffinity_np");
    // printf("%sThread affinity = %d\n", prefix, CPU_COUNT(&cpu_set));

    // s = pthread_attr_getinheritsched(&attr, &i);
    // if (s != 0) handle_error_en(s, "pthread_attr_getinheritsched");

    // printf("%sInherit scheduler   = %s\n", prefix,
    //         (i == PTHREAD_INHERIT_SCHED)  ? "PTHREAD_INHERIT_SCHED" :
    //         (i == PTHREAD_EXPLICIT_SCHED) ? "PTHREAD_EXPLICIT_SCHED" :
    //         "???");

    // s = pthread_attr_getschedpolicy(&attr, &i);
    // if (s != 0) handle_error_en(s, "pthread_attr_getschedpolicy");
    // printf("%sScheduling policy   = %s\n", prefix,
    //         (i == SCHED_OTHER) ? "SCHED_OTHER" :
    //         (i == SCHED_FIFO)  ? "SCHED_FIFO" :
    //         (i == SCHED_RR)    ? "SCHED_RR" :
    //         "???");

    // s = pthread_attr_getschedparam(&attr, &sp);
    // if (s != 0) handle_error_en(s, "pthread_attr_getschedparam");
    // printf("%sScheduling priority = %d\n", prefix, sp.sched_priority);

    // // pthread_attr_getaffinity_np()

    s = pthread_attr_destroy(&attr);
    if (s != 0) handle_error_en(s, "pthread_attr_destroy");
}

void display_attr(pthread_attr_t attr, char *prefix){
    // https://man7.org/linux/man-pages/man3/pthread_getattr_np.3.html
    int s;
    // pthread_attr_t attr;



    // thread_configs(&attr);

    // display_pthread_attr(&attr, prefix);




    // https://man7.org/linux/man-pages/man3/pthread_attr_init.3.html
    // https://man7.org/linux/man-pages/man3/pthread_attr_destroy.3.html
    int i;
    struct sched_param sp;
    
    // s = pthread_attr_getdetachstate(attr, &i);
    // if (s != 0)
    //     handle_error_en(s, "pthread_attr_getdetachstate");
    // printf("%sDetach state        = %s\n", prefix,
    //         (i == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
    //         (i == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" :
    //         "???");

    // s = pthread_attr_getscope(attr, &i);
    // if (s != 0)
    //     handle_error_en(s, "pthread_attr_getscope");
    // printf("%sScope               = %s\n", prefix,
    //         (i == PTHREAD_SCOPE_SYSTEM)  ? "PTHREAD_SCOPE_SYSTEM" :
    //         (i == PTHREAD_SCOPE_PROCESS) ? "PTHREAD_SCOPE_PROCESS" :
    //         "???");

    cpu_set_t cpu_set;

    // CPU_ZERO(&cpu_set);

    // CPU_SET(0,&cpu_set);

    // int a = CPU_COUNT(&cpu_set);

    // printf("%sThread affinity = %d\n", prefix, CPU_COUNT(&cpu_set));

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

    // pthread_attr_getaffinity_np()

    // s = pthread_attr_destroy(&attr);
    // if (s != 0) handle_error_en(s, "pthread_attr_destroy");
}
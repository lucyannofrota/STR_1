#include "pthread_info.h"

#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

void display_pthread_attr(pthread_attr_t *attr, char *prefix){
    // https://man7.org/linux/man-pages/man3/pthread_attr_init.3.html
    // https://man7.org/linux/man-pages/man3/pthread_attr_destroy.3.html
    int s, i;
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

    s = pthread_attr_getinheritsched(attr, &i);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getinheritsched");
    printf("%sInherit scheduler   = %s\n", prefix,
            (i == PTHREAD_INHERIT_SCHED)  ? "PTHREAD_INHERIT_SCHED" :
            (i == PTHREAD_EXPLICIT_SCHED) ? "PTHREAD_EXPLICIT_SCHED" :
            "???");

    s = pthread_attr_getschedpolicy(attr, &i);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getschedpolicy");
    printf("%sScheduling policy   = %s\n", prefix,
            (i == SCHED_OTHER) ? "SCHED_OTHER" :
            (i == SCHED_FIFO)  ? "SCHED_FIFO" :
            (i == SCHED_RR)    ? "SCHED_RR" :
            "???");

    s = pthread_attr_getschedparam(attr, &sp);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getschedparam");
    printf("%sScheduling priority = %d\n", prefix, sp.sched_priority);
}

void display_thread_attributes(pthread_t thread, char *prefix){
    // https://man7.org/linux/man-pages/man3/pthread_getattr_np.3.html
    int s;
    pthread_attr_t attr;

    s = pthread_getattr_np(thread, &attr);
    if (s != 0)
        handle_error_en(s, "pthread_getattr_np");

    display_pthread_attr(&attr, prefix);

    s = pthread_attr_destroy(&attr);
    if (s != 0)
        handle_error_en(s, "pthread_attr_destroy");
}
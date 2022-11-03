#ifndef PTHREAD_INFO_H__
#define PTHREAD_INFO_H__

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

void thread_configs(pthread_attr_t *attr, int setaffinity, int sched_type,int priority_mod);

void change_thread_priority(pthread_t thread,int priority);

void display_thread_attr(pthread_t thread, char *prefix);

void display_attr(pthread_attr_t attr, char *prefix);

// https://man7.org/linux/man-pages/man3/pthread_attr_init.3.html
// https://man7.org/linux/man-pages/man3/pthread_attr_destroy.3.html
// https://man7.org/linux/man-pages/man3/pthread_getattr_np.3.html


#endif // PTHREAD_INFO_H__
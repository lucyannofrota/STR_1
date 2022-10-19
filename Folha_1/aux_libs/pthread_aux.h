#ifndef PTHREAD_INFO_H__
#define PTHREAD_INFO_H__

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

// void set_pthread_affinity(pthread_t thread, cpu_set_t *cpu_set);

// void set_pthread_sched_attr(pthread_t thread, pthread_attr_t * attr);

void thread_configs(pthread_attr_t *attr,int priority_mod);

void change_thread_priority(pthread_attr_t *attr,int priority_mod);

void set_thread_configs(pthread_t thread, cpu_set_t *cpu_set,pthread_attr_t *attr);

//  void display_pthread_affinity(pthread_t thread, char *prefix);

void display_thread_attr(pthread_t thread, char *prefix);
// https://man7.org/linux/man-pages/man3/pthread_attr_init.3.html
// https://man7.org/linux/man-pages/man3/pthread_attr_destroy.3.html
// https://man7.org/linux/man-pages/man3/pthread_getattr_np.3.html

void display_attr(pthread_attr_t attr, char *prefix);

#endif // PTHREAD_INFO_H__
#ifndef PTHREAD_INFO_H__
#define PTHREAD_INFO_H__

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define handle_error_en(en, msg) \
    do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

void display_pthread_affinity(pthread_t thread, char *prefix);

void display_thread_sched_attr(pthread_t thread, char *prefix);
// https://man7.org/linux/man-pages/man3/pthread_attr_init.3.html
// https://man7.org/linux/man-pages/man3/pthread_attr_destroy.3.html
// https://man7.org/linux/man-pages/man3/pthread_getattr_np.3.html

#endif // PTHREAD_INFO_H__
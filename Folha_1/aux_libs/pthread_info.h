#ifndef PTHREAD_INFO_H__
#define PTHREAD_INFO_H__

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

void display_pthread_attr(pthread_attr_t *attr, char *prefix);
// https://man7.org/linux/man-pages/man3/pthread_attr_init.3.html
// https://man7.org/linux/man-pages/man3/pthread_attr_destroy.3.html

void display_thread_attributes(pthread_t thread, char *prefix);
// https://man7.org/linux/man-pages/man3/pthread_getattr_np.3.html

#endif // PTHREAD_INFO_H__
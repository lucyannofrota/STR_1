#ifndef AUX_FUNCTIONS_H__
#define AUX_FUNCTIONS_H__

#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>

// #include <stdlib.h>
#include <pthread.h>
// #include <unistd.h>
// #include <sched.h>
// #include <sys/resource.h>

#define CLASS 2
#define GROUP 4
// #include <sys/resource.h>

// From https://man7.org/linux/man-pages/man2/clock_gettime.2.html
// struct timespec {
//     time_t   tv_sec;        /* seconds */
//     long     tv_nsec;       /* nanoseconds */
// };

void add_timespec(const struct timespec *tim_1,const struct timespec *tim_2,struct timespec *result);

void sub_timespec(struct timespec *tim_1,struct timespec *tim_2,struct timespec *result);

double dtime_ms(const struct timespec *tim_1,const struct timespec *tim_2);

void clk_wait(double m_sec);

void calc_func_ripple(void (*func)(int,int), int n_samples, struct timespec dtime_spec[n_samples]);

void calc_func_ripple_alt(void (*func)(int,int), int time_us,int faction, int n_samples, struct timespec dtime_spec[n_samples]);

void sim_func(int time_us,int faction);

double timespec_to_double_ms(struct timespec *time);

void print_table(struct timespec *tab,int M, int N, char *prefix);

void print_timespec(struct timespec t,char *prefix);

void report_times(int M, int N, int *r_samp, double sched_tab[][N], double time_tab[][N], int interval[2], char *prefix);

#endif // AUX_FUNCTIONS_H__
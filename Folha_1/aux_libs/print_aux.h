#ifndef PRINT_FUNCTIONS_H__
#define PRINT_FUNCTIONS_H__

#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>

double timespec_to_double_ms(struct timespec *time);

void print_table(struct timespec *tab,int M, int N, char *prefix);

void print_timespec(struct timespec t,char *prefix);

void print_table_double(int M, int N, double tab[][N], char *prefix);

void report_times(int M, int N, double sched_tab[][N], double time_tab[][N], char *prefix);

#endif // PRINT_FUNCTIONS_H__
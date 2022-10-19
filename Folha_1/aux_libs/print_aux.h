#ifndef PRINT_FUNCTIONS_H__
#define PRINT_FUNCTIONS_H__

#include <stdio.h>
#include <sys/time.h>

double timespec_to_double_ms(struct timespec *time);

void print_table(struct timespec *tab,int M, int N, char *prefix);

void print_timespec(struct timespec t,char *prefix);

void print_table_double(double **tab,int M, int N, char *prefix);

#endif // PRINT_FUNCTIONS_H__
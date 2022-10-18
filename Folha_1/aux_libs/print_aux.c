#include "print_aux.h"

double timespec_to_double_ms(struct timespec time){
    return ((time.tv_sec*1000000000+time.tv_nsec)/1000000000.0)*1000; // ms
}

void print_table(struct timespec *tab,int M, int N, char *prefix){
    double max_values[M];

    // printf("Val: %3.5f\n",timespec_to_double_ms(*( tab + (0*N + 0)*sizeof(struct timespec) )));

    // Max values initialization
    int i, j;

    for(i = 0; i < M; i ++){
        max_values[i] = 0;
    }

    // Print time table
    printf("%sTime table (%i samples) [ms]:\n",prefix,N);
    for(i = 0; i < N; i++){
        printf("%s\t",prefix);
        for(j = 0; j < M; j++){
            // *((arr+i*n) + j))
            // printf("Val: %3.5f\n",timespec_to_double_ms( *( tab + (j*N + i)*sizeof(struct timespec) ) ));
            if(max_values[j] < timespec_to_double_ms(*( tab + (j*N + i) ))){
                max_values[j] = timespec_to_double_ms(*( tab + (j*N + i) ));
            }
            printf("| f%i = %8.2f ",j+1,timespec_to_double_ms(*( tab + (j*N + i) )));
        }
        printf("|\n");
    }
    // Print max values
    printf("%sMax Times (%i samples) [ms]:\n",prefix,N);
    printf("%s\t",prefix);
    for(j = 0; j < M; j++){
        printf("| f%i = %8.2f ",j+1,max_values[j]);
    }
    printf("|\n\n\n");
}

void print_timespec(struct timespec t,char *prefix){
    printf("%s sec: %i\n%snsec: %ld\n",prefix,(int)t.tv_sec,prefix,t.tv_nsec);
}
#include "aux_functions.h"

void clk_wait(double m_sec){
    // https://stackoverflow.com/questions/20332382/linux-sleeping-with-clock-nanosleep

    struct timespec deadline;
    clock_gettime(CLOCK_REALTIME, &deadline);

    // Add the time you want to sleep
    deadline.tv_nsec += (long) ceil(m_sec*1000000000/1000.0);

    // Normalize the time to account for the second boundary
    if(deadline.tv_nsec >= 1000000000) {
        deadline.tv_nsec -= 1000000000;
        deadline.tv_sec++;
    }
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &deadline, NULL);
}

void add_timespec(const struct timespec *tim_1,const struct timespec *tim_2,struct timespec *result){
    // from <sys/time.h>
    // define timeradd(a, b, result)
    result->tv_sec = tim_1->tv_sec + tim_2->tv_sec;
    result->tv_nsec = tim_1->tv_nsec + tim_2->tv_nsec;
    if(result->tv_nsec >= 1e9){
        result->tv_sec++;
        result->tv_nsec -= 1e9;
    }
}

void sub_timespec(struct timespec *tim_1,struct timespec *tim_2,struct timespec *result){
    result->tv_sec = tim_1->tv_sec - tim_2->tv_sec;
    result->tv_nsec = tim_1->tv_nsec - tim_2->tv_nsec;
    if(result->tv_nsec < 0){
        result->tv_sec -= 1;
        result->tv_nsec += 1000000000;
    }
}

double dtime_ms(const struct timespec *tim_1,const struct timespec *tim_2){
    // from <sys/time.h>
    // # define timersub(a, b, result)
    struct timespec result;
    result.tv_sec = tim_1->tv_sec - tim_2->tv_sec;
    result.tv_nsec = tim_1->tv_nsec - tim_2->tv_nsec;
    if(result.tv_nsec < 0 && result.tv_sec > 0){
        result.tv_sec --;
        result.tv_nsec += 1e9;
    }
    return (result.tv_sec*1e9 + ((long int)result.tv_nsec))/1000000.0;
}

void calc_func_ripple(void (*func)(int,int), int n_samples, struct timespec dtime_spec[n_samples]){
    int i;
    struct timespec time1 = {0,0}, time2 = {0,0};

    for(i = 0; i < n_samples; i++){
        clock_gettime(CLOCK_REALTIME, &time1);
        func(CLASS,GROUP);
        clock_gettime(CLOCK_REALTIME, &time2);
        sub_timespec(&time2,&time1,&dtime_spec[i]);
        clk_wait(5); // 5 ms
    }
}

void calc_func_ripple_alt(void (*func)(int,int), int time_us,int faction, int n_samples, struct timespec dtime_spec[n_samples]){
    int i;
    struct timespec time1 = {0,0}, time2 = {0,0};

    for(i = 0; i < n_samples; i++){
        clock_gettime(CLOCK_REALTIME, &time1);
        func(time_us,faction);
        clock_gettime(CLOCK_REALTIME, &time2);
        sub_timespec(&time2,&time1,&dtime_spec[i]);
        clk_wait(5); // 5 ms
    }
}

void sim_func(int time_us,int faction){
    struct timespec period = {0,time_us*1e3}, time;
    for(int i = 0; i < time_us/faction; i++){
        clock_gettime(CLOCK_REALTIME, &time);
        add_timespec(&time,&period,&time);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &period, NULL);
    }
}

double timespec_to_double_ms(struct timespec *time){
    return ((time->tv_sec*1000000000+time->tv_nsec)/1000000000.0)*1000; // ms
}

void print_table(struct timespec *tab,int M, int N, char *prefix){
    double max_values[M];
    double mean_values[M];


    // Max values initialization
    int i, j;

    for(i = 0; i < M; i ++){
        max_values[i] = 0;
        mean_values[i] = 0;
    }

    // Print time table
    printf("%sTime table (%i samples) [ms]:\n",prefix,N);
    for(i = 0; i < N; i++){
        printf("%s\t",prefix);
        for(j = 0; j < M; j++){
            // *((arr+i*n) + j))
            // printf("Val: %3.5f\n",timespec_to_double_ms( *( tab + (j*N + i)*sizeof(struct timespec) ) ));
            mean_values[j] += timespec_to_double_ms(( tab + (j*N + i) ));
            if(max_values[j] < timespec_to_double_ms(( tab + (j*N + i) ))){
                max_values[j] = timespec_to_double_ms(( tab + (j*N + i) ));
            }
            printf("| f%i = %9.3f ",j+1,timespec_to_double_ms(( tab + (j*N + i) )));
        }
        printf("|\n");
    }
    // Print max values
    printf("%sMax Times (%i samples) [ms]:\n",prefix,N);
    printf("%s\t",prefix);
    for(j = 0; j < M; j++){
        printf("| f%i = %9.3f ",j+1,max_values[j]);
    }
    printf("|\n");
    printf("%sMean Times (%i samples) [ms]:\n",prefix,N);
    printf("%s\t",prefix);
    for(j = 0; j < M; j++){
        printf("| f%i = %9.3f ",j+1,mean_values[j]/N);
    }
    printf("|\n\n\n");
}

void print_timespec(struct timespec t,char *prefix){
    printf("%s sec: %i\n%snsec: %ld\n",prefix,(int)t.tv_sec,prefix,t.tv_nsec);
}

void centerText(char *buff, char *text, int fieldWidth) {
    int padlen = (fieldWidth - strlen(text)) / 2;
    snprintf(buff,fieldWidth,"%*s%s%*s\n", padlen, "", text, padlen, "");
} 

void report_times(int M, int N, int *r_samp, double sched_tab[][N], double time_tab[][(N-1)*2], int interval[2], char *prefix){

    int i,j;

    float RT[M][N-1], RTJ[M];

    int idx[M][N-1], idx_counts[M];

    if(interval != NULL){
        for(i = 0; i < M; i++){
            idx_counts[i] = 0;
        }
    }


    const int width = 25*5;

    char sep[width+1];

    for(i = 0; i < width; i++){
        sep[i] = '-';
    } 
    sep[width-1] = '\0';

    // Print h separator
    printf("%s%s\n",prefix,sep);
    // Print Title
    char title_text[width-2];
    snprintf(title_text,width-2,"Time table (%2i samples) [ms]",N-1);
    char c_text[width-2];
    centerText(c_text,title_text,width-2);
    printf("%s|%s|\n",prefix,c_text);
    // Print h separator
    printf("%s%s\n",prefix,sep);
    // Header 1
    const int base = ceil((width-2-3-2-2)/4.0);

    int l_size = base, l2_size = base, r2_size = base, r_size = (width-2-3-2-2)-l_size-l2_size-r2_size;
    char l[l_size], l2[l2_size], r2[r2_size], r[r_size];
    centerText(l,"Scheduled Time (ST)",l_size+1);
    centerText(l2,"Activation Time (AT)",l2_size);
    // centerText(c,"Exit Time (ET)",c_size);
    centerText(r2,"Exit Time (ET)",r2_size-1);
    centerText(r,"Response Time (RT)",r_size);
    printf("%s|fcn|%s||%s||%s||%s|\n",prefix,l,l2,r2,r);
    // Header 2
    centerText(l," ",l_size+1);
    centerText(l2," ",l2_size);
    // centerText(c," ",c_size);
    centerText(r2," ",r2_size-1);
    centerText(r,"RT = ET - AT",r_size);
    printf("%s|   |%s||%s||%s||%s|\n",prefix,l,l2,r2,r);
    // Core
    l_size-= 0; l2_size-=1; r2_size-=2; r_size-=1;
    for(i = 0; i < M; i++){
        // Print h separator
        printf("%s%s\n",prefix,sep);
        // printf("R_samp: %i\n",r_samp[i]);
        for(j = 0; j < r_samp[i]/*N-1*/; j++){
            if(interval != NULL){
                if( interval[0] <= sched_tab[i][j+1] && interval[1] >= sched_tab[i][j+1]){
                    idx[i][idx_counts[i]] = j;
                    idx_counts[i]++;
                }
            }
            RT[i][j] = time_tab[i][j*2+1]-time_tab[i][j*2];
            printf("%s|#f%i|%*.3f||%*.3f||%*.3f||%*.3f|\n",prefix,i+1,
                l_size,sched_tab[i][j+1],
                l2_size,time_tab[i][j*2],
                r2_size,time_tab[i][j*2+1],
                r_size,RT[i][j]
            );
        }
    }
    // Footer
    printf("%s%s\n\n\n",prefix,sep); 


    // Max Values
    float min[M], max[M];


    // printf("M: %i\n",M);
    for(i = 0; i < M; i++){
        min[i] = 1e9; max[i] = -1e9;
        for(j = 0; j < r_samp[i]/*N-1*/; j++){
            if(min[i] > RT[i][j]) min[i] = RT[i][j];
            if(max[i] < RT[i][j]) max[i] = RT[i][j];       
        }
        RTJ[i] = max[i] - min[i];
    }

    const int space = 50;


    printf("%sAll Samples:\n",prefix);
    printf("%s\t%*s\n\n",prefix,space+9+2,"[      Min,      Max] (ms)");
    for(i = 0; i < M; i++){
        printf("%s\tf%i:\n",prefix,i+1);
        printf("%s\t\tResponse Time (RT):%*s",prefix,space-19-21," ");
        printf("[%9.4f,%9.4f]\n",min[i],max[i]);
        printf("%s\t\tResponse Time Jitter (RTJ): %*s%9.4f\n",prefix,space-9-28," ",RTJ[i]);
        printf("\n");
    }

    if(interval != NULL){

        float min_i[M], max_i[M], RTJ_i[M];

        for(i = 0; i < M; i ++){
            min_i[i] = 1e9; max_i[i] = -1e9;
            for(j = 0; j < idx_counts[i]; j++){
                if(min_i[i] > RT[i][idx[i][j]]) min_i[i] = RT[i][idx[i][j]];
                if(max_i[i] < RT[i][idx[i][j]]) max_i[i] = RT[i][idx[i][j]];
            }
            RTJ_i[i] = max_i[i] - min_i[i];
        }

        printf("%sInterval [%i,%i] Samples:\n",prefix,interval[0],interval[1]);
        printf("%s\t%*s\n\n",prefix,space+9+2,"[      Min,      Max] (ms)");
        for(i = 0; i < M; i++){
            printf("%s\tf%i:\n",prefix,i+1);
            printf("%s\t\tResponse Time (RT):%*s",prefix,space-19-21," ");
            printf("[%9.4f,%9.4f]\n",min_i[i],max_i[i]);
            printf("%s\t\tResponse Time Jitter (RTJ): %*s%9.4f\n",prefix,space-9-28," ",RTJ_i[i]);
            printf("\n");
        }

    }
}
#include "print_aux.h"

double timespec_to_double_ms(struct timespec *time){
    return ((time->tv_sec*1000000000+time->tv_nsec)/1000000000.0)*1000; // ms
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
    printf("|\n\n\n");
}

void print_timespec(struct timespec t,char *prefix){
    printf("%s sec: %i\n%snsec: %ld\n",prefix,(int)t.tv_sec,prefix,t.tv_nsec);
}

void print_table_double(int M, int N, double tab[][N], char *prefix){
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
            if(i == 0) printf("| f%i = %9.3f ",j+1,tab[j][i]);
            else printf("| f%i%s= %9.3f ",j+1,i % 2 == 0 ? "+" : "-",tab[j][i]);
        }
        printf("|\n");
    }
    // Print max values
    printf("%sMax Times (%i samples) [ms]:\n",prefix,N);
    printf("%s\t",prefix);
    for(j = 0; j < M; j++){
        printf("| f%i = %9.3f ",j+1,max_values[j]);
    }
    printf("|\n\n\n");
}

void centerText(char *buff, char *text, int fieldWidth) {
    int padlen = (fieldWidth - strlen(text)) / 2;
    snprintf(buff,fieldWidth,"%*s%s%*s\n", padlen, "", text, padlen, "");
} 

void report_times(int M, int N, double sched_tab[][N], double time_tab[][(N-1)*2], char *prefix){

    const int width = 60;

    int i,j;

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
    // Header
    int l_size = ceil((width-2-3-2)/3.0), r_size = ceil((width-2-3-2)/3.0), c_size = width-2-3-2-l_size-r_size;
    char l[l_size], r[r_size], c[c_size];
    centerText(l,"Sched Time",l_size-1);
    centerText(c,"Start Time(ST)",c_size);
    centerText(r,"End Time(ET)",r_size);
    printf("%s|fun|%s||%s||%s|\n",prefix,l,c,r);
    l_size-=2; c_size-=1; r_size-=1;
    for(i = 0; i < M; i++){
        // Print h separator
        printf("%s%s\n",prefix,sep);
        for(j = 0; j < N-1; j++){
            printf("%s|#f%i|%*.3f||%*.3f||%*.3f|\n",prefix,i+1,l_size,sched_tab[i][j+1],c_size,time_tab[i][j*2],r_size,time_tab[i][j*2+1]);
        }
    }
    printf("%s%s\n",prefix,sep);
}
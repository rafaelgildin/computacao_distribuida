/*
https://www.includehelp.com/c-programming-questions/compiling-program-with-pthread-library-linux.aspx
gcc calc_pi_somaserie.c -pthread -o calc_pi_somaserie && ./calc_pi_somaserie



*/  
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


double sum = 0;  /* sequence sum */
int n;  /* size of sequence */
unsigned int nthr; /* number of threads */

void *Thread_sum (void *rank); 


int main (int argc, char *argv[]){
    
    int i;
    double pi = 0; 
    
    pthread_t *tid;  /*  ID das threads */
    
    if (argc < 3) {
        printf("\nDigite o número de threads: ");
        scanf("%d",&nthr);
        printf("\nDigite o tamanho da sequencia: ");
        scanf("%d", &n);
    } else {
        nthr = atoi(argv[1]);
        n = atoi(argv[2]);
    }
    
    
    printf("n: %g, nthr: %d\n", (double) n, nthr);
    
    
    tid = calloc(nthr, sizeof(pthread_t));
    
    
    /* criando threads */
    for (i = 0; i< nthr ; i++){
        /*  http://stackoverflow.com/a/19233025 */
        int *j = malloc(sizeof(int));
        *j = i;
        /* cria a i-esima thread */
        pthread_create (&tid[i], NULL, Thread_sum, (void *)j);
    } 
    
    /* espera cada thread filha terminar */
    for (i = 0; i< nthr ; i++){
        pthread_join (tid[i], NULL);
    } 
    
    
    
    /* Calculate pi value */
    pi = 4.0*sum;
    
    printf("Valor de pi:\%1.10f\n", pi);
    
    return 0;
}





void *Thread_sum (void *rank) {

    unsigned int my_rank = *((unsigned int *)rank); 
    double factor;
    long long i;
    long long my_n = n/nthr;
    long long my_first_i = my_n * my_rank;
    long long my_last_i = my_first_i + my_n;
    
    if (my_first_i % 2 == 0) /* my_first_i is even */
        factor = 1.0;
    else /* my_first_i is odd */
        factor = -1.0;
    
    for (i = my_first_i; i < my_last_i; i++, factor = -factor){
        sum += factor/(2*i + 1);
    }
    
    return NULL;
    /* Thread_sum */
}
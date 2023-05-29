/*
p=calc_pi_somaserie_mpi && \
mpicc -g -Wall -o $p $p.c && \
mpiexec -np 4 $p
*/  
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double Calculate_local_Pi_sum (
    int my_n,
    int my_rank,
    int my_first_i,
    int my_last_i
);

int main (int argc, char **argv){
    int n=1000;  /* size of sequence */

    double pi_local, pi_total = 0;
    int num_procs, my_id, source;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    printf("Hello world! I'm process %i out of %i processes\n", my_id, num_procs);

    int my_n = n/num_procs;
    int my_first_i = my_n * my_id;
    int my_last_i = my_first_i + my_n;

    pi_local = Calculate_local_Pi_sum(
        my_n,
        my_id,
        my_first_i,
        my_last_i
    );

    if (my_id != 0){
        MPI_Send(&pi_local, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        pi_total = pi_local;
        for(source = 1; source < num_procs; source++){
            MPI_Recv(&pi_local, 1, MPI_DOUBLE, source, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            pi_total += pi_local;
        }
    }

    if (my_id == 0){
        pi_total = 4.0*pi_total;
        printf("Com %d prcs, Valor de pi=\%1.10f\n", num_procs, pi_total);
    }

    MPI_Finalize();
    return 0;
}

double Calculate_local_Pi_sum (
    int my_n,
    int my_rank,
    int my_first_i,
    int my_last_i
) {
    int i;
    double factor,sum;
    if (my_first_i % 2 == 0) /* my_first_i is even */
        factor = 1.0;
    else /* my_first_i is odd */
        factor = -1.0;
    for (i = my_first_i; i < my_last_i; i++, factor = -factor){
        sum += factor/(2*i + 1);
    }
    return sum;
}
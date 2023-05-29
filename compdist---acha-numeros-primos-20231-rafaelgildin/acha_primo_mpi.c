/*
p=acha_primo_mpi && \
mpicc -g -Wall -o $p $p.c -lm && \
mpiexec -np 3 $p 0 1000
*/  

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int ehPrimo(int numero){
    if (numero <= 1) {
        return 0;
    }
    for(int fator = 2; fator <= sqrt(numero); fator++){
        if (numero % fator == 0)
            return 0;
    }
    return 1;
}

int main(int argc, char** argv) {
    int rank, q_processos, inicio, fim, i;
    int contador_local = 0, contador=0;
    int* primos_locais = NULL;
    int* primos = NULL;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &q_processos);

    // Erro na chamada
    if (argc != 3) {
        if (rank == 0) {
            printf("Chamada: %s <inicio> <fim>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    inicio = atoi(argv[1]);
    fim = atoi(argv[2]);

    // printf("\nprocesso = %d\n", rank);

    if (rank == 0) {
        printf("Encontrando primos entre %d e %d\n", inicio, fim);
    }

    // Calcular o delta local
    int delta = fim - inicio + 1;
    int delta_local = delta / q_processos;
    int resto = delta % q_processos;
    int inicio_local = inicio + rank * delta_local;
    int fim_local = inicio_local + delta_local - 1;

    // Ajustar para o resto
    if (rank < resto) {
        delta_local++;
        inicio_local += rank;
        fim_local = inicio_local + delta_local - 1;
    } else {
        inicio_local += resto;
        fim_local += resto;
    }

    // Alocar memoria para os primos locais
    primos_locais = (int*) calloc(delta_local,sizeof(int));
    if (primos_locais == NULL) {
        printf("Falha na alocacao de memoria\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Encontrar os primos no delta local
    contador_local = 0;
    for (i = inicio_local; i <= fim_local; i++) {
        if (ehPrimo(i)) {
            primos_locais[contador_local++] = i;
        }
    }

    // Juntar todos os primos no processo 0
    if (rank == 0) {
        primos = (int*) calloc(delta,sizeof(int));
        if (primos == NULL) {
            printf("Falha na alocacao de memoria\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    MPI_Gather(primos_locais, delta_local, MPI_INT, primos, delta_local, MPI_INT, 0, MPI_COMM_WORLD);

    // Printar os primos do processo 0
    if (rank == 0) {
        for (i = 0; i < delta; i++) {
            if (primos[i] != 0) {
                printf("%d ", primos[i]);
                contador++;
            }
        }
        printf("\nTotal de primos = %d\n", contador);
        free(primos);
    }

    free(primos_locais);
    MPI_Finalize();
    return 0;
}

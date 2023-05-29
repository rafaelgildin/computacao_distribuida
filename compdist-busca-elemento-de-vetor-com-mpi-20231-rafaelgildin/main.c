/*
Objetivo: Buscar elemento em vetor com MPI

p=main && \
mpicc -g -Wall -o $p $p.c && \
mpiexec -np 2 $p
*/  
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int get_n_lines(char *filename){
    /*
    Pegar o numero de linhas em arquivo csv
    */

    int dado=0,qlinhas=0;
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        return 0;
    }

    while (fscanf(fp, "%d",&dado)== 1){
        qlinhas++;
    }
    fclose(fp);
    // printf("qlinhas = %d\n", qlinhas);
    return qlinhas;
}

void get_data(char *filename, int qlinhas, int *v){
    /*
    Pegar os dados em arquivo csv,
    considerando 1 coluna.
    */
    int i=0;
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }
    while (fscanf(fp, "%d",&v[i])== 1){
        i++;
    }
    fclose(fp);
}

int main(int argc, char** argv) {
    char *filename = "vetor1.csv";
    int num=8332200;

    // ----------- armazena os dados do csv no vetor v
    int qlinhas, *v;
    qlinhas = get_n_lines(filename);
    v = (int*)malloc(qlinhas * sizeof(int));
    if (v == NULL) {
        printf("Erro na alocacao do vetor.\n");
        return 0;
    }
    get_data(filename,qlinhas,v);

    // ----------- busca numero em vetor
    int ranking, qprocessos, encontrado=0;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &ranking);
    MPI_Comm_size(MPI_COMM_WORLD, &qprocessos);
    
    int inicio = ranking * qlinhas / qprocessos;
    int fim = (ranking + 1) * qlinhas / qprocessos;
    int i;

    for (i = inicio; i < fim; i++) {
        if (v[i] == num) {
            encontrado = 1;
            break;
        }
    }
    
    int resultado;
    MPI_Reduce(&encontrado, &resultado, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if (ranking == 0) {
        if (resultado == 1) {
            printf("%d foi encontrado no vetor\n", num);
        } else {
            printf("%d nao foi encontrado no vetor\n", num);
        }
    }
    
    MPI_Finalize();
    return 0;
}
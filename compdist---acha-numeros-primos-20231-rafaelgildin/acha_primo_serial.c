/*
p=acha_primo_serial && \
gcc -Wall $p.c -lm -o $p && \
./$p
*/  

#include <stdio.h>
#include <math.h>
#include <time.h>

int primo(int numero){
    int raiz, fator;
    raiz = (int) sqrt((double) numero);

    for(fator = 2; fator <= raiz; fator++){
        if (numero % fator == 0)    
            return 0;
    }
    return 1;
}

void achar_primos(int j, int k){
    if(j>=k){
        printf("\nk deve ser maior que j\n");
        return;
    }

    int quantidade=0,numero,p;
    
    printf("primos: ");
    for(numero = j; numero < k; numero ++) {  
        p = primo(numero);
        quantidade += p;
        if(p){
            printf("%d ",numero);
        }
    }    
    printf("\nTotal de numeros primos entre %d e %d: %d\n", j, k, quantidade);
}

int main(){
    int j=10, k=20;
    achar_primos(j,k);
    return 0;   
}

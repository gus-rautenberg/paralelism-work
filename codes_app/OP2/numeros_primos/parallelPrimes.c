#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <openacc.h>

#define MAX 300000000

// pgcc -acc -Minfo=accel -o main parallelPrimes.c -lm
// pgg: o compilador da nvidia para C, tanto pra cuda como acc.
// -acc Habilita a aceleração com OpenACC.
// -Minfo=accel mostra quais partes do código foram paralelizadas e quais otimizacoes foram usadas
// LUIZ, openACC basicamente é um openmp só que usa a GPU.

int main() {
    char *primes = (char *) malloc(MAX * sizeof(char));
    if (primes == NULL) {
        fprintf(stderr, "Erro na alocação de memória\n");
        return 1;
    }
    double start = acc_get_wtime(); 

  /* Create an array of values, where '1' indicates that a number is prime.
   * Start by assuming all numbers are prime by setting them to 1.
   */
    #pragma acc parallel loop copyout(primes[0:MAX])
    for (int i = 0; i < MAX; i++) {
        primes[i] = 1;
    }

    /* Loop through a portion of the array (up to the square root of MAX). If
   * it's a prime, ensure all multiples of it are set to zero (false), as they
   * clearly cannot be prime.
   */
    int limit = sqrt(MAX) + 1;

    #pragma acc data copy(primes[0:MAX])
    {
        #pragma acc parallel loop
        for (int i = 2; i < limit; i++) {
            if (primes[i-1]) { 
                #pragma acc loop
                for (int j = i*i; j <= MAX; j += i) {
                    primes[j-1] = 0;
                }
            }
        }
    }

  /* Output the results */
    int count = 0;
    #pragma acc parallel loop reduction(+:count) copyin(primes[0:MAX])
    for (int i = 2; i <= MAX; i++) {
        if (primes[i-1]) {
            // printf("%d\n", i);

            count++;
        }
    }
    double end = acc_get_wtime(); 
    double executionTime = (end - start) * 1000.0; 
    printf("There were %d primes up to %d\n", count, MAX);
    printf("Parallel execution time: %.2f ms\n", executionTime);

    free(primes);
    return 0;
}

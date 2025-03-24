#include <stdio.h>
#include <math.h>
#include <time.h>

#define MAX 1000000

int main() {
    // Registra o tempo de início
    clock_t start = clock();

    /* Create an array of values, where '1' indicates that a number is prime.
     * Start by assuming all numbers are prime by setting them to 1.
     */
    char primes[MAX];
    for (int i = 0; i < MAX; i++) {
        primes[i] = 1;
    }

    /* Loop through a portion of the array (up to the square root of MAX). If
     * it's a prime, ensure all multiples of it are set to zero (false), as they
     * clearly cannot be prime.
     */
    int limit = sqrt(MAX) + 1;
    for (int i = 2; i < limit; i++) {
        if (primes[i-1]) {
            for (int j = i * i; j <= MAX; j += i) {
                primes[j - 1] = 0;
            }
        }
    }

    /* Output the results */
    int count = 0;
    for (int i = 2; i <= MAX; i++) {
        if (primes[i - 1]) {
            count++;
        }
    }
    printf("There were %d primes up to %d\n", count, MAX);

    // Registra o tempo de fim
    clock_t end = clock();

    // Calcula o tempo de execução
    double executionTime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0; // tempo em milissegundos
    printf("Execution time: %.5f ms\n", executionTime);

    return 0;
}

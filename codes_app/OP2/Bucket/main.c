#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <string.h>

#define MAX 10         // Número de buckets
#define REPETICOES 10  // Número de execuções para média

// Função para encontrar o valor máximo em um array
float findMax(float array[], int n) {
    float max = array[0];
    for (int i = 1; i < n; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

// Função para inserir um elemento no bucket (insertion sort)
void insertionSort(float bucket[], int n) {
    for (int i = 1; i < n; i++) {
        float key = bucket[i];
        int j = i - 1;
        while (j >= 0 && bucket[j] > key) {
            bucket[j + 1] = bucket[j];
            j--;
        }
        bucket[j + 1] = key;
    }
}

// Função para realizar o bucket sort
void bucketSort(float array[], int n) {
    float max = findMax(array, n);
    
    int bucketCount = MAX;
    float buckets[bucketCount][n];
    int bucketSizes[bucketCount];

    // Inicializa os tamanhos dos buckets
    for (int i = 0; i < bucketCount; i++) {
        bucketSizes[i] = 0;
    }

    // Distribuir os elementos nos buckets
    for (int i = 0; i < n; i++) {
        int index = (array[i] * bucketCount) / (max + 1);
        buckets[index][bucketSizes[index]] = array[i];
        bucketSizes[index]++;
    }

    // Ordenar cada bucket individualmente
    for (int i = 0; i < bucketCount; i++) {
        if (bucketSizes[i] > 0) {
            insertionSort(buckets[i], bucketSizes[i]);
        }
    }

    // Concatenar os buckets ordenados de volta no array original
    int index = 0;
    for (int i = 0; i < bucketCount; i++) {
        for (int j = 0; j < bucketSizes[i]; j++) {
            array[index++] = buckets[i][j];
        }
    }
}

// Função para imprimir o array (opcional)
void printArray(float array[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%.2f ", array[i]);
    }
    printf("\n");
}

// Função para ler os dados de um arquivo e retornar o número de elementos lidos
int readFile(const char *filename, float **array) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s!\n", filename);
        return -1;
    }

    int n = 0;
    while (fscanf(file, "%f", &(*array)[n]) == 1) {
        n++;
        *array = (float*)realloc(*array, sizeof(float) * (n + 1));
    }
    fclose(file);
    return n;
}

int main() {
    DIR *dir;
    struct dirent *entry;
    const char *folder = "data";

    // Abrir o diretório "data"
    if ((dir = opendir(folder)) == NULL) {
        perror("Erro ao abrir a pasta 'data'");
        return 1;
    }

    // Percorrer cada arquivo na pasta
    while ((entry = readdir(dir)) != NULL) {
        // Ignorar entradas especiais "." e ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Monta o caminho completo do arquivo na pasta "data"
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "%s/%s", folder, entry->d_name);

        // Lê o arquivo e armazena os dados em um array
        float *original = (float*)malloc(sizeof(float));
        if (original == NULL) {
            perror("Erro na alocação de memória");
            continue;
        }
        int n = readFile(filepath, &original);
        if (n == -1) {
            free(original);
            continue;
        }

        double soma_tempo = 0.0;
        // Executa o bucket sort REPETICOES vezes
        for (int rep = 0; rep < REPETICOES; rep++) {
            // Cria uma cópia do array original para cada execução
            float *array = (float*)malloc(sizeof(float) * n);
            if (array == NULL) {
                perror("Erro na alocação de memória para o array de execução");
                break;
            }
            for (int i = 0; i < n; i++) {
                array[i] = original[i];
            }

            clock_t start = clock();
            bucketSort(array, n);
            clock_t end = clock();
            soma_tempo += (double)(end - start) / CLOCKS_PER_SEC;

            free(array);
        }
        double media_tempo = soma_tempo / REPETICOES;

        // Cria o arquivo de saída com o tempo médio de execução fora da pasta "data"
        char outpath[256];
        snprintf(outpath, sizeof(outpath), "%s_tempo.txt", entry->d_name);
        FILE *outfile = fopen(outpath, "w");
        if (outfile == NULL) {
            printf("Erro ao criar o arquivo %s\n", outpath);
        } else {
            fprintf(outfile, "Tempo médio de execução do bucket sort (em %d execuções): %f segundos\n", REPETICOES, media_tempo);
            fclose(outfile);
        }

        free(original);
    }

    closedir(dir);
    return 0;
}

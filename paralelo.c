#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <direct.h>

typedef struct
{
    int **M1;
    int **M2;
    int n1, m1, n2, m2;
    int inicio, fim;
    int thread_id;
    int tamanho; // lado da matriz quadrada
} ThreadData;

// ler matriz de arquivo
int **ler_matriz(const char *nome_arquivo, int *linhas, int *colunas)
{
    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp)
    {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    fscanf(fp, "%d %d", linhas, colunas);

    int **matriz = malloc((*linhas) * sizeof(int *));
    for (int i = 0; i < *linhas; i++)
    {
        matriz[i] = malloc((*colunas) * sizeof(int));
    }

    for (int i = 0; i < *linhas; i++)
    {
        for (int j = 0; j < *colunas; j++)
        {
            char lixo[20];
            fscanf(fp, "%s %d", lixo, &matriz[i][j]);
        }
    }

    fclose(fp);
    return matriz;
}

// Função da thread
void *calcula_parte(void *arg)
{
    ThreadData *data = (ThreadData *)arg;

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    // Criar pasta específica para o tamanho da matriz
    char pasta[150];
    sprintf(pasta, ".\\resultados\\paralelo\\%dx%d", data->tamanho, data->tamanho);
    _mkdir(pasta); // cria a pasta se não existir

    char nome_arquivo[200];
    sprintf(nome_arquivo, "%s\\resultado_%d.csv", pasta, data->thread_id);

    FILE *fp = fopen(nome_arquivo, "w");
    if (!fp)
    {
        perror("Erro ao criar arquivo");
        pthread_exit(NULL);
    }

    fprintf(fp, "%d %d\n", data->n1, data->m2);

    for (int idx = data->inicio; idx < data->fim; idx++)
    {
        int i = idx / data->m2;
        int j = idx % data->m2;

        int soma = 0;
        for (int k = 0; k < data->m1; k++)
        {
            soma += data->M1[i][k] * data->M2[k][j];
        }

        fprintf(fp, "c%dx%d %d\n", i + 1, j + 1, soma);
    }

    QueryPerformanceCounter(&end);
    double tempo_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
    fprintf(fp, "%.6f\n", tempo_ms);

    fclose(fp);
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Uso: %s <arquivoM1> <arquivoM2> <P>\n", argv[0]);
        return 1;
    }

    char *arq1 = argv[1];
    char *arq2 = argv[2];
    int P = atoi(argv[3]);

    int n1, m1, n2, m2;
    int **M1 = ler_matriz(arq1, &n1, &m1);
    int **M2 = ler_matriz(arq2, &n2, &m2);

    if (m1 != n2)
    {
        fprintf(stderr, "Impossível multiplicar: m1 != n2 (%d,%d)\n", m1, n2);
        return 1;
    }

    // Criar pastas pai
    _mkdir(".\\resultados");
    _mkdir(".\\resultados\\paralelo");

    int total_elem = n1 * m2;
    int num_threads = (int)ceil((double)total_elem / P);

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    ThreadData *thread_data = malloc(num_threads * sizeof(ThreadData));

    // Criação das threads
    for (int t = 0; t < num_threads; t++)
    {
        int inicio = t * P;
        int fim = (inicio + P < total_elem) ? inicio + P : total_elem;

        thread_data[t].M1 = M1;
        thread_data[t].M2 = M2;
        thread_data[t].n1 = n1;
        thread_data[t].m1 = m1;
        thread_data[t].n2 = n2;
        thread_data[t].m2 = m2;
        thread_data[t].inicio = inicio;
        thread_data[t].fim = fim;
        thread_data[t].thread_id = t;
        thread_data[t].tamanho = n1; // tamanho da matriz quadrada

        pthread_create(&threads[t], NULL, calcula_parte, &thread_data[t]);
    }

    // Espera todas as threads
    for (int t = 0; t < num_threads; t++)
    {
        pthread_join(threads[t], NULL);
    }

    printf("Concluído. Foram criados %d arquivos de saída.\n", num_threads);

    // Libera memória
    for (int i = 0; i < n1; i++)
        free(M1[i]);
    for (int i = 0; i < n2; i++)
        free(M2[i]);
    free(M1);
    free(M2);
    free(threads);
    free(thread_data);

    return 0;
}

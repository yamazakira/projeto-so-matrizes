#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <string.h>

int **ler_matriz(const char *nome_arquivo, int *linhas, int *colunas) {
    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    fscanf(fp, "%d %d", linhas, colunas);

    int **matriz = malloc((*linhas) * sizeof(int*));
    for (int i = 0; i < *linhas; i++)
        matriz[i] = malloc((*colunas) * sizeof(int));

    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            char lixo[20];
            fscanf(fp, "%s %d", lixo, &matriz[i][j]);
        }
    }

    fclose(fp);
    return matriz;
}

void calcula_parte(const char *arq1, const char *arq2, int inicio, int fim, int id, const char *saida_path) {
    int n1, m1, n2, m2;
    int **M1 = ler_matriz(arq1, &n1, &m1);
    int **M2 = ler_matriz(arq2, &n2, &m2);

    if (m1 != n2) {
        fprintf(stderr, "Impossível multiplicar: m1 != n2 (%d,%d)\n", m1, n2);
        exit(1);
    }

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    char nome_arquivo[256];
    sprintf(nome_arquivo, "%s/resultado_%d.csv", saida_path, id);
    FILE *fp = fopen(nome_arquivo, "w");
    if (!fp) {
        perror("Erro ao criar arquivo");
        exit(1);
    }

    fprintf(fp, "%d %d\n", n1, m2);

    for (int idx = inicio; idx < fim; idx++) {
        int i = idx / m2;
        int j = idx % m2;

        int soma = 0;
        for (int k = 0; k < m1; k++) {
            soma += M1[i][k] * M2[k][j];
        }

        fprintf(fp, "c%dx%d %d\n", i + 1, j + 1, soma);
    }

    QueryPerformanceCounter(&end);
    double tempo_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
    fprintf(fp, "%.6f\n", tempo_ms);
    fclose(fp);

    for (int i = 0; i < n1; i++) free(M1[i]);
    for (int i = 0; i < n2; i++) free(M2[i]);
    free(M1);
    free(M2);

    exit(0); // Termina o processo filho
}

int main(int argc, char *argv[]) {
    // Modo filho: 7 argumentos
    if (argc == 7) {
        int inicio = atoi(argv[3]);
        int fim = atoi(argv[4]);
        int id = atoi(argv[5]);
        calcula_parte(argv[1], argv[2], inicio, fim, id, argv[6]);
        return 0;
    }

    // Modo pai: 3 argumentos
    if (argc != 4) {
        printf("Uso: %s <arquivoM1> <arquivoM2> <P>\n", argv[0]);
        return 1;
    }

    char *arq1 = argv[1];
    char *arq2 = argv[2];
    int P = atoi(argv[3]);
    char saida_path[] = ".\\resultados\\processos";

    // Ler apenas os tamanhos das matrizes
    FILE *f1 = fopen(arq1, "r");
    FILE *f2 = fopen(arq2, "r");
    if (!f1 || !f2) { perror("Erro ao abrir arquivo"); return 1; }
    int n1, m1, n2, m2;
    fscanf(f1, "%d %d", &n1, &m1);
    fscanf(f2, "%d %d", &n2, &m2);
    fclose(f1);
    fclose(f2);

    if (m1 != n2) { fprintf(stderr, "Impossível multiplicar\n"); return 1; }

    int total_elem = n1 * m2;
    int num_processos = (int) ceil((double) total_elem / P);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    for (int t = 0; t < num_processos; t++) {
        int inicio = t * P;
        int fim = (inicio + P < total_elem) ? inicio + P : total_elem;

        char cmd[512];
        sprintf(cmd, "\"%s\" \"%s\" \"%s\" %d %d %d %s", argv[0], arq1, arq2, inicio, fim, t, saida_path);

        if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            fprintf(stderr, "Erro ao criar processo %d\n", t);
            return 1;
        }

        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }

    printf("Todos os processos criados (%d processos)\n", num_processos);
    return 0;
}

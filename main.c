#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> // Incluímos a biblioteca math.h para usar a função log2

#define MAX_VAL 100000
#define SIZE 100000 // Tamanho do vetor, pode ser ajustado até 1000000
#define NUM_BUSCAS 100 // Número de buscas aleatórias

// Função para gerar números aleatórios dentro de um intervalo
unsigned int rand_range(unsigned int max) {
    return rand() % (max + 1);
}

// Função de comparação para o qsort
int comparar(const void *a, const void *b) {
    return (*(unsigned int*)a - *(unsigned int*)b);
}

// Função de busca binária
int busca_binaria(unsigned int *vetor, int tamanho, unsigned int chave) {
    int esquerda = 0;
    int direita = tamanho - 1;
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        if (vetor[meio] == chave) {
            return meio; // Retorna o índice do elemento encontrado
        } else if (vetor[meio] < chave) {
            esquerda = meio + 1;
        } else {
            direita = meio - 1;
        }
    }
    return -1; // Retorna -1 se o elemento não for encontrado
}

int main() {
    FILE *arquivo = fopen("resultados_busca.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Escreve o cabeçalho do arquivo CSV
    fprintf(arquivo, "Busca,Chave,Índice Encontrado,Comparações,Tempo Execução\n");

    unsigned int *vetor = (unsigned int *)malloc(SIZE * sizeof(unsigned int));
    if (vetor == NULL) {
        printf("Erro na alocação de memória.\n");
        fclose(arquivo);
        return 1;
    }

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Preenche o vetor com valores únicos de forma mais eficiente
    for (unsigned int i = 0; i < SIZE; i++) {
        vetor[i] = i;
    }
    // Embaralha o vetor para garantir aleatoriedade
    for (unsigned int i = 0; i < SIZE; i++) {
        unsigned int j = rand_range(SIZE - 1);
        unsigned int temp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = temp;
    }

    int num_comparacoes;
    clock_t inicio, fim;
    double tempo_execucao;

    // Ordena o vetor antes de realizar as buscas
    qsort(vetor, SIZE, sizeof(unsigned int), comparar);

    // Realiza 100 buscas aleatórias e grava os resultados no arquivo CSV
    for (int i = 0; i < NUM_BUSCAS; i++) {
        unsigned int chave = rand_range(MAX_VAL);
        inicio = clock();
        int indice_encontrado = busca_binaria(vetor, SIZE, chave);
        fim = clock();
        num_comparacoes = indice_encontrado != -1 ? log2(indice_encontrado + 1) : log2(SIZE);
        tempo_execucao = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

        // Escreve os resultados da busca no arquivo CSV
        fprintf(arquivo, "%d,%u,%d,%d,%f\n", i + 1, chave, indice_encontrado, num_comparacoes, tempo_execucao);
    }

    // Libera a memória alocada e fecha o arquivo
    free(vetor);
    fclose(arquivo);

    printf("Os resultados das buscas foram salvos em 'resultados_busca.csv'.\n");

    return 0;
}

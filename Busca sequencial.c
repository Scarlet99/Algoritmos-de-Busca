#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#ifdef __linux__
#include <malloc.h>
#endif

#define MAX_VAL 100000
#define MIN_SIZE 100000  // Tamanho mínimo do vetor
#define MAX_SIZE 1000000 // Tamanho máximo do vetor
#define SIZE_STEP 100000 // Incremento do tamanho do vetor
#define NUM_BUSCAS 100   // Número de buscas aleatórias

// Função para gerar números aleatórios dentro de um intervalo
unsigned int rand_range(unsigned int max) { return rand() % (max + 1); }

// Função de busca sequencial
int busca_sequencial(unsigned int *vetor, int tamanho, unsigned int chave) {
    for (int i = 0; i < tamanho; i++) {
        if (vetor[i] == chave) {
            return i; // Retorna o índice do elemento encontrado
        }
    }
    return -1; // Retorna -1 se o elemento não for encontrado
}

// Função para calcular a média
double calcular_media(double *valores, int n) {
    double soma = 0.0;
    for (int i = 0; i < n; i++) {
        soma += valores[i];
    }
    return soma / n;
}

// Função para calcular o desvio padrão
double calcular_desvio_padrao(double *valores, int n, double media) {
    double soma = 0.0;
    for (int i = 0; i < n; i++) {
        soma += (valores[i] - media) * (valores[i] - media);
    }
    return sqrt(soma / n);
}

int main() {
    FILE *arquivo = fopen("resultados_busca.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Escreve o cabeçalho do arquivo CSV
    fprintf(arquivo,
            "Tamanho Vetor,Busca,Chave,Índice Encontrado,Comparações,Tempo Execução,Consumo Memória\n");

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Loop para testar diferentes tamanhos de vetor
    for (unsigned int tamanho_vetor = MIN_SIZE; tamanho_vetor <= MAX_SIZE; tamanho_vetor += SIZE_STEP) {
        unsigned int *vetor = (unsigned int *)malloc(tamanho_vetor * sizeof(unsigned int));
        if (vetor == NULL) {
            printf("Erro na alocação de memória.\n");
            fclose(arquivo);
            return 1;
        }

        // Preenche o vetor com valores únicos de forma mais eficiente
        for (unsigned int i = 0; i < tamanho_vetor; i++) {
            vetor[i] = i;
        }
        // Embaralha o vetor para garantir aleatoriedade
        for (unsigned int i = 0; i < tamanho_vetor; i++) {
            unsigned int j = rand_range(tamanho_vetor - 1);
            unsigned int temp = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = temp;
        }

        double tempos_execucao[NUM_BUSCAS];
        double num_comparacoes[NUM_BUSCAS];
        double consumos_memoria[NUM_BUSCAS];
        size_t consumo_memoria = tamanho_vetor * sizeof(unsigned int);
#ifdef __linux__
        consumo_memoria = malloc_usable_size(vetor);
#endif

        // Realiza 100 buscas aleatórias e grava os resultados no arquivo CSV
        for (int i = 0; i < NUM_BUSCAS; i++) {
            unsigned int chave = rand_range(MAX_VAL);
            clock_t inicio = clock();
            int indice_encontrado = busca_sequencial(vetor, tamanho_vetor, chave);
            clock_t fim = clock();
            num_comparacoes[i] = indice_encontrado != -1 ? indice_encontrado + 1 : tamanho_vetor;
            tempos_execucao[i] = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            consumos_memoria[i] = consumo_memoria;

            // Escreve os resultados da busca no arquivo CSV
            fprintf(arquivo, "%u,%d,%u,%d,%f,%f,%zu\n", tamanho_vetor, i + 1, chave, indice_encontrado,
                    num_comparacoes[i], tempos_execucao[i], consumo_memoria);
        }

        double media_comparacoes = calcular_media(num_comparacoes, NUM_BUSCAS);
        double desvio_padrao_comparacoes = calcular_desvio_padrao(num_comparacoes, NUM_BUSCAS, media_comparacoes);
        double media_tempo_execucao = calcular_media(tempos_execucao, NUM_BUSCAS);
        double desvio_padrao_tempo_execucao = calcular_desvio_padrao(tempos_execucao, NUM_BUSCAS, media_tempo_execucao);
        double media_consumo_memoria = calcular_media(consumos_memoria, NUM_BUSCAS);
        double desvio_padrao_consumo_memoria = calcular_desvio_padrao(consumos_memoria, NUM_BUSCAS, media_consumo_memoria);

        // Imprime a média e o desvio padrão para o tamanho atual do vetor
        printf("Tamanho do vetor: %u\n", tamanho_vetor);
        printf("Média de comparações: %f\n", media_comparacoes);
        printf("Desvio padrão de comparações: %f\n", desvio_padrao_comparacoes);
        printf("Média de tempo de execução: %f\n", media_tempo_execucao);
        printf("Desvio padrão de tempo de execução: %f\n", desvio_padrao_tempo_execucao);
        printf("Média de consumo de memória: %f\n", media_consumo_memoria);
        printf("Desvio padrão de consumo de memória: %f\n", desvio_padrao_consumo_memoria);

        // Libera a memória alocada
        free(vetor);
    }

    // Fecha o arquivo
    fclose(arquivo);

    printf("Os resultados das buscas foram salvos em 'resultados_busca.csv'.\n");

    return 0;
}



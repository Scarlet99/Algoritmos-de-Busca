#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_VAL 100000
#define SIZE_INCREMENT 100000 // Incremento do tamanho do vetor
#define NUM_BUSCAS 100 // Número de buscas aleatórias
#define NUM_EXECUCOES 3 // Número de execuções para calcular média e desvio padrão
#define MAX_SIZE 1000000 // Tamanho máximo do vetor

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

// Função para calcular o consumo de memória do vetor
size_t calcular_consumo_memoria(unsigned int *vetor, int tamanho) {
    return tamanho * sizeof(unsigned int);
}

int main() {
    FILE *arquivo = fopen("resultados_busca.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Escreve o cabeçalho do arquivo CSV
    fprintf(arquivo, "Tamanho Vetor,Execucao,Busca,Chave,Indice Encontrado,Comparações,Tempo Execucao,Consumo Memoria\n");

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Itera sobre todos os tamanhos de vetor desejados
    for (int tamanho_vetor = SIZE_INCREMENT; tamanho_vetor <= MAX_SIZE; tamanho_vetor += SIZE_INCREMENT) {
        // Variáveis para cálculo das estatísticas
        double soma_comparacoes = 0, soma_tempo = 0, soma_memoria = 0;
        double soma_quad_comparacoes = 0, soma_quad_tempo = 0;

        for (int execucao = 1; execucao <= NUM_EXECUCOES; execucao++) {
            unsigned int *vetor = (unsigned int *)malloc(tamanho_vetor * sizeof(unsigned int));
            if (vetor == NULL) {
                printf("Erro na alocação de memória.\n");
                fclose(arquivo);
                return 1;
            }

            // Preenche o vetor com valores únicos
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

            // Ordena o vetor antes de realizar as buscas
            qsort(vetor, tamanho_vetor, sizeof(unsigned int), comparar);

            // Realiza as buscas no vetor e grava os resultados no arquivo CSV
            for (int i = 0; i < NUM_BUSCAS; i++) {
                unsigned int chave = rand_range(MAX_VAL);
                clock_t inicio = clock();
                int indice_encontrado = busca_binaria(vetor, tamanho_vetor, chave);
                clock_t fim = clock();
                int num_comparacoes = (indice_encontrado != -1) ? log2(indice_encontrado + 1) : log2(tamanho_vetor);
                double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

                // Calcula o consumo de memória
                size_t consumo_memoria = calcular_consumo_memoria(vetor, tamanho_vetor);

                // Atualiza as somas para cálculo da média e do desvio padrão
                soma_comparacoes += num_comparacoes;
                soma_tempo += tempo_execucao;
                soma_memoria += consumo_memoria;
                soma_quad_comparacoes += num_comparacoes * num_comparacoes;
                soma_quad_tempo += tempo_execucao * tempo_execucao;

                // Escreve os resultados da busca no arquivo CSV
                fprintf(arquivo, "%d,%d,%d,%u,%d,%d,%f,%zu\n", tamanho_vetor, execucao, i + 1, chave, indice_encontrado, num_comparacoes, tempo_execucao, consumo_memoria);
            }

            // Libera a memória alocada para o vetor
            free(vetor);
        }

        // Calcula média e desvio padrão
        int total_execucoes = NUM_EXECUCOES * NUM_BUSCAS;
        double media_comparacoes = soma_comparacoes / total_execucoes;
        double media_tempo = soma_tempo / total_execucoes;
        double media_memoria = soma_memoria / total_execucoes;
        double desvio_padrao_comparacoes = sqrt((soma_quad_comparacoes / total_execucoes) - (media_comparacoes * media_comparacoes));
        double desvio_padrao_tempo = sqrt((soma_quad_tempo / total_execucoes) - (media_tempo * media_tempo));

        // Imprime os resultados das médias e desvios padrões na tela
        printf("Tamanho do vetor: %d\n", tamanho_vetor);
        printf("Média de comparações: %.2f, Desvio padrão: %.2f\n", media_comparacoes, desvio_padrao_comparacoes);
        printf("Média de tempo de execução: %.6f s, Desvio padrão: %.6f s\n", media_tempo, desvio_padrao_tempo);
        printf("Média de consumo de memória: %.2f bytes\n", media_memoria);
        printf("-----------------------------------\n");
    }

    // Fecha o arquivo
    fclose(arquivo);

    printf("Os resultados das buscas foram salvos em 'resultados_busca.csv'.\n");

    return 0;
}


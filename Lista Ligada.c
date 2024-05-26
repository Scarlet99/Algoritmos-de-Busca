#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_VAL 100000
#define MIN_SIZE 100000 // Tamanho mínimo do vetor
#define MAX_SIZE 1000000 // Tamanho máximo do vetor
#define SIZE_STEP 100000 // Incremento do tamanho do vetor
#define NUM_BUSCAS 100 // Número de buscas aleatórias
#define NUM_EXECUCOES 3 // Número de execuções do pior caso

// Definição da estrutura de um nó da lista ligada
typedef struct No {
    unsigned int valor;
    struct No *proximo;
} No;

// Função para criar um novo nó
No *novo_no(unsigned int valor) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro na alocação de memória.\n");
        exit(1);
    }
    novo->valor = valor;
    novo->proximo = NULL;
    return novo;
}

// Função para inserir um nó no início da lista
void inserir_inicio(No **cabeca, unsigned int valor) {
    No *novo = novo_no(valor);
    novo->proximo = *cabeca;
    *cabeca = novo;
}

// Função de busca sequencial em lista ligada
int busca_sequencial_lista(No *cabeca, unsigned int chave, int *num_comparacoes) {
    No *atual = cabeca;
    int indice = 0;
    while (atual != NULL) {
        (*num_comparacoes)++;
        if (atual->valor == chave) {
            return indice; // Retorna o índice do elemento encontrado
        }
        atual = atual->proximo;
        indice++;
    }
    return -1; // Retorna -1 se o elemento não for encontrado
}

// Função para gerar números aleatórios dentro de um intervalo
unsigned int rand_range(unsigned int max) {
    return rand() % (max + 1);
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
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Abre o arquivo para escrita dos resultados
    FILE *arquivo = fopen("resultados_busca.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Escreve o cabeçalho do arquivo CSV
    fprintf(arquivo, "Tamanho Lista,Busca,Chave,Índice Encontrado,Comparações,Tempo Execução,Consumo Memória\n");

    // Loop para testar diferentes tamanhos de lista
    for (unsigned int tamanho_lista = MIN_SIZE; tamanho_lista <= MAX_SIZE; tamanho_lista += SIZE_STEP) {
        // Criação do vetor e preenchimento com valores únicos
        unsigned int *vetor = (unsigned int *)malloc(tamanho_lista * sizeof(unsigned int));
        if (vetor == NULL) {
            printf("Erro na alocação de memória.\n");
            return 1;
        }

        for (unsigned int i = 0; i < tamanho_lista; i++) {
            vetor[i] = i;
        }

        // Embaralha o vetor
        for (unsigned int i = 0; i < tamanho_lista; i++) {
            unsigned int j = rand_range(tamanho_lista - 1);
            unsigned int temp = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = temp;
        }

        // Criação da lista ligada a partir do vetor
        No *cabeca = NULL;
        for (int i = tamanho_lista - 1; i >= 0; i--) {
            inserir_inicio(&cabeca, vetor[i]);
        }

        // Arrays para armazenar resultados
        double tempos_execucao[NUM_BUSCAS];
        double num_comparacoes[NUM_BUSCAS];
        double consumos_memoria[NUM_BUSCAS];

        // Realiza as buscas na lista ligada e salva os resultados no arquivo CSV
        for (int i = 0; i < NUM_BUSCAS; i++) {
            unsigned int chave = rand_range(MAX_VAL);
            clock_t inicio = clock();
            int comparacoes = 0;
            int indice_encontrado = busca_sequencial_lista(cabeca, chave, &comparacoes);
            clock_t fim = clock();
            double tempo_execucao = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

            // Salva os resultados no array
            num_comparacoes[i] = comparacoes;
            tempos_execucao[i] = tempo_execucao;
            consumos_memoria[i] = tamanho_lista * sizeof(No);

            // Escreve os resultados da busca no arquivo CSV
            fprintf(arquivo, "%u,%d,%u,%d,%d,%f,%f\n", tamanho_lista, i + 1, chave, indice_encontrado, comparacoes, tempo_execucao, consumos_memoria[i]);
        }

        // Calcula a média e o desvio padrão
        double media_comparacoes = calcular_media(num_comparacoes, NUM_BUSCAS);
        double desvio_padrao_comparacoes = calcular_desvio_padrao(num_comparacoes, NUM_BUSCAS, media_comparacoes);
        double media_tempo_execucao = calcular_media(tempos_execucao, NUM_BUSCAS);
        double desvio_padrao_tempo_execucao = calcular_desvio_padrao(tempos_execucao, NUM_BUSCAS, media_tempo_execucao);
        double media_consumo_memoria = calcular_media(consumos_memoria, NUM_BUSCAS);
        double desvio_padrao_consumo_memoria = calcular_desvio_padrao(consumos_memoria, NUM_BUSCAS, media_consumo_memoria);

        // Imprime a média e o desvio padrão
        printf("Tamanho do vetor: %u\n", tamanho_lista);
        printf("Média de comparações: %f\n", media_comparacoes);
        printf("Desvio padrão de comparações: %f\n", desvio_padrao_comparacoes);
        printf("Média de tempo de execução: %f\n", media_tempo_execucao);
        printf("Desvio padrão de tempo de execução: %f\n", desvio_padrao_tempo_execucao);
        printf("Média de consumo de memória: %f\n", media_consumo_memoria);
        printf("Desvio padrão de consumo de memória: %f\n", desvio_padrao_consumo_memoria);

        // Arrays para armazenar resultados do pior caso
        double tempos_execucao_pior[NUM_EXECUCOES];
        double num_comparacoes_pior[NUM_EXECUCOES];
        double consumos_memoria_pior[NUM_EXECUCOES];

        // Realiza as buscas do pior caso (chave não presente) na lista ligada
        for (int i = 0; i < NUM_EXECUCOES; i++) {
            unsigned int chave = tamanho_lista + 1; // Chave não presente
            clock_t inicio = clock();
            int comparacoes = 0;
            int indice_encontrado = busca_sequencial_lista(cabeca, chave, &comparacoes);
            clock_t fim = clock();
            double tempo_execucao = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

            // Salva os resultados no array
            num_comparacoes_pior[i] = comparacoes;
            tempos_execucao_pior[i] = tempo_execucao;
            consumos_memoria_pior[i] = tamanho_lista * sizeof(No);

            // Escreve os resultados da busca no arquivo CSV
            fprintf(arquivo, "%u,Pior Caso %d,%u,%d,%d,%f,%f\n", tamanho_lista, i + 1, chave, indice_encontrado, comparacoes, tempo_execucao, consumos_memoria_pior[i]);
        }

        // Calcula a média e o desvio padrão para o pior caso
      double media_comparacoes_pior = calcular_media(num_comparacoes_pior, NUM_EXECUCOES);
      double desvio_padrao_comparacoes_pior = calcular_desvio_padrao(num_comparacoes_pior, NUM_EXECUCOES, media_comparacoes_pior);
      double media_tempo_execucao_pior = calcular_media(tempos_execucao_pior, NUM_EXECUCOES);
double desvio_padrao_tempo_execucao_pior = calcular_desvio_padrao(tempos_execucao_pior, NUM_EXECUCOES, media_tempo_execucao_pior);
double media_consumo_memoria_pior = calcular_media(consumos_memoria_pior, NUM_EXECUCOES);
double desvio_padrao_consumo_memoria_pior = calcular_desvio_padrao(consumos_memoria_pior, NUM_EXECUCOES, media_consumo_memoria_pior);

// Imprime a média e o desvio padrão para o pior caso
printf("Pior caso para o tamanho do vetor: %u\n", tamanho_lista);
printf("Média de comparações (pior caso): %f\n", media_comparacoes_pior);
printf("Desvio padrão de comparações (pior caso): %f\n", desvio_padrao_comparacoes_pior);
printf("Média de tempo de execução (pior caso): %f\n", media_tempo_execucao_pior);
printf("Desvio padrão de tempo de execução (pior caso): %f\n", desvio_padrao_tempo_execucao_pior);
printf("Média de consumo de memória (pior caso): %f\n", media_consumo_memoria_pior);
printf("Desvio padrão de consumo de memória (pior caso): %f\n", desvio_padrao_consumo_memoria_pior);

// Libera a memória alocada para o vetor e a lista ligada
free(vetor);
while (cabeca != NULL) {
    No *temp = cabeca;
    cabeca = cabeca->proximo;
    free(temp);
}
}

// Fecha o arquivo
fclose(arquivo);

printf("Os resultados das buscas foram salvos em 'resultados_busca.csv'.\n");

return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VAL 100000
#define SIZE 100000 // Tamanho do vetor, pode ser ajustado até 1000000
#define NUM_BUSCAS 100 // Número de buscas aleatórias

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

// Função para imprimir a lista ligada
void imprimir_lista(No *cabeca) {
    No *atual = cabeca;
    while (atual != NULL) {
        printf("%u ", atual->valor);
        atual = atual->proximo;
    }
    printf("\n");
}

// Função de busca sequencial em lista ligada
int busca_sequencial_lista(No *cabeca, unsigned int chave) {
    No *atual = cabeca;
    int indice = 0;
    while (atual != NULL) {
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

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Criação do vetor e preenchimento com valores únicos
    unsigned int *vetor = (unsigned int *)malloc(SIZE * sizeof(unsigned int));
    if (vetor == NULL) {
        printf("Erro na alocação de memória.\n");
        return 1;
    }

    for (unsigned int i = 0; i < SIZE; i++) {
        vetor[i] = i;
    }

    // Embaralha o vetor
    for (unsigned int i = 0; i < SIZE; i++) {
        unsigned int j = rand_range(SIZE - 1);
        unsigned int temp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = temp;
    }

    // Criação da lista ligada a partir do vetor
    No *cabeca = NULL;
    for (int i = SIZE - 1; i >= 0; i--) {
        inserir_inicio(&cabeca, vetor[i]);
    }

    // Imprime a lista ligada
    imprimir_lista(cabeca);

    // Abre o arquivo para escrita dos resultados
    FILE *arquivo = fopen("resultados_busca.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        free(vetor);
        while (cabeca != NULL) {
            No *temp = cabeca;
            cabeca = cabeca->proximo;
            free(temp);
        }
        return 1;
    }

    // Escreve o cabeçalho do arquivo CSV
    fprintf(arquivo, "Busca;Chave;Índice Encontrado;Comparações;Tempo Execução\n");

    // Realiza as buscas na lista ligada e salva os resultados no arquivo CSV
    for (int i = 0; i < NUM_BUSCAS; i++) {
        unsigned int chave = rand_range(MAX_VAL);
        clock_t inicio = clock();
        int indice_encontrado = busca_sequencial_lista(cabeca, chave);
        clock_t fim = clock();
        int num_comparacoes = indice_encontrado != -1 ? indice_encontrado + 1 : SIZE;
        double tempo_execucao = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

        // Escreve os resultados da busca no arquivo CSV
        fprintf(arquivo, "%d;%u;%d;%d;%f\n", i + 1, chave, indice_encontrado, num_comparacoes, tempo_execucao);
    }

    // Fecha o arquivo
    fclose(arquivo);

    // Libera a memória alocada para o vetor e a lista ligada
    free(vetor);
    while (cabeca != NULL) {
        No *temp = cabeca;
        cabeca = cabeca->proximo;
        free(temp);
    }

    printf("Os resultados das buscas foram salvos em 'resultados_busca.csv'.\n");

    return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VAL 100000
#define SIZE 100000 // Tamanho do vetor, pode ser ajustado até 1000000
#define NUM_BUSCAS 100 // Número de buscas aleatórias

// Definição da estrutura de um nó da árvore binária de busca
typedef struct NoArvore {
    unsigned int valor;
    struct NoArvore *esquerda;
    struct NoArvore *direita;
} NoArvore;

// Função para criar um novo nó da árvore
NoArvore *novo_no_arvore(unsigned int valor) {
    NoArvore *novo = (NoArvore *)malloc(sizeof(NoArvore));
    if (novo == NULL) {
        printf("Erro na alocação de memória.\n");
        exit(1);
    }
    novo->valor = valor;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Função para inserir um nó na árvore
NoArvore *inserir_arvore(NoArvore *raiz, unsigned int valor) {
    if (raiz == NULL) {
        return novo_no_arvore(valor);
    }
    if (valor < raiz->valor) {
        raiz->esquerda = inserir_arvore(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = inserir_arvore(raiz->direita, valor);
    }
    return raiz;
}

// Função de busca na árvore binária de busca com contagem de comparações
NoArvore *busca_arvore_contagem(NoArvore *raiz, unsigned int chave, int *comparacoes) {
    if (raiz == NULL || raiz->valor == chave) {
        (*comparacoes)++;
        return raiz;
    }
    (*comparacoes)++;
    if (chave < raiz->valor) {
        return busca_arvore_contagem(raiz->esquerda, chave, comparacoes);
    }
    return busca_arvore_contagem(raiz->direita, chave, comparacoes);
}

// Função para liberar a memória da árvore
void liberar_arvore(NoArvore *raiz) {
    if (raiz == NULL) {
        return;
    }
    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);
    free(raiz);
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

    // Criação da árvore binária de busca a partir do vetor
    NoArvore *raiz = NULL;
    for (int i = 0; i < SIZE; i++) {
        raiz = inserir_arvore(raiz, vetor[i]);
    }

    // Abre o arquivo para escrita dos resultados
    FILE *arquivo = fopen("resultados_busca.csv", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        free(vetor);
        liberar_arvore(raiz);
        return 1;
    }

    // Escreve o cabeçalho do arquivo CSV
    fprintf(arquivo, "Busca;Chave;Encontrado;Comparações;Tempo Execução\n");

    // Realiza as buscas na árvore e salva os resultados no arquivo CSV
    for (int i = 0; i < NUM_BUSCAS; i++) {
        unsigned int chave = rand_range(MAX_VAL);
        int num_comparacoes = 0;
        clock_t inicio = clock();
        NoArvore *encontrado = busca_arvore_contagem(raiz, chave, &num_comparacoes);
        clock_t fim = clock();
        double tempo_execucao = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

        // Escreve os resultados da busca no arquivo CSV
        fprintf(arquivo, "%d;%u;%s;%d;%f\n", i + 1, chave, encontrado != NULL ? "Sim" : "Não", num_comparacoes, tempo_execucao);
    }

    // Fecha o arquivo
    fclose(arquivo);

    // Libera a memória alocada para o vetor e a árvore
    free(vetor);
    liberar_arvore(raiz);

    printf("Os resultados das buscas foram salvos em 'resultados_busca.csv'.\n");

    return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_VAL 100000
#define SIZE_INCREMENT 100000 // Incremento do tamanho do vetor
#define NUM_BUSCAS 100 // Número de buscas aleatórias
#define MAX_SIZE 1000000 // Tamanho máximo do vetor

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

// Função para calcular o tamanho da árvore binária de busca
size_t calcular_tamanho_arvore(NoArvore *raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return sizeof(NoArvore) + calcular_tamanho_arvore(raiz->esquerda) + calcular_tamanho_arvore(raiz->direita);
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
    fprintf(arquivo, "Tamanho Vetor;Busca;Chave;Encontrado;Comparações;Tempo Execução (ms);Consumo Memória (bytes)\n");

    // Itera sobre os tamanhos de vetor desejados
    for (unsigned int tamanho_vetor = SIZE_INCREMENT; tamanho_vetor <= MAX_SIZE; tamanho_vetor += SIZE_INCREMENT) {
        // Criação do vetor e preenchimento com valores únicos
        unsigned int *vetor = (unsigned int *)malloc(tamanho_vetor * sizeof(unsigned int));
        if (vetor == NULL) {
            printf("Erro na alocação de memória.\n");
            return 1;
        }

        for (unsigned int i = 0; i < tamanho_vetor; i++) {
            vetor[i] = i;
        }

        // Embaralha o vetor
        for (unsigned int i = 0; i < tamanho_vetor; i++) {
            unsigned int j = rand_range(tamanho_vetor - 1);
            unsigned int temp = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = temp;
        }

        // Criação da árvore binária de busca a partir do vetor
        NoArvore *raiz = NULL;
        for (unsigned int i = 0; i < tamanho_vetor; i++) {
            raiz = inserir_arvore(raiz, vetor[i]);
        }

        // Calcula o consumo de memória da árvore
        size_t memoria_arvore = calcular_tamanho_arvore(raiz);
        size_t memoria_vetor = tamanho_vetor * sizeof(unsigned int);
        size_t memoria_total = memoria_vetor + memoria_arvore;

        // Variáveis para cálculo da média e desvio padrão
        double soma_comparacoes = 0, soma_tempo = 0, soma_memoria = 0;
        double soma_quad_comparacoes = 0, soma_quad_tempo = 0, soma_quad_memoria = 0;

        // Realiza as buscas na árvore e salva os resultados no arquivo CSV
        for (int i = 0; i < NUM_BUSCAS; i++) {
            unsigned int chave = rand_range(MAX_VAL);
            int num_comparacoes = 0;
            clock_t inicio = clock();
            NoArvore *encontrado = busca_arvore_contagem(raiz, chave, &num_comparacoes);
            clock_t fim = clock();
            double tempo_execucao = ((double)(fim - inicio)) / (CLOCKS_PER_SEC / 1000.0); // Tempo em milissegundos

            // Escreve os resultados da busca no arquivo CSV
            fprintf(arquivo, "%u;%d;%u;%s;%d;%.3f;%zu\n", tamanho_vetor, i + 1, chave, encontrado != NULL ? "Sim" : "Não", num_comparacoes, tempo_execucao, memoria_total);

            // Atualiza as somas para cálculo da média e desvio padrão
            soma_comparacoes += num_comparacoes;
            soma_tempo += tempo_execucao;
            soma_memoria += memoria_total;
            soma_quad_comparacoes += num_comparacoes * num_comparacoes;
            soma_quad_tempo += tempo_execucao * tempo_execucao;
            soma_quad_memoria += memoria_total * memoria_total;
        }

        // Calcula a média e o desvio padrão
        double media_comparacoes = soma_comparacoes / NUM_BUSCAS;
        double media_tempo = soma_tempo / NUM_BUSCAS;
        double media_memoria = soma_memoria / NUM_BUSCAS;
        double desvio_padrao_comparacoes = sqrt(soma_quad_comparacoes / NUM_BUSCAS - media_comparacoes * media_comparacoes);
        double desvio_padrao_tempo = sqrt(soma_quad_tempo / NUM_BUSCAS - media_tempo * media_tempo);
        double desvio_padrao_memoria = sqrt(soma_quad_memoria / NUM_BUSCAS - media_memoria * media_memoria);

        // Libera a memória alocada para o vetor e a árvore
        free(vetor);
        liberar_arvore(raiz);

        // Imprime os resultados na tela
        printf("Tamanho do vetor: %u\n", tamanho_vetor);
        printf("Média de comparações: %.2f\n", media_comparacoes);
        printf("Desvio padrão de comparações: %.2f\n", desvio_padrao_comparacoes);
        printf("Média de tempo de execução: %.3f milissegundos\n", media_tempo);
        printf("Desvio padrão de tempo de execução: %.3f milissegundos\n", desvio_padrao_tempo);
        printf("Média de consumo de memória: %.2f bytes\n", media_memoria);
        printf("Desvio padrão de consumo de memória: %.2f bytes\n", desvio_padrao_memoria);
        printf("-----------------------------------\n");
    }

    // Fecha o arquivo
    fclose(arquivo);

    printf("Os resultados das buscas foram salvos em 'resultados_busca.csv'.\n");

    return 0;
}




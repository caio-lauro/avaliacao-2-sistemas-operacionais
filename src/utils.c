#include "utils.h"

// Variável para armazenar o tamanho da matriz de impressão
const int TAMANHO_MATRIZ_IMPRESSAO = 3 * TAMANHO + 3;

// Variável para armazenar o tamanho da borda
const int TAMANHO_BORDA = TAMANHO_MATRIZ_IMPRESSAO + 2;

matriz_t *criar_matriz(size_t tamanho) {
    matriz_element_t **elementos = calloc(tamanho, sizeof(matriz_element_t*));
    if (elementos == NULL) {
        printf("Erro na alocação dos ponteiros da matriz.\n");
        exit(EXIT_FAILURE); 
    }
    
    elementos[0] = calloc(tamanho * tamanho, sizeof(matriz_element_t));
    if (elementos[0] == NULL) {
        printf("Erro na alocação dos elementos da matriz\n");
        exit(EXIT_FAILURE);
    }
    
    // Configurar ponteiros para cada linha
    for (size_t j = 1; j < tamanho; j++) {
        elementos[j] = elementos[0] + j * tamanho;
    }

    for (size_t i = 0; i < tamanho; i++) {
        for (size_t j = 0; j < tamanho; j++) {
            if ((i % 3 == 1) && (j % 3 == 1)) {
                elementos[i][j].c = 'T';
                elementos[i][j].pid = 0;
                elementos[i][j].thread_id = 0;
            } else {
                elementos[i][j].c = '-';
                elementos[i][j].pid = -1;
                elementos[i][j].thread_id = -1;
            }
        }
    }
    
    // Criar a estrutura da matriz
    matriz_t *matriz = malloc(sizeof(matriz_t));
    if (matriz == NULL) {
        printf("Erro na alocação da estrutura da matriz.\n");
        exit(EXIT_FAILURE);
    }

    matriz->elementos = elementos;
    matriz->size = tamanho;

    return matriz;
}

void imprimir_matriz(matriz_t *matriz) {
    // Variável para armazenar números das guias com espaçamento
    char numero_guia[5];

    // Borda superior
    for (int j = 0; j < TAMANHO_BORDA; j++) {
        print_colorido_rgb(" ", fundo_borda, NULL);
    }
    printf("\n");

    // Borda lateral da esquerda
    print_colorido_rgb(" ", fundo_borda, NULL);

    // Imprimir guias de colunas
    // Espaçamento inicial
    print_colorido_rgb("  ", fundo, texto_colunas);

    // Números das colunas
    for (size_t j = 0; j < matriz->size; j++) {
        sprintf(numero_guia, " %2ld", j);
        print_colorido_rgb(numero_guia, fundo, texto_colunas);
    }
    print_colorido_rgb(" ", fundo, texto_colunas);

    // Borda lateral da direita
    print_colorido_rgb(" ", fundo_borda, NULL);

    printf("\n");

    // Imprimir linhas da matriz
    for (size_t i = 0; i < matriz->size; i++) {
        // Borda lateral da esquerda
        print_colorido_rgb(" ", fundo_borda, NULL);

        // Imprimir guia de linhas
        sprintf(numero_guia, "%2ld ", i);
        print_colorido_rgb(numero_guia, fundo, texto_linhas);

        // Imprimir elementos da linha
        for (size_t j = 0; j < matriz->size; j++) {
            switch (matriz->elementos[i][j].c) {
                case 'F':
                    print_colorido_rgb(" ", fundo, NULL);
                    print_colorido_rgb(FOGO, fundo, NULL);
                    print_colorido_rgb(" ", fundo, NULL);
                    break;
                case '-':
                    print_colorido_rgb(" - ", fundo, texto);
                    break;
                case 'T':
                    print_colorido_rgb(" T ", fundo, texto);
                    break;
            }
        }

        // Borda lateral da direita
        print_colorido_rgb(" ", fundo_borda, NULL);

        printf("\n");
    }

    // Borda inferior
    for (int j = 0; j < TAMANHO_BORDA; j++) {
        print_colorido_rgb(" ", fundo_borda, NULL);
    }
    printf("\n");
}

void liberar_matriz(matriz_t **matriz) {
    free((*matriz)->elementos[0]);
    free((*matriz)->elementos);
    (*matriz) = NULL;
}
#include "utils.h"

// Variável para armazenar o tamanho da matriz de impressão
const int TAMANHO_MATRIZ_IMPRESSAO = 3 * TAMANHO + 3;

// Variável para armazenar o tamanho da borda
const int TAMANHO_BORDA = TAMANHO_MATRIZ_IMPRESSAO + 2;

void inicializar_matriz(matriz_t *matriz, size_t tamanho) {
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
            } else {
                elementos[i][j].c = '-';
            }
            elementos[i][j].fogo = false;
        }
    }
    
    // Criar a estrutura da matriz
    matriz->elementos = elementos;
    matriz->size = tamanho;
}

void inicializar_vetor(pthread_arr_t *arr, size_t tamanho_matriz) {
    size_t qtd_linha = tamanho_matriz / 3;
    size_t tamanho = qtd_linha * qtd_linha;

    arr_element_t *elementos = malloc(sizeof(arr_element_t) * tamanho);
    if (elementos == NULL) {
        printf("Erro na alocação dos elementos do vetor\n");
        exit(EXIT_FAILURE);
    }

    size_t k = 0;
    for (size_t i = 1; i < tamanho_matriz; i += 3) {
        for (size_t j = 1; j < tamanho_matriz; j += 3) {
            elementos[k].pid = elementos[k].thread_id = 0;
            elementos[k].x = j;
            elementos[k++].y = i;
        }
    }

    arr->elementos = elementos;
    arr->size = tamanho;
}

void imprimir_matriz(matriz_t matriz) {
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
    for (size_t j = 0; j < matriz.size; j++) {
        sprintf(numero_guia, " %2ld", j);
        print_colorido_rgb(numero_guia, fundo, texto_colunas);
    }
    print_colorido_rgb(" ", fundo, texto_colunas);

    // Borda lateral da direita
    print_colorido_rgb(" ", fundo_borda, NULL);

    printf("\n");

    // Imprimir linhas da matriz
    for (size_t i = 0; i < matriz.size; i++) {
        // Borda lateral da esquerda
        print_colorido_rgb(" ", fundo_borda, NULL);

        // Imprimir guia de linhas
        sprintf(numero_guia, "%2ld ", i);
        print_colorido_rgb(numero_guia, fundo, texto_linhas);

        // Imprimir elementos da linha
        for (size_t j = 0; j < matriz.size; j++) {
            if (matriz.elementos[i][j].fogo) {
                print_colorido_rgb(FOGO, fundo, NULL);
            } else if (matriz.elementos[i][j].c == 'T') {
                print_colorido_rgb(" T ", fundo, texto);
            } else {
                print_colorido_rgb(" - ", fundo, texto);
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

void liberar_matriz(matriz_t *matriz) {
    free(matriz->elementos[0]);
    free(matriz->elementos);
    matriz = NULL;
}

void liberar_vetor(pthread_arr_t *arr) {
    free(arr->elementos);
    arr = NULL;
}
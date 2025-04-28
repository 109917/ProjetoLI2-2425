#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Faltava para isdigit, isupper, islower

#define TAMANHO 5

typedef struct estado {
    char tabuleiro[TAMANHO][TAMANHO];
    struct estado *anterior;
} estado_t;

estado_t *estado_atual = NULL;

// Função que salva o novo estado do tabuleiro
void salvar_estado() {
    estado_t *novo = malloc(sizeof(estado_t));
    memcpy(novo->tabuleiro, estado_atual->tabuleiro, sizeof(novo->tabuleiro));
    novo->anterior = estado_atual;
    estado_atual = novo;
}

// Função que volta ao estado anterior do tabuleiro
void desfazer() {
    if (estado_atual->anterior) {
        estado_t *temp = estado_atual;
        estado_atual = estado_atual->anterior;
        free(temp);
    } else {
        printf("Não há mais estados para desfazer.\n");
    }
}

void começar_jogo() {
    estado_atual = malloc(sizeof(estado_t));
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            estado_atual->tabuleiro[i][j] = 'a' + (j % 5);
        }
    }
    estado_atual->anterior = NULL;
}

typedef struct {
    int linha;
    int coluna;
} coordenada_t;

coordenada_t parse_coord(const char *str) {
    coordenada_t c;
    c.coluna = str[0] - 'a';
    c.linha = atoi(str + 1) - 1;
    return c;
}

int eh_coordenada(const char *str) {
    return (str[0] >= 'a' && str[0] <= 'e') && isdigit(str[1]);
}

void printTabuleiro() {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            printf("%c ", estado_atual->tabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void casaaBranco(coordenada_t coord) {
    int i = coord.linha;
    int j = coord.coluna;

    if (i < 0 || i >= TAMANHO || j < 0 || j >= TAMANHO) return;
    if (estado_atual->tabuleiro[i][j] == '#') return;
    if (isupper(estado_atual->tabuleiro[i][j])) return;

    estado_atual->tabuleiro[i][j] = toupper(estado_atual->tabuleiro[i][j]);
}

void casaRiscada(coordenada_t coord) {
    int i = coord.linha;
    int j = coord.coluna;

    if (i < 0 || i >= TAMANHO || j < 0 || j >= TAMANHO) return;
    estado_atual->tabuleiro[i][j] = '#';
}

int linha_valida(int linha) {
    int contagem[26] = {0};
    for (int j = 0; j < TAMANHO; j++) {
        if (isupper(estado_atual->tabuleiro[linha][j])) {
            int idx = estado_atual->tabuleiro[linha][j] - 'A';
            contagem[idx]++;
            if (contagem[idx] > 1) {
                printf("Erro: Letra '%c' repetida na linha %d\n", estado_atual->tabuleiro[linha][j], linha + 1);
                return 0;
            }
        }
    }
    return 1;
}

int coluna_valida(int coluna) {
    int contagem[26] = {0};
    for (int i = 0; i < TAMANHO; i++) {
        if (isupper(estado_atual->tabuleiro[i][coluna])) {
            int idx = estado_atual->tabuleiro[i][coluna] - 'A';
            contagem[idx]++;
            if (contagem[idx] > 1) {
                printf("Erro: Letra '%c' repetida na coluna %d\n", estado_atual->tabuleiro[i][coluna], coluna + 1);
                return 0;
            }
        }
    }
    return 1;
}

// Função que verifica se uma casa branca está isolada
int casa_isolada(int x, int y) {
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (nx >= 0 && nx < TAMANHO && ny >= 0 && ny < TAMANHO) {
            if (isupper(estado_atual->tabuleiro[nx][ny])) {
                return 0; // tem vizinho
            }
        }
    }
    return 1; // isolado
}

int verificar_vitoria() {
    for (int i = 0; i < TAMANHO; i++) {
        if (!linha_valida(i)) return 0;
    }
    for (int j = 0; j < TAMANHO; j++) {
        if (!coluna_valida(j)) return 0;
    }
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            char c = estado_atual->tabuleiro[i][j];
            if (islower(c)) return 0;
            if (isupper(c) && casa_isolada(i, j)) return 0;
        }
    }
    return 1;
}

int main() {
    começar_jogo();
    char comando[100];

    while (1) {
        printTabuleiro();
        printf("> ");
        if (!fgets(comando, sizeof(comando), stdin)) break;
        
        comando[strcspn(comando, "\n")] = 0; // remover o \n

        if (strcmp(comando, "s") == 0) {
            break;
        } else if (strcmp(comando, "d") == 0) {
            desfazer();
        } else if (comando[0] == 'b' && comando[1] == ' ') {
            coordenada_t coord = parse_coord(comando + 2);
            casaaBranco(coord);
            salvar_estado();
        } else if (comando[0] == 'r' && comando[1] == ' ') {
            coordenada_t coord = parse_coord(comando + 2);
            casaRiscada(coord);
            salvar_estado();
        } else {
            printf("Comando desconhecido.\n");
        }

        if (verificar_vitoria()) {
            printTabuleiro();
            printf("Puzzle completo!\n");
            break;
        }
    }

    // Liberar memória
    while (estado_atual) {
        estado_t *temp = estado_atual;
        estado_atual = estado_atual->anterior;
        free(temp);
    }

    return 0;
}

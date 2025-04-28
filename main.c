#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO 5
#define MAXUNDO 100

typedef struct estado {
    char tabuleiro [TAMANHO][TAMANHO];
    struct estado *anterior;
} estado_t;

estado_t *estado_atual = NULL;

void salvar_estado() {
    estado_t *novo = malloc(sizeof(estado_t));
    memcpy(novo->tabuleiro, estado_atual->tabuleiro, sizeof(novo->tabuleiro));
    novo->anterior = estado_atual;
    estado_atual = novo;
}

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

    // Exemplo: preencher o tabuleiro com letras minúsculas
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            estado_atual->tabuleiro[i][j] = 'a' + (j % 26); // só como exemplo
        }
    }

    estado_atual->anterior = NULL;
}

void salvar_estado() {
    estado_t *novo = malloc(sizeof(estado_t));
    memcpy(novo->tabuleiro, estado_atual->tabuleiro, sizeof(novo->tabuleiro));
    novo->anterior = estado_atual;
    estado_atual = novo;
}

typedef struct {
    int linha;
    int coluna;
} coordenada_t;

void gravar_estado(const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "w");
    fclose(f);
}

void carregar_estado(const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "r");
    fclose(f);
}

coordenada_t parse_coord(const char *str) {
    coordenada_t c;
    c.coluna = str [0] - 'a';
    c.linha = atoi(str + 1) - 1;
    return c;
}

int eh_coordenada(const char *str){
    return (str[0] >= 'a' && str[0] <= 'e') && isdigit(str[1]);
}

int tabuleiro[TAMANHO][TAMANHO] = {
    {'e', 'c', 'a', 'd', 'c'},
    {'d', 'c', 'd', 'e', 'c'},
    {'b', 'd', 'd', 'c', 'e'},
    {'c', 'd', 'e', 'e', 'b'},
    {'a', 'c', 'c', 'b', 'b'}
};
char casasiniciais[] = {'a', 'b', 'c', 'd', 'e'};
char casas[] = {'a', 'b', 'c', 'd', 'e', 'A', 'B', 'C', 'D', 'E', '#'};


void printTabuleiro(){
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            printf("%c ", estado_atual->tabuleiro[i][j]);
        }
        printf("\n");
    }
}

void casaaBranco (int i, int j){
    if (i < 0 || i >= TAMANHO || j < 0 || j >= TAMANHO) return;
    if (tabuleiro[i][j] == 0) return;
    if (tabuleiro[i][j] == '#') return;
    if (tabuleiro[i][j] == 'A' || tabuleiro[i][j] == 'B' || tabuleiro[i][j] == 'C' || tabuleiro[i][j] == 'D' || tabuleiro[i][j] == 'E') return;
        else { 
            if (tabuleiro[i][j] == 'a') {
                tabuleiro[i][j] = 'A';
            }
            else if (tabuleiro[i][j] == 'b') {
                tabuleiro[i][j] = 'B';
            }
            else if (tabuleiro[i][j] == 'c') {
                tabuleiro[i][j] = 'C';
            }
            else if (tabuleiro[i][j] == 'd') {
                tabuleiro[i][j] = 'D';
            }
            else if (tabuleiro[i][j] == 'e') {
                tabuleiro[i][j] = 'E';
            }
        }
}

void casaRiscada (int i, int j){
    if (i < 0 || i >= TAMANHO || j < 0 || j >= TAMANHO) return;
    if (tabuleiro[i][j] == 0) return;
    if (tabuleiro[i][j] == '#') return;
        else {
            tabuleiro [i][j] = '#';
        }
}

void comandos(char *input) {
    if (input[0] == 's') {
        exit(0);
    } else if (input[0] == 'g' && input[1] == ' ') {
        gravar_estado(input + 2);
    } else if (input[0] == 'l' && input[1] == ' ') {
        ler_estado(input + 2);
    } else if (input[0] == 'b' && input[1] == ' ') {
        coordenada_t coord = parse_coord(input + 2);
        casaaBranco(coord);
    } else if (input[0] == 'r' && input[1] == ' ') {
        coordenada_t coord = parse_coord(input + 2);
        casaRiscada(coord);
    } else if (input[0] == 'v') {
        verificar_estado();
    } else if (input[0] == 'a') {
        ajuda();
    } else if (input[0] == 'A') {
        ajuda_iterativa();
    } else if (input[0] == 'R') {
        resolver_jogo();
    } else if (input[0] == 'd') {
        desfazer();
    } else if (eh_coordenada(input)) {
        coordenada_t coord = parse_coord(input);
        selecionar_coord(coord);
    } else {
        printf("Comando desconhecido.\n");
    }
}

int main () {
    começar_jogo();

    char comando[100];

    while (1){
        printTabuleiro();
        printf("> ");
        fgets(comando, sizeof(comando), stdin);

        if (comando[0] == 's') break;
        else if (comando[0] == 'd') desfazer();
        else if (comando[0] == 'b') {
            int linha = comando[2] - '1';       // ex: b 3  → linha 2
            int coluna = comando[1] - 'a';      // ex: b c  → coluna 2
            pintar_maiuscula(linha, coluna);
        }
    }

    return 0;
}

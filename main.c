#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO 5

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
    for (int i = 0; i < TAMANHO; i++){
        for (int j = 0; j < TAMANHO; j++){
            if (tabuleiro[i][j] == 0){
                printf(" ");
            }
            else printf ("%c", tabuleiro[i][j]);
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


int main () {
    char input;
    int i, j;
    int c = 0;
    while (1){
        printTabuleiro();
        c = scanf(" %c %d %d", &input , &i, &j);
        if (c == 1 && input == 's') break;
        if (c == 3) {
            if (input == 'b') casaaBranco(i, j);
            else 
                if (input == 'r') casaRiscada(i, j);
        }
        else {
            printf("Comando invalido\n");
        }
    }
    return 0;
}
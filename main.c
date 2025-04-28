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

    // Exemplo: preencher o tabuleiro com letras minúsculas
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            estado_atual->tabuleiro[i][j] = 'a' + (j % 26); // só como exemplo
        }
    }

    estado_atual->anterior = NULL;
}

typedef struct {
    int linha;
    int coluna;
} coordenada_t;

void gravar_jogo(const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "w");
    fclose(f);
}

void carregar_jogo(const char *nome_ficheiro) {
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

void casaaBranco(const char *str) {
    if (!eh_coordenada(str)) return;

    coordenada_t coord = parse_coord(str);
    int i = coord.linha;
    int j = coord.coluna;

    if (i < 0 || i >= TAMANHO || j < 0 || j >= TAMANHO) return;
    if (tabuleiro[i][j] == '#') return;
    if (tabuleiro[i][j] == 'A' || tabuleiro[i][j] == 'B' || tabuleiro[i][j] == 'C' || tabuleiro[i][j] == 'D' || tabuleiro[i][j] == 'E') return;

    switch (tabuleiro[i][j]) {
        case 'a': tabuleiro[i][j] = 'A'; break;
        case 'b': tabuleiro[i][j] = 'B'; break;
        case 'c': tabuleiro[i][j] = 'C'; break;
        case 'd': tabuleiro[i][j] = 'D'; break;
        case 'e': tabuleiro[i][j] = 'E'; break;
    }
}

void casaRiscada(coordenada_t coord) {
    int i = coord.linha;
    int j = coord.coluna;

    if (i < 0 || i >= TAMANHO || j < 0 || j >= TAMANHO) return;
    if (estado_atual->tabuleiro[i][j] == '#') return;

    estado_atual->tabuleiro[i][j] = '#';
}

int linha_valida(int linha) {
    int contagem[26] = {0};

    for (int j = 0; j < TAMANHO; j++) {
        if (isupper(tabuleiro[linha][j])) {
            int idx = tabuleiro[linha][j] - 'A';
            contagem[idx]++;
            if (contagem[idx] > 1) {
                printf("Erro: Letra '%c' repetida na linha %d\n", tabuleiro[linha][j], linha+1);
                return 0;
            }
        }
    }
    return 1;
}

int coluna_valida(int coluna) {
    int contagem[26] = {0};

    for (int i = 0; i < TAMANHO; i++) {
        if (isupper(tabuleiro[i][coluna])) {
            int idx = tabuleiro[i][coluna] - 'A';
            contagem[idx]++;
            if (contagem[idx] > 1) {
                printf("Erro: Letra '%c' repetida na coluna %d\n", tabuleiro[i][coluna], coluna+1);
                return 0;
            }
        }
    }
    return 1;
}

int caminho_ortogonal(int x, int y) {
    int brancos = 0;
    int dx[4] = { -1, 1, 0, 0 };
    int dy[4] = { 0, 0, -1, 1 };

    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (nx >= 0 && nx < TAMANHO && ny >= 0 && ny < TAMANHO) {
            if (isupper(tabuleiro[nx][ny])) {
                brancos++;
            }
        }
    }
    return brancos == 0;
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
            char c = tabuleiro[i][j];
            if (islower(c)) return 0;  // letra minúscula ainda presente
            if (isupper(c) && casa_isolada(i, j)) return 0;  // casa branca isolada
        }
    }

    return 1;
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
        if (verificar_vitoria()) {
            mostrar_tabuleiro();
            printf("Puzzle completo!\n");
            break;  // termina o programa
        }
    }
    return 0;
}

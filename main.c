#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAMANHO 5

typedef struct estado {
    char tabuleiro[TAMANHO][TAMANHO];
    struct estado *anterior;
} estado_t;

typedef struct {
    int linha;
    int coluna;
} coordenada_t;

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

void carregar_txt(const char *nome) {
    FILE *f = fopen(nome, "r");
    if (!f) {
        printf("Erro ao abrir ficheiro TXT.\n");
        return;
    }

    estado_t *novo = malloc(sizeof(estado_t));
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (fscanf(f, " %c", &novo->tabuleiro[i][j]) != 1) {
                printf("Erro ao ler o tabuleiro no ficheiro.\n");
                fclose(f);
                free(novo);
    return;
            }

        }
    }
    novo->anterior = NULL;
    if (estado_atual) free(estado_atual);
    estado_atual = novo;

    fclose(f);
}

void gravar_txt(const char *nome) {
    FILE *f = fopen(nome, "w");
    if (!f) {
        printf("Erro ao gravar ficheiro TXT.\n");
        return;
    }

    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            fprintf(f, "%c ", estado_atual->tabuleiro[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
}

coordenada_t parse_coord(const char *str) {
    coordenada_t c;
    c.linha = str[0] - 'a';
    c.coluna = atoi(str + 1) - 1;
    return c;
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

int verificar_repeticoes_letras_linhas() {
    for (int i = 0; i < TAMANHO; i++) {
        int contagem[26] = {0};
        for (int j = 0; j < TAMANHO; j++) {
            char c = estado_atual->tabuleiro[i][j];
            if (isupper(c)) {
                int idx = c - 'A';
                contagem[idx]++;
                if (contagem[idx] > 1) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int verificar_minisculas_linhas() {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            char c = estado_atual->tabuleiro[i][j];
            if (islower(c)) {
                return 0;
            }
        }
    }
    return 1;
}

int verificar_vizinhos_riscados_linhas() {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (estado_atual->tabuleiro[i][j] == '#') {
                int vizinho = 0;
                if (i > 0 && isupper(estado_atual->tabuleiro[i - 1][j])) vizinho = 1;
                if (i < TAMANHO - 1 && isupper(estado_atual->tabuleiro[i + 1][j])) vizinho = 1;
                if (j > 0 && isupper(estado_atual->tabuleiro[i][j - 1])) vizinho = 1;
                if (j < TAMANHO - 1 && isupper(estado_atual->tabuleiro[i][j + 1])) vizinho = 1;
                if (!vizinho) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int linhas_validas() {
    return verificar_repeticoes_letras_linhas() &&
           verificar_minisculas_linhas() &&
           verificar_vizinhos_riscados_linhas();
}

int verificar_repeticoes_letras_colunas() {
    for (int j = 0; j < TAMANHO; j++) {
        int contagem[26] = {0};
        for (int i = 0; i < TAMANHO; i++) {
            char c = estado_atual->tabuleiro[i][j];
            if (isupper(c)) {
                int idx = c - 'A';
                contagem[idx]++;
                if (contagem[idx] > 1) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int verificar_minisculas_colunas() {
    for (int j = 0; j < TAMANHO; j++) {
        for (int i = 0; i < TAMANHO; i++) {
            char c = estado_atual->tabuleiro[i][j];
            if (islower(c)) {
                return 0;
            }
        }
    }
    return 1;
}

int verificar_vizinhos_riscados_colunas() {
    for (int j = 0; j < TAMANHO; j++) {
        for (int i = 0; i < TAMANHO; i++) {
            if (estado_atual->tabuleiro[i][j] == '#') {
                int vizinho = 0;
                if (i > 0 && isupper(estado_atual->tabuleiro[i - 1][j])) vizinho = 1;
                if (i < TAMANHO - 1 && isupper(estado_atual->tabuleiro[i + 1][j])) vizinho = 1;
                if (j > 0 && isupper(estado_atual->tabuleiro[i][j - 1])) vizinho = 1;
                if (j < TAMANHO - 1 && isupper(estado_atual->tabuleiro[i][j + 1])) vizinho = 1;
                if (!vizinho) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int colunas_validas() {
    return verificar_repeticoes_letras_colunas() &&
           verificar_minisculas_colunas() &&
           verificar_vizinhos_riscados_colunas();
}


void dfs(int i, int j, int visitado[TAMANHO][TAMANHO], int *conectadas) {
    if (i < 0 || i >= TAMANHO || j < 0 || j >= TAMANHO) return;
    if (visitado[i][j]) return;
    if (!isupper(estado_atual->tabuleiro[i][j])) return;

    visitado[i][j] = 1;
    (*conectadas)++;

    dfs(i + 1, j, visitado, conectadas);
    dfs(i - 1, j, visitado, conectadas);
    dfs(i, j + 1, visitado, conectadas);
    dfs(i, j - 1, visitado, conectadas);
}

int todas_casas_conectadas() {
    int visitado[TAMANHO][TAMANHO] = {0};
    int total_brancas = 0;
    int conectadas = 0;

    int inicio_i = -1, inicio_j = -1;
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (isupper(estado_atual->tabuleiro[i][j])) {
                total_brancas++;
                if (inicio_i == -1) {
                    inicio_i = i;
                    inicio_j = j;
                }
            }
        }
    }

    if (total_brancas == 0) return 1;

    dfs(inicio_i, inicio_j, visitado, &conectadas);
    return conectadas == total_brancas;
}
int verificar_vitoria() {
    for (int i = 0; i < TAMANHO; i++) {
        if (!linhas_validas()) return 0; // Substituído por linhas_validas
    }
    for (int j = 0; j < TAMANHO; j++) {
        if (!colunas_validas()) return 0; // Substituído por colunas_validas
    }
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            char c = estado_atual->tabuleiro[i][j];
            if (islower(c)) return 0;
            if (!todas_casas_conectadas()) return 0;
        }
    }
    return 1;
}

void gravar_estado(const char *nome) {
    gravar_txt(nome);
}

void ler_estado(const char *nome) {
    carregar_txt(nome);
}

void começar_jogo() {
    estado_t *inicio = malloc(sizeof(estado_t));
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            inicio->tabuleiro[i][j] = 'a' + j;  
        }
    }
    inicio->anterior = NULL;
    estado_atual = inicio;
}

void ler_comandos_jogo(char *comando) {
    if (comando[0] == 's') {
        exit(0);
    } else if (comando[0] == 'd') {
        desfazer();
    } else if (comando[0] == 'v') {
        if (verificar_repeticoes_letras_colunas() && verificar_repeticoes_letras_linhas());
        else printf("Existem letras repitidas na mesma linha ou coluna! \n");
        if (verificar_minisculas_colunas() && verificar_minisculas_linhas());
        else printf("Existem letras minúsculas no tabuleiro! \n");
        if (verificar_vizinhos_riscados_colunas() && verificar_vizinhos_riscados_linhas());
        else printf("Existem casas riscadas vizinhas! \n");
        if (todas_casas_conectadas());
        else printf("Não existe caminho ortogonal entre todas as casas a Branco!\n");
    } else if (comando[0] == 'b' && comando[1] == ' ') {
        coordenada_t coord = parse_coord(comando + 2);
        salvar_estado();
        casaaBranco(coord);
    } else if (comando[0] == 'r' && comando[1] == ' ') {
        coordenada_t coord = parse_coord(comando + 2);
        salvar_estado();
        casaRiscada(coord);
    } else if (comando[0] == 'g' && comando[1] == ' ') {
        gravar_estado(comando + 2);
    } else if (comando[0] == 'l' && comando[1] == ' ') {
        ler_estado(comando + 2);
    } else if (comando[0] == 'r' && comando[1] == ' ') {
        resolver_jogo();
    } else {
        printf("Comando desconhecido.\n");
    }
}

int pode_colocar(int linha, int coluna, char letra) {
    if (isupper(estado_atual->tabuleiro[linha][coluna])) return 0;

    for (int j = 0; j < TAMANHO; j++) {
        if (estado_atual->tabuleiro[linha][j] == letra) return 0;
    }

    for (int i = 0; i < TAMANHO; i++) {
        if (estado_atual->tabuleiro[i][coluna] == letra) return 0;
    }

    return 1;
}

int resolver_recursivo() {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            char c = estado_atual->tabuleiro[i][j];
            if (islower(c)) {
                char letra_maiuscula = toupper(c);
                if (pode_colocar(i, j, letra_maiuscula)) {
                    estado_atual->tabuleiro[i][j] = letra_maiuscula;
                    if (resolver_recursivo()) {
                        return 1;
                    }
                    estado_atual->tabuleiro[i][j] = c;
                }
                return 0; 
            }
        }
    }

    return verificar_vitoria();
}

void resolver_jogo() {
    salvar_estado();
    if (resolver_recursivo()) {
        printf("Tabuleiro resolvido!\n");
    } else {
        printf("Não foi possível resolver o tabuleiro.\n");
    }
}

int main() {
    printf("Comandos : \n 's' - sair do jogo \n 'r' - riscar a coordenada \n 'b' - colocar a casa da coordenada Branca \n 'd' - voltar atrás \n 'v' - verificar as restrições \n \n");
    começar_jogo();
    
    char comando[100];

    while (1) {
        printTabuleiro();
        printf("> ");
        if (fgets(comando, sizeof(comando), stdin)) {
            comando[strcspn(comando, "\n")] = '\0';
            ler_comandos_jogo(comando);
        }

        comando[strcspn(comando, "\n")] = '\0';

        if (verificar_vitoria()) {
            printf("Puzzle completo!\n");
            break;
        }
    }

    return 0;
}

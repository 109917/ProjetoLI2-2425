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

void resolver_jogo(void);

void salvar_estado() {
    estado_t *novo = malloc(sizeof(estado_t));
    if (!novo) exit(EXIT_FAILURE);
    memcpy(novo->tabuleiro, estado_atual->tabuleiro, sizeof(novo->tabuleiro));
    novo->anterior = estado_atual;
    estado_atual = novo;
}

void desfazer() {
    if (estado_atual && estado_atual->anterior) {
        estado_t *temp = estado_atual;
        estado_atual = estado_atual->anterior;
        free(temp);
    } else {
        printf("Não há mais estados para desfazer.\n");
    }
}

void carregar_txt(const char *nome) {
    printf("Carregando jogo do ficheiro: %s\n", nome);
    FILE *f = fopen(nome, "r");
    if (!f) {
        printf("Erro ao abrir ficheiro TXT.\n");
        return;
    }

    estado_t *novo = malloc(sizeof(estado_t));
    if (!novo) exit(EXIT_FAILURE);

    int c, i = 0, j = 0;
    while ((c = fgetc(f)) != EOF){
        if (c == '\n') {
            i++;
            j = 0;
            continue;
        }
        novo->tabuleiro[i][j] = c;
        j++;
    }
        
    novo->anterior = NULL;
    if (estado_atual) free(estado_atual);
    estado_atual = novo;
    fclose(f);

    // FILE *f = fopen(nome, "r");
    // if (!f) {
    //     printf("Erro ao abrir ficheiro TXT.\n");
    //     return;
    // }

    // estado_t *novo = malloc(sizeof(estado_t));
    // if (!novo) exit(EXIT_FAILURE);

    // for (int i = 0; i < TAMANHO; i++) {
    //     for (int j = 0; j < TAMANHO; j++) {
    //         if (fscanf(f, " %c", &novo->tabuleiro[i][j]) != 1) {
    //             printf("Erro ao ler o tabuleiro no ficheiro.\n");
    //             fclose(f);
    //             free(novo);
    //             return;
    //         }
    //     }
    // }

    // novo->anterior = NULL;
    // if (estado_atual) free(estado_atual);
    // estado_atual = novo;
    // fclose(f);
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
    c.linha = tolower(str[0]) - 'a';
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
                if (contagem[idx] > 1) return 0;
            }
        }
    }
    return 1;
}

int verificar_minisculas_linhas() {
    for (int i = 0; i < TAMANHO; i++)
        for (int j = 0; j < TAMANHO; j++)
            if (islower(estado_atual->tabuleiro[i][j])) return 0;
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
                if (!vizinho) return 0;
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
                if (contagem[idx] > 1) return 0;
            }
        }
    }
    return 1;
}

int verificar_minisculas_colunas() {
    for (int j = 0; j < TAMANHO; j++)
        for (int i = 0; i < TAMANHO; i++)
            if (islower(estado_atual->tabuleiro[i][j])) return 0;
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
                if (!vizinho) return 0;
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
    int visitado[TAMANHO][TAMANHO] = {{0}};
    int total = 0, conectadas = 0;
    int i0 = -1, j0 = -1;

    for (int i = 0; i < TAMANHO; i++)
        for (int j = 0; j < TAMANHO; j++)
            if (isupper(estado_atual->tabuleiro[i][j])) {
                total++;
                if (i0 == -1) { i0 = i; j0 = j; }
            }

    if (total == 0) return 1;

    dfs(i0, j0, visitado, &conectadas);
    return conectadas == total;
}

int verificar_vitoria() {
    return linhas_validas() &&
           colunas_validas() &&
           todas_casas_conectadas();
}

int pode_colocar(int linha, int coluna, char letra) {
    if (estado_atual->tabuleiro[linha][coluna] == '#') return 0;
    if (isupper(estado_atual->tabuleiro[linha][coluna])) return 0;

    for (int j = 0; j < TAMANHO; j++)
        if (estado_atual->tabuleiro[linha][j] == letra) return 0;

    for (int i = 0; i < TAMANHO; i++)
        if (estado_atual->tabuleiro[i][coluna] == letra) return 0;

    return 1;
}


int aplicar_restricao_repeticoes_linhas() {
    for (int i = 0; i < TAMANHO; i++) {
        int contagem[26] = {0};
        for (int j = 0; j < TAMANHO; j++) {
            char c = estado_atual->tabuleiro[i][j];
            if (isupper(c)) {
                int idx = c - 'A';
                contagem[idx]++;
                if (contagem[idx] > 1) {
                    estado_atual->tabuleiro[i][j] = '#'; 
                    return 1; 
                }
            }
        }
    }
    return 0; 
}

int aplicar_restricao_minisculas_linhas() {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (islower(estado_atual->tabuleiro[i][j])) {
                estado_atual->tabuleiro[i][j] = toupper(estado_atual->tabuleiro[i][j]);
                return 1; 
            }
        }
    }
    return 0; 
}


int aplicar_restricao_vizinhos_riscados_linhas() {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (estado_atual->tabuleiro[i][j] == '#') {
                int vizinho = 0;
                if (i > 0 && isupper(estado_atual->tabuleiro[i - 1][j])) vizinho = 1;
                if (i < TAMANHO - 1 && isupper(estado_atual->tabuleiro[i + 1][j])) vizinho = 1;
                if (j > 0 && isupper(estado_atual->tabuleiro[i][j - 1])) vizinho = 1;
                if (j < TAMANHO - 1 && isupper(estado_atual->tabuleiro[i][j + 1])) vizinho = 1;
                if (!vizinho) {
                    estado_atual->tabuleiro[i][j] = '#';
                    return 1;
                }
            }
        }
    }
    return 0;
}


int aplicar_restricao_repeticoes_colunas() {
    for (int j = 0; j < TAMANHO; j++) {
        int contagem[26] = {0};
        for (int i = 0; i < TAMANHO; i++) {
            char c = estado_atual->tabuleiro[i][j];
            if (isupper(c)) {
                int idx = c - 'A';
                contagem[idx]++;
                if (contagem[idx] > 1) {
                    estado_atual->tabuleiro[i][j] = '#'; 
                    return 1;
                }
            }
        }
    }
    return 0; 
}

int aplicar_restricao_minisculas_colunas() {
    for (int j = 0; j < TAMANHO; j++) {
        for (int i = 0; i < TAMANHO; i++) {
            if (islower(estado_atual->tabuleiro[i][j])) {
                estado_atual->tabuleiro[i][j] = toupper(estado_atual->tabuleiro[i][j]);
                return 1; 
            }
        }
    }
    return 0; 
}

int aplicar_restricao_vizinhos_riscados_colunas() {
    for (int j = 0; j < TAMANHO; j++) {
        for (int i = 0; i < TAMANHO; i++) {
            if (estado_atual->tabuleiro[i][j] == '#') {
                int vizinho = 0;
                if (i > 0 && isupper(estado_atual->tabuleiro[i - 1][j])) vizinho = 1;
                if (i < TAMANHO - 1 && isupper(estado_atual->tabuleiro[i + 1][j])) vizinho = 1;
                if (j > 0 && isupper(estado_atual->tabuleiro[i][j - 1])) vizinho = 1;
                if (j < TAMANHO - 1 && isupper(estado_atual->tabuleiro[i][j + 1])) vizinho = 1;
                if (!vizinho) {
                    estado_atual->tabuleiro[i][j] = '#';
                    return 1; 
                }
            }
        }
    }
    return 0; 
}
int aplicar_restricao_riscadas_vizinhas() {
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            if (estado_atual->tabuleiro[i][j] == '#') {
                if ((i > 0 && estado_atual->tabuleiro[i - 1][j] == '#') ||
                    (i < TAMANHO - 1 && estado_atual->tabuleiro[i + 1][j] == '#') ||
                    (j > 0 && estado_atual->tabuleiro[i][j - 1] == '#') ||
                    (j < TAMANHO - 1 && estado_atual->tabuleiro[i][j + 1] == '#')) {
                    return 0; 
                }
            }
        }
    }
    return 0;
}

int aplicar_primeira_restricao() {
    if (aplicar_restricao_minisculas_colunas()) return 1;
    if (aplicar_restricao_minisculas_linhas()) return 1;
    if (aplicar_restricao_repeticoes_linhas()) return 1;
    if (aplicar_restricao_repeticoes_colunas()) return 1;
    if (aplicar_restricao_riscadas_vizinhas()) return 1; 
    if (aplicar_restricao_vizinhos_riscados_linhas()) return 1;
    if (aplicar_restricao_vizinhos_riscados_colunas()) return 1;
    salvar_estado();
    return 0;
}

void resolver_com_restricoes() {
    int alterado;

    do {
        salvar_estado();
        alterado = aplicar_primeira_restricao();
    } while (alterado);
}
void começar_jogo() {
    estado_t *inicio = malloc(sizeof(estado_t));
    if (!inicio) exit(EXIT_FAILURE);
    for (int i = 0; i < TAMANHO; i++)
        for (int j = 0; j < TAMANHO; j++)
            inicio->tabuleiro[i][j] = 'a' + j;
    inicio->anterior = NULL;
    estado_atual = inicio;
}

void ler_comandos_jogo(char *comando) {
    if (comando[0] == 's') {
        exit(0);
    } else if (comando[0] == 'd') {
        desfazer();
    } else if (comando[0] == 'a'){
        salvar_estado();
        if (aplicar_primeira_restricao()){
            printf("Dica aplicada \n");
        } else printf("Nenhuma dica detetada \n");
    } else if (comando[0] == 'A') {
        resolver_com_restricoes();
        printf("Não há mais dicas aplicaveis\n");
    } else if (comando[0] == 'v') {
        if (!verificar_repeticoes_letras_colunas() || !verificar_repeticoes_letras_linhas())
            printf("Existem letras repetidas na mesma linha ou coluna!\n");
        if (!verificar_minisculas_colunas() || !verificar_minisculas_linhas())
            printf("Existem letras minúsculas no tabuleiro!\n");
        if (!verificar_vizinhos_riscados_colunas() || !verificar_vizinhos_riscados_linhas())
            printf("Existem casas riscadas sem vizinhos brancos!\n");
        if (!todas_casas_conectadas())
            printf("Não existe caminho ortogonal entre todas as casas a Branco!\n");
    } else if (strncmp(comando, "b ", 2) == 0) {
        salvar_estado();
        casaaBranco(parse_coord(comando + 2));
    } else if (strncmp(comando, "r ", 2) == 0) {
        salvar_estado();
        casaRiscada(parse_coord(comando + 2));
    } else if (strncmp(comando, "g ", 2) == 0) {
        gravar_txt(comando + 2);
    } else if (strncmp(comando, "l ", 2) == 0) {
        carregar_txt(comando + 2);
    } //else if (strcmp(comando, "R") == 0) {
        //resolver_jogo();
    //} 
    else {
        printf("Comando desconhecido.\n");
    }
}

int main() {
    printf("Comandos :\n 's' - sair do jogo\n 'r' - riscar coordenada\n 'b' - casa Branca\n 'd' - desfazer\n 'v' - verificar restrições\n 'r' - resolver\n 'g' <jogo.txt> - gravar o jogo no ficheiro jogo.txt \n 'l' <jogo.txt> - ler o jogo gravado no ficheiro jogo.txt \n \n");
    começar_jogo();

    char comando[100];

    while (1) {
        printTabuleiro();
        printf("> ");
        if (fgets(comando, sizeof(comando), stdin)) {
            comando[strcspn(comando, "\n")] = '\0';
            ler_comandos_jogo(comando);
            if (verificar_vitoria()) {
                printTabuleiro();
                printf("Puzzle completo!\n");
                break;
            }
        }
    }

    return 0;
}

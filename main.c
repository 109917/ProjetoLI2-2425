#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct estado
{
 //char tabuleiro[TAMANHO][TAMANHO];
    char **tabuleiro;
    int linhas;
    int colunas;
    struct estado *anterior;
} estado_t;

typedef struct
{
    int linha; 
    int coluna;
} coordenada_t;

estado_t *estado_atual;

void resolver_jogo(void);

void salvar_estado() {
    if (!estado_atual || !estado_atual->tabuleiro) {
        return;
    } 
    estado_t *novo = malloc(sizeof(estado_t));
    if (!novo)
        exit(EXIT_FAILURE);

    novo->linhas = estado_atual->linhas;
    novo->colunas = estado_atual->colunas;

    // reservar memoria para o novo tabuleiro
    novo->tabuleiro = malloc(novo->linhas * sizeof(char *));
    for (int i = 0; i < novo->linhas; i++) {
        novo->tabuleiro[i] = malloc(novo->colunas * sizeof(char));
        if (estado_atual->tabuleiro[i]) // proteção extra
            memcpy(novo->tabuleiro[i], estado_atual->tabuleiro[i], novo->colunas * sizeof(char));
        else
            memset(novo->tabuleiro[i], 0, novo->colunas * sizeof(char));
    }

    novo->anterior = estado_atual;
    estado_atual = novo;
}

void desfazer() {
    if (estado_atual && estado_atual->anterior) {
        estado_t *temp = estado_atual;
        estado_atual = estado_atual->anterior;
        for (int i = 0; i < temp->linhas; i++) {
            free(temp->tabuleiro[i]);
        }
        free(temp->tabuleiro);
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
    if (!novo){
        exit(EXIT_FAILURE);
    }

    if (fscanf(f, "%d %d", &novo->linhas, &novo->colunas) != 2) {
        printf("Erro ao ler dimensões do tabuleiro.\n");
        fclose(f);
        free(novo);
        return;
    }

    novo->tabuleiro = malloc(novo->linhas * sizeof(char *));
    for (int i = 0; i < novo->linhas; i++) {
        novo->tabuleiro[i] = malloc(novo->colunas * sizeof(char));
    }
    int c;
    // ler até fim de linha
    while ((c = fgetc(f)) != EOF && c != '\n') {
        // ciclo vazio propositado
    }
    int linha = 0, coluna = 0;
    while ((c = fgetc(f)) != EOF && linha < novo->linhas){
        if (c == '\n'){
            // fim de linha
            continue;
        }
        novo->tabuleiro[linha][coluna++] = (char)c;
        if (coluna == novo->colunas){
            coluna = 0;
            linha++;
        }
    }

    novo->anterior = NULL;
    if (estado_atual) {
        for (int i = 0; i < estado_atual->linhas; i++) {
            free(estado_atual->tabuleiro[i]);
        }
        free(estado_atual->tabuleiro);
        free(estado_atual);
    }
    estado_atual = novo;
    fclose(f);
    printf("Jogo carregado com sucesso!\n");
}

void gravar_txt(const char *nome) {
    if(!estado_atual){
        printf("Sem jogo para gravar.\n");
        return;
    }

     printf("Gravando jogo no ficheiro: %s\n", nome);
    FILE *f = fopen(nome, "w");
    if (!f) {
        printf("Erro ao gravar ficheiro TXT.\n");
        return;
    }

 // escrever primeira linha com numero de linhas e colunas
    fprintf(f, "%d %d\n", estado_atual->linhas, estado_atual->colunas);

    for (int i = 0; i < estado_atual->linhas; i++){
        for (int j = 0; j < estado_atual->colunas; j++){
            fputc(estado_atual->tabuleiro[i][j], f);
        }
        if(i < estado_atual->linhas){
            fputc('\n', f);
        }
    }

    fclose(f);
    printf("Jogo gravado com sucesso!\n");
}

coordenada_t parse_coord(const char *str){
    coordenada_t c;
    c.linha = tolower(str[0]) - 'a';
    c.coluna = atoi(str + 1) - 1;
    return c;
}

void printTabuleiro() {
    for (int i = 0; i < estado_atual->linhas; i++){
        for (int j = 0; j < estado_atual->colunas; j++){
            printf("%c ", estado_atual->tabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void casaaBranco(coordenada_t coord){
    int i = coord.linha;
    int j = coord.coluna;

    if (i < 0 || i >= estado_atual->linhas || j < 0 || j >= estado_atual->colunas) return;
    if (estado_atual->tabuleiro[i][j] == '#') return;
    if (isupper(estado_atual->tabuleiro[i][j])) return;

    estado_atual->tabuleiro[i][j] = toupper(estado_atual->tabuleiro[i][j]);
}

void casaRiscada(coordenada_t coord){
    int i = coord.linha;
    int j = coord.coluna;

    if (i < 0 || i >= estado_atual->linhas || j < 0 || j >= estado_atual->colunas) return;
    estado_atual->tabuleiro[i][j] = '#';
}

int verificar_minisculas() {
    for (int j = 0; j < estado_atual->colunas; j++) {
        for (int i = 0; i < estado_atual->linhas; i++) {
            if (islower(estado_atual->tabuleiro[i][j])) {
                return 0;
            }
        }
    }
    return 1;
}

int verificar_vizinhos_riscados_casa(int i, int j) {
    if (estado_atual->tabuleiro[i][j] != '#' || estado_atual->tabuleiro[i][j] == '#'){
        int vizinho = 0;
        if (i > 0 && (estado_atual->tabuleiro[i - 1][j] == '#')) vizinho++;
        if (i < estado_atual->linhas - 1 && (estado_atual->tabuleiro[i + 1][j] == '#')) vizinho++;
        if (j > 0 && (estado_atual->tabuleiro[i][j - 1] == '#')) vizinho++;
        if (j < estado_atual->colunas - 1 && (estado_atual->tabuleiro[i][j + 1] == '#')) vizinho++;
        if (vizinho > 0) return 0;
    }
    return 1;
}

int verificar_vizinhos_riscados() {
    for (int j = 0; j < estado_atual->colunas; j++) {
        for (int i = 0; i < estado_atual->linhas; i++) {
            if (verificar_vizinhos_riscados_casa(i, j)) continue;
            else return 0;
        }
    }
    return 1;
}


int verificar_repeticoes_letras_linhas(){
    for (int i = 0; i < estado_atual->linhas; i++){
        int contagem[26] = {0};
        for (int j = 0; j < estado_atual->colunas; j++){
            char c = estado_atual->tabuleiro[i][j];
            if (isupper(c)){
                int idx = c - 'A';
                contagem[idx]++;
                if (contagem[idx] > 1) return 0;
            }
        }
    }
    return 1;
}

int linhas_validas() {
    return verificar_repeticoes_letras_linhas() &&
    verificar_minisculas() &&
    verificar_vizinhos_riscados();
}

int verificar_repeticoes_letras_colunas(){
    for (int j = 0; j < estado_atual->colunas; j++) {
        int contagem[26] = {0};
        for (int i = 0; i < estado_atual->linhas; i++) {
            char c = estado_atual->tabuleiro[i][j];
            if (isupper(c)){
                int idx = c - 'A';
                contagem[idx]++;
                if (contagem[idx] > 1)
                return 0;
            }
        }
    }
    return 1;
}



int colunas_validas(){
    return verificar_repeticoes_letras_colunas() &&
    verificar_minisculas() &&
    verificar_vizinhos_riscados();
}

void dfs(int i, int j, int **visitado, int *conectadas){
    if (i < 0 || i >= estado_atual->linhas || j < 0 || j >= estado_atual->colunas) return;
    if (visitado[i][j]) return;
    if (!isupper(estado_atual->tabuleiro[i][j]))return;

    visitado[i][j] = 1;
    (*conectadas)++;

    dfs(i + 1, j, visitado, conectadas);
    dfs(i - 1, j, visitado, conectadas);
    dfs(i, j + 1, visitado, conectadas);
    dfs(i, j - 1, visitado, conectadas);
}

int todas_casas_conectadas(){
    // int visitado[TAMANHO][TAMANHO] = {{0}};
    int total = 0, conectadas = 0;
    int i0 = -1, j0 = -1;

    int **visitado;

    // reservar memoria para as linhas e colunas
    visitado = malloc(estado_atual->linhas * sizeof(int *));
    for (int i = 0; i < estado_atual->linhas; i++) {
        visitado[i] = malloc(estado_atual->colunas * sizeof(int));
    }

    // colocar todas as posições com 0
    for (int i = 0; i < estado_atual->linhas; i++) {
        for (int j = 0; j < estado_atual->colunas; j++) {
        visitado[i][j] = 0;
        }
    }

    for (int i = 0; i < estado_atual->linhas; i++){
        for (int j = 0; j < estado_atual->colunas; j++)
            if (isupper(estado_atual->tabuleiro[i][j])){
                total++;
                if (i0 == -1){
                    i0 = i;
                    j0 = j;
                }
            }

        if (total == 0) {
            for (int i = 0; i < estado_atual->linhas; i++)
                free(visitado[i]);
            free(visitado);
            return 1;
        }
        dfs(i0, j0, visitado, &conectadas);
    }

    for (int i = 0; i < estado_atual->linhas; i++)
        free(visitado[i]);
    free(visitado);

    return conectadas == total;
}

int verificar_vitoria() {
    return estado_atual &&
    linhas_validas() &&
    colunas_validas() &&
    todas_casas_conectadas();
}

int aplicar_restricao_repeticoes_linhas(){
    for (int i = 0; i < estado_atual->linhas; i++){
        int contagem[26] = {0};
        for (int j = 0; j < estado_atual->colunas; j++) {
            char c = estado_atual->tabuleiro[i][j];
            if (isupper(c)) {
                int idx = c - 'A';
                contagem[idx]++;
                if (contagem[idx] > 1 ) {
                    return 0;  // Letra repetida => inválido
                }
            }
        }
    }
    return 1;  // Nenhuma repetição
}


int aplicar_restricao_minisculas(){
    for (int i = 0; i < estado_atual->linhas; i++) {
        for (int j = 0; j < estado_atual->colunas; j++) {
            if (islower(estado_atual->tabuleiro[i][j])) {
                estado_atual->tabuleiro[i][j] = toupper(estado_atual->tabuleiro[i][j]);
                return 1;
            }
        }
    }
    return 0;
}

int aplicar_restricao_vizinhos_riscados() {
    for (int i = 0; i < estado_atual->linhas; i++) {
        for (int j = 0; j < estado_atual->colunas; j++) {
            if (estado_atual->tabuleiro[i][j] == '#') continue;
            if (verificar_vizinhos_riscados_casa(i, j)){
            estado_atual->tabuleiro[i][j] = '#';
            return 1;
            }
        }
    }
    return 0;
}

int aplicar_restricao_repeticoes_colunas() {
    for (int j = 0; j < estado_atual->colunas; j++) {
        int contagem[26] = {0};
        for (int i = 0; i < estado_atual->linhas; i++) {
            char c = estado_atual->tabuleiro[i][j];
            if (isupper(c)) {
                int idx = c - 'A';
                contagem[idx]++;
                if (contagem[idx] > 1 ) {
                    return 0;  // Letra repetida => inválido
                }
            }
        }
    }
    return 1;  // Nenhuma repetição
}



int aplicar_primeira_restricao(){
    if (aplicar_restricao_minisculas()) return 1;
    if (aplicar_restricao_repeticoes_colunas()) aplicar_restricao_vizinhos_riscados();
    if (aplicar_restricao_repeticoes_linhas()) aplicar_restricao_vizinhos_riscados();
    if (aplicar_restricao_vizinhos_riscados()) return 1;


    salvar_estado();
    return 0;
}

void resolver_com_restricoes() {
    int alterado;
    int aux = estado_atual->linhas * estado_atual->colunas;

    do {
        alterado = 1;

        for (int i = 0; i < estado_atual->linhas; i++) {
            for (int j = 0; j < estado_atual->colunas; j++) {
                printf("%d %d \n", i, j);
                aplicar_primeira_restricao();

            }
        }
        aux--;
        if (aux < 0) alterado = 0;
    } while (alterado);


}



void comecar_jogo()
{
    estado_t *inicio = malloc(sizeof(estado_t));
    if (!inicio){
        free(inicio);
        exit(EXIT_FAILURE);
    }
        
    inicio->linhas = 5; 
    inicio->colunas = 5;
    inicio->tabuleiro = malloc(inicio->linhas * sizeof(char *));
    for (int i = 0; i < inicio->linhas; i++) {
        inicio->tabuleiro[i] = malloc(inicio->colunas * sizeof(char));
        for (int j = 0; j < inicio->colunas; j++){
            inicio->tabuleiro[i][j] = 'a' + j; 
        }
    }
    inicio->anterior = NULL;
    estado_atual = inicio;
}

void ler_comandos_jogo(char *comando){
    if (comando[0] == 's') {
        exit(0);
    } else if (comando[0] == 'd') {
        desfazer();
    } else if (comando[0] == 'a') {
        salvar_estado();
        if (aplicar_primeira_restricao()) {
            printf("Dica aplicada \n");
        }
        else
        printf("Nenhuma dica detetada \n");
    } else if (comando[0] == 'A') {
        resolver_com_restricoes();
        printf("Não há mais dicas aplicaveis\n");
    } else if (comando[0] == 'v') {
        if (!verificar_repeticoes_letras_colunas() || !verificar_repeticoes_letras_linhas()) printf("Existem letras repetidas na mesma linha ou coluna!\n");
        if (!verificar_minisculas()) printf("Existem letras minúsculas no tabuleiro!\n");
        if (!verificar_vizinhos_riscados()) printf("Existem casas riscadas sem vizinhos brancos!\n");
        if (!todas_casas_conectadas()) printf("Não existe caminho ortogonal entre todas as casas a Branco!\n");
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
    } 
 // else if (strcmp(comando, "R") == 0) {
 // resolver_jogo();
 // }
    else {
    printf("Comando desconhecido.\n");
    }
}

#ifndef TEST_BUILD

int main() {
    printf("Comandos :\n 's' - sair do jogo\n 'r' - riscar coordenada\n 'b' - casa Branca\n 'd' - desfazer\n 'v' - verificar restrições\n 'r' - resolver\n 'g' <jogo.txt> - gravar o jogo no ficheiro jogo.txt \n 'l' <jogo.txt> - ler o jogo gravado no ficheiro jogo.txt \n \n");
    comecar_jogo();

    char comando[100];

    while (1) {
 //printTabuleiro();
        printf("> ");
        if (fgets(comando, sizeof(comando), stdin)) {
            comando[strcspn(comando, "\n")] = '\0';
            ler_comandos_jogo(comando);
            printTabuleiro();
            if (verificar_vitoria()) {
                printf("Puzzle completo!\n");
                break;
            }
        }
    }

    return 0;
}
#endif

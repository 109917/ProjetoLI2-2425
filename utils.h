#ifndef JOGO
#define JOGO
#define TAMANHO 5
typedef struct {
    int linha;
    int coluna;
} coordenada_t;

typedef struct estado {
    char tabuleiro[TAMANHO][TAMANHO];
    struct estado *anterior;
} estado_t;


extern estado_t *estado_atual;

void começar_jogo();
void salvar_estado();
void desfazer();
void printTabuleiro();
coordenada_t parse_coord(const char *str);
int eh_coordenada(const char *str);
void casaaBranco(coordenada_t coord);
void casaRiscada(coordenada_t coord);
int linha_valida(int linha);
int coluna_valida(int coluna);
int casa_isolada(int linha, int coluna);
int verificar_vitoria();
void comandos(char *input);
void gravar_jogo(const char *nome_ficheiro);
void carregar_jogo(const char *nome_ficheiro);
void selecionar_coord(coordenada_t coord);

#endif

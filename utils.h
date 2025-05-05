#ifndef UTILS_H
#define UTILS_H

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

void salvar_estado();
void desfazer();
void carregar_txt(const char *nome);
void gravar_txt(const char *nome);
coordenada_t parse_coord(const char *str);
void printTabuleiro();
void casaaBranco(coordenada_t coord);
void casaRiscada(coordenada_t coord);
int verificar_repeticoes_letras_linhas();
int verificar_minisculas_linhas();
int verificar_vizinhos_riscados_linhas();
int linhas_validas();
int verificar_repeticoes_letras_colunas();
int verificar_minisculas_colunas();
int verificar_vizinhos_riscados_colunas();
int colunas_validas();
void dfs(int i, int j, int visitado[TAMANHO][TAMANHO], int *conectadas);
int todas_casas_conectadas();
int verificar_vitoria();
int pode_colocar(int linha, int coluna, char letra);
int aplicar_restricao_repeticoes_linhas();
int aplicar_restricao_minisculas_linhas();
int aplicar_restricao_vizinhos_riscados_linhas();
int aplicar_restricao_repeticoes_colunas();
int aplicar_restricao_minisculas_colunas();
int aplicar_restricao_vizinhos_riscados_colunas();
int aplicar_restricao_riscadas_vizinhas();
int aplicar_primeira_restricao();
void resolver_com_restricoes();
void começar_jogo();
void ler_comandos_jogo(char *comando);

#endif 
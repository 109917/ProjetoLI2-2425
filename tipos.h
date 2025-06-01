#include <ctype.h>

#ifndef TIPOS_H
#define TIPOS_H

typedef struct estado
{
    char **tabuleiro;
    int linhas;
    int colunas;
    struct estado *anterior;
} estado_t;


typedef struct {
    int linha;
    int coluna;
} coordenada_t;

#endif
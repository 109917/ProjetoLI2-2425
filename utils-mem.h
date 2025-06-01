#include <stdlib.h>
#include "tipos.h"


void libertar_estado(estado_t *estado) {
    while (estado) {
        estado_t *anterior = estado->anterior;
        for (int i = 0; i < estado->linhas; i++) {
            free(estado->tabuleiro[i]);
        }
        free(estado->tabuleiro);
        free(estado);
        estado = anterior;
    }
}
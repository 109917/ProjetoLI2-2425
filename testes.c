#include <CUnit/Basic.h>
#include "utils.h"

void test_parse_coord() {
    coordenada_t c = parse_coord("a1");
    CU_ASSERT_EQUAL(c.coluna, 0);
    CU_ASSERT_EQUAL(c.linha, 0);

    c = parse_coord("e5");
    CU_ASSERT_EQUAL(c.coluna, 4);
    CU_ASSERT_EQUAL(c.linha, 4);
}

void test_eh_coordenada() {
    CU_ASSERT_TRUE(eh_coordenada("a1"));
    CU_ASSERT_TRUE(eh_coordenada("e5"));
    CU_ASSERT_FALSE(eh_coordenada("f1"));
    CU_ASSERT_FALSE(eh_coordenada("a0"));
    CU_ASSERT_FALSE(eh_coordenada("aa"));
}

void test_linha_valida() {
    começar_jogo();
    estado_atual->tabuleiro[0][0] = 'A';
    estado_atual->tabuleiro[0][1] = 'B';
    estado_atual->tabuleiro[0][2] = 'C';
    estado_atual->tabuleiro[0][3] = 'D';
    estado_atual->tabuleiro[0][4] = 'E';

    CU_ASSERT_TRUE(linha_valida(0));

    estado_atual->tabuleiro[0][3] = 'A'; 
    CU_ASSERT_FALSE(linha_valida(0));
}

void test_coluna_valida() {
    começar_jogo();
    estado_atual->tabuleiro[0][0] = 'A';
    estado_atual->tabuleiro[1][0] = 'B';
    estado_atual->tabuleiro[2][0] = 'C';
    estado_atual->tabuleiro[3][0] = 'D';
    estado_atual->tabuleiro[4][0] = 'E';

    CU_ASSERT_TRUE(coluna_valida(0));

    estado_atual->tabuleiro[3][0] = 'A'; 
    CU_ASSERT_FALSE(coluna_valida(0));
}

void test_casa_isolada() {
    começar_jogo();
    for (int i = 0; i < TAMANHO; i++) 
        for (int j = 0; j < TAMANHO; j++) 
            estado_atual->tabuleiro[i][j] = '#';

    estado_atual->tabuleiro[2][2] = 'A';
    CU_ASSERT_TRUE(casa_isolada(2, 2));

    estado_atual->tabuleiro[2][3] = 'B'; 
    CU_ASSERT_FALSE(casa_isolada(2, 2));
}

void test_verificar_vitoria() {
    começar_jogo();
    char letras[] = { 'A', 'B', 'C', 'D', 'E' };

    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            estado_atual->tabuleiro[i][j] = letras[j];
        }
    }

    CU_ASSERT_TRUE(verificar_vitoria());

    estado_atual->tabuleiro[1][1] = 'b';
    CU_ASSERT_FALSE(verificar_vitoria());
}

int main() {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Suite de Testes", NULL, NULL);

    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(pSuite, "Teste parse_coord", test_parse_coord);
    CU_add_test(pSuite, "Teste eh_coordenada", test_eh_coordenada);
    CU_add_test(pSuite, "Teste linha_valida", test_linha_valida);
    CU_add_test(pSuite, "Teste coluna_valida", test_coluna_valida);
    CU_add_test(pSuite, "Teste casa_isolada", test_casa_isolada);
    CU_add_test(pSuite, "Teste verificar_vitoria", test_verificar_vitoria);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

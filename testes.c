#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "utils.h"

// Declare the estado_atual variable
extern estado_t *estado_atual;

void test_salvar_estado() {
    começar_jogo();
    salvar_estado();
    CU_ASSERT_PTR_NOT_NULL(estado_atual->anterior);
}

void test_desfazer() {
    começar_jogo();
    salvar_estado();
    desfazer();
    CU_ASSERT_PTR_NULL(estado_atual->anterior);
}

void test_carregar_txt() {
    começar_jogo();
    gravar_txt("test.txt");
    carregar_txt("test.txt");
    CU_ASSERT_PTR_NOT_NULL(estado_atual);
}

void test_gravar_txt() {
    começar_jogo();
    gravar_txt("test.txt");
    FILE *f = fopen("test.txt", "r");
    CU_ASSERT_PTR_NOT_NULL(f);
    if (f) fclose(f);
}

void test_parse_coord() {
    coordenada_t coord = parse_coord("a1");
    CU_ASSERT_EQUAL(coord.linha, 0);
    CU_ASSERT_EQUAL(coord.coluna, 0);
}

void test_casaaBranco() {
    começar_jogo();
    coordenada_t coord = {0, 0};
    casaaBranco(coord);
    CU_ASSERT_TRUE(isupper(estado_atual->tabuleiro[0][0]));
}

void test_casaRiscada() {
    começar_jogo();
    coordenada_t coord = {0, 0};
    casaRiscada(coord);
    CU_ASSERT_EQUAL(estado_atual->tabuleiro[0][0], '#');
}

void test_verificar_repeticoes_letras_linhas() {
    começar_jogo();
    CU_ASSERT_TRUE(verificar_repeticoes_letras_linhas());
}

void test_verificar_minisculas_linhas() {
    começar_jogo();
    CU_ASSERT_FALSE(verificar_minisculas_linhas());
}

void test_verificar_vizinhos_riscados_linhas() {
    começar_jogo();
    CU_ASSERT_TRUE(verificar_vizinhos_riscados_linhas());
}

void test_linhas_validas() {
    começar_jogo();
    CU_ASSERT_TRUE(linhas_validas());
}

void test_verificar_repeticoes_letras_colunas() {
    começar_jogo();
    CU_ASSERT_TRUE(verificar_repeticoes_letras_colunas());
}

void test_verificar_minisculas_colunas() {
    começar_jogo();
    CU_ASSERT_FALSE(verificar_minisculas_colunas());
}

void test_verificar_vizinhos_riscados_colunas() {
    começar_jogo();
    CU_ASSERT_TRUE(verificar_vizinhos_riscados_colunas());
}

void test_colunas_validas() {
    começar_jogo();
    CU_ASSERT_TRUE(colunas_validas());
}

void test_todas_casas_conectadas() {
    começar_jogo();
    CU_ASSERT_TRUE(todas_casas_conectadas());
}

void test_verificar_vitoria() {
    começar_jogo();
    CU_ASSERT_FALSE(verificar_vitoria());
}

void test_pode_colocar() {
    começar_jogo();
    CU_ASSERT_TRUE(pode_colocar(0, 0, 'A'));
    CU_ASSERT_FALSE(pode_colocar(0, 0, '#'));
}

void test_aplicar_primeira_restricao() {
    começar_jogo();
    CU_ASSERT_TRUE(aplicar_primeira_restricao());
}

void test_resolver_com_restricoes() {
    começar_jogo();
    resolver_com_restricoes();
    CU_ASSERT_TRUE(linhas_validas());
    CU_ASSERT_TRUE(colunas_validas());
}

int main() {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Testes do Projeto", NULL, NULL);
    if (!suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Testar salvar_estado", test_salvar_estado);
    CU_add_test(suite, "Testar desfazer", test_desfazer);
    CU_add_test(suite, "Testar carregar_txt", test_carregar_txt);
    CU_add_test(suite, "Testar gravar_txt", test_gravar_txt);
    CU_add_test(suite, "Testar parse_coord", test_parse_coord);
    CU_add_test(suite, "Testar casaaBranco", test_casaaBranco);
    CU_add_test(suite, "Testar casaRiscada", test_casaRiscada);
    CU_add_test(suite, "Testar verificar_repeticoes_letras_linhas", test_verificar_repeticoes_letras_linhas);
    CU_add_test(suite, "Testar verificar_minisculas_linhas", test_verificar_minisculas_linhas);
    CU_add_test(suite, "Testar verificar_vizinhos_riscados_linhas", test_verificar_vizinhos_riscados_linhas);
    CU_add_test(suite, "Testar linhas_validas", test_linhas_validas);
    CU_add_test(suite, "Testar verificar_repeticoes_letras_colunas", test_verificar_repeticoes_letras_colunas);
    CU_add_test(suite, "Testar verificar_minisculas_colunas", test_verificar_minisculas_colunas);
    CU_add_test(suite, "Testar verificar_vizinhos_riscados_colunas", test_verificar_vizinhos_riscados_colunas);
    CU_add_test(suite, "Testar colunas_validas", test_colunas_validas);
    CU_add_test(suite, "Testar todas_casas_conectadas", test_todas_casas_conectadas);
    CU_add_test(suite, "Testar verificar_vitoria", test_verificar_vitoria);
    CU_add_test(suite, "Testar pode_colocar", test_pode_colocar);
    CU_add_test(suite, "Testar aplicar_primeira_restricao", test_aplicar_primeira_restricao);
    CU_add_test(suite, "Testar resolver_com_restricoes", test_resolver_com_restricoes);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
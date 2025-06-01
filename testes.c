#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "utils-testes.h"

// Declare the estado_atual variable
extern estado_t *estado_atual;

void comecar_jogo(void);

void test_comecar_jogo() {
    comecar_jogo();
    CU_ASSERT_EQUAL(estado_atual->linhas, 5);
    CU_ASSERT_EQUAL(estado_atual->colunas, 5);
    CU_ASSERT_PTR_NULL(estado_atual->anterior);
}

void test_salvar_estado() {
    comecar_jogo();
    salvar_estado();
    CU_ASSERT_PTR_NOT_NULL(estado_atual->anterior);
}

void test_desfazer() {
    comecar_jogo();
    salvar_estado();
    desfazer();
    CU_ASSERT_PTR_NULL(estado_atual->anterior);
}

void test_carregar_txt() {
    comecar_jogo();
    gravar_txt("test.txt");
    carregar_txt("test.txt");
    CU_ASSERT_PTR_NOT_NULL(estado_atual);
}

void test_gravar_txt() {
    comecar_jogo();
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
    comecar_jogo();
    coordenada_t coord = {0, 0};
    casaaBranco(coord);
    CU_ASSERT_TRUE(isupper(estado_atual->tabuleiro[0][0]));
}

void test_casaRiscada() {
    comecar_jogo();
    coordenada_t coord = {0, 0};
    casaRiscada(coord);
    CU_ASSERT_EQUAL(estado_atual->tabuleiro[0][0], '#');
}

void test_verificar_repeticoes_letras_linhas() {
    comecar_jogo();
    CU_ASSERT_TRUE(verificar_repeticoes_letras_linhas());
}

void test_linhas_validas() {
    comecar_jogo();
    CU_ASSERT_TRUE(linhas_validas());
}

void test_verificar_repeticoes_letras_colunas() {
    comecar_jogo();
    CU_ASSERT_TRUE(verificar_repeticoes_letras_colunas());
}


void test_colunas_validas() {
    comecar_jogo();
    CU_ASSERT_TRUE(colunas_validas());
}

void test_todas_casas_conectadas() {
    comecar_jogo();
    CU_ASSERT_TRUE(todas_casas_conectadas());
}

void test_verificar_vitoria() {
    comecar_jogo();
    CU_ASSERT_FALSE(verificar_vitoria());
}

// void test_pode_colocar() {
//     comecar_jogo();
//     CU_ASSERT_TRUE(pode_colocar(0, 0, 'A'));
//     CU_ASSERT_FALSE(pode_colocar(0, 0, '#'));
// }

void test_aplicar_primeira_restricao() {
    comecar_jogo();
    CU_ASSERT_TRUE(aplicar_primeira_restricao());
}

void test_resolver_com_restricoes() {
    comecar_jogo();
    resolver_com_restricoes();
    CU_ASSERT_TRUE(linhas_validas());
    CU_ASSERT_TRUE(colunas_validas());
}

void test_jogo_resolvido() {
    carregar_txt("solucao.txt");
    //CU_ASSERT_TRUE(linhas_validas());
    CU_ASSERT_EQUAL(verificar_repeticoes_letras_linhas(), 1);
    CU_ASSERT_EQUAL(verificar_minisculas(), 1);
    CU_ASSERT_EQUAL(verificar_vizinhos_riscados(), 1);
    //CU_ASSERT_TRUE(colunas_validas());
    CU_ASSERT_EQUAL(verificar_repeticoes_letras_colunas(), 1);
    //CU_ASSERT_TRUE(verificar_vitoria());
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
    CU_add_test(suite, "Testar linhas_validas", test_linhas_validas);
    CU_add_test(suite, "Testar verificar_repeticoes_letras_colunas", test_verificar_repeticoes_letras_colunas);
    CU_add_test(suite, "Testar colunas_validas", test_colunas_validas);
    CU_add_test(suite, "Testar todas_casas_conectadas", test_todas_casas_conectadas);
    CU_add_test(suite, "Testar verificar_vitoria", test_verificar_vitoria);
    CU_add_test(suite, "Testar aplicar_primeira_restricao", test_aplicar_primeira_restricao);
    CU_add_test(suite, "Testar resolver_com_restricoes", test_resolver_com_restricoes);
    CU_add_test(suite, "Testar jogo_resolvido", test_jogo_resolvido);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
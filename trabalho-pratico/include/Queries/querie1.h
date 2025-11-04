/**
 * @file querie1.h
 * @brief Declarações de funções para a execução da query 1 no catálogo de utilizadores.
 *
 * Este arquivo define as funções necessárias para listar o resumo de um utilizador consoante o seu identificador,
 * imprimindo os resultados num ficheiro de saída.
 */

#ifndef QUEARIE1_H
#define QUEARIE1_H

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "users.h"

/**
 * @brief Imprime as informações de um utilizador no ficheiro de saída.
 * 
 * Esta função imprime os dados de um utilizador, como nome, email e tipo de subscrição,
 * para um ficheiro de saída especificado.
 * 
 * @param user Ponteiro para o utilizador cujas informações serão impressas.
 * @param output_file Ficheiro de saída onde as informações do utilizador serão gravadas.
 */
char* formatUserOutput(const User* user, char separador);
/**
 * @brief Executa a query 1 e imprime o resumo do utilizador no ficheiro de saída.
 * 
 * Esta função procura o utilizador pelo seu identificador no catálogo de utilizadores e,
 * caso encontrado, imprime o seu resumo no ficheiro de saída. Caso contrário, não imprime nada.
 * 
 * @param user_id Identificador do utilizador a ser procurado no catálogo.
 * @param user_catalog Catálogo de utilizadores onde a pesquisa será realizada.
 * @param artist_catalog Catálogo de artistas onde a pesquisa será realizada.
 * @param output_file Ficheiro de saída onde o resumo do utilizador será impresso.
 */
char* querie1(char* id, Users_catalog* user_catalog, Artists_catalog* artist_catalog, char separador);

#endif // QUEARIE1_H

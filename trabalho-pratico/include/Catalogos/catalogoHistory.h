#ifndef CATALOGO_HISTORIC_H
#define CATALOGO_HISTORIC_H

#include <glib.h>
#include "history.h"
#include "catalogoUser.h"
#include "catalogoMusic.h"

/**
 * @struct History_catalog
 * @brief Estrutura que contém as tabelas de históricos organizados por ano.
 */
typedef struct history_catalog History_catalog;

/**
 * @brief Obtém a tabela hash de anos do catálogo de históricos.
 * 
 * @param catalogo Ponteiro para o catálogo de históricos.
 * @return Ponteiro para a tabela hash de anos.
 */
GHashTable* get_years_table(History_catalog* catalogo);

/**
 * @brief Libera a memória associada ao catálogo de históricos.
 * 
 * @param catalog Ponteiro para a estrutura do catálogo de históricos.
 */
void free_history_catalog(History_catalog* catalog);

/**
 * @brief Cria e carrega um catálogo de históricos a partir de um arquivo CSV.
 * 
 * @param path Caminho para o arquivo CSV contendo os dados dos históricos.
 * @param structure1 Estrutura auxiliar necessária para a construção do catálogo.
 * @param structure2 Estrutura auxiliar necessária para a construção do catálogo.
 * @return Ponteiro para o catálogo de históricos criado.
 */
History_catalog* historyToCatalog(char* path, void* structure1, void* structure2, void* structure4);

/**
 * @brief Constrói e adiciona um histórico ao catálogo a partir de tokens.
 * 
 * @param line Linha contendo os dados do histórico.
 * @param tokens Array de tokens extraídos da linha.
 * @param catalog Ponteiro para o catálogo de históricos onde o histórico será adicionado.
 * @param user_catalog Ponteiro para o catálogo de usuários.
 * @param music_catalog Ponteiro para o catálogo de músicas.
 */
void buildHistory(char* line, char** tokens, History_catalog* catalog, Users_catalog* user_catalog, Musics_catalog* music_catalog, Artists_catalog* artist_catalog);
/**
 * @brief Wrapper para uso com a função parser, chamando buildHistory.
 * 
 * @param line Linha contendo os dados do histórico.
 * @param tokens Array de tokens extraídos da linha.
 * @param structure1 Ponteiro para a primeira estrutura auxiliar.
 * @param structure2 Ponteiro para a segunda estrutura auxiliar.
 * @param structure3 Ponteiro para a terceira estrutura auxiliar.
 * @param structure4 Ponteiro para a quarta estrutura auxiliar.
 * @param structure5 Ponteiro para a quinta estrutura auxiliar.
 */
void buildHistory_wrapper(char* line, char** tokens, void* structure1, void* structure2, void* structure3, void* structure4, void* structure5);

#endif // CATALOGO_HISTORIC_H

#ifndef ARTISTS_CATALOG_H
#define ARTISTS_CATALOG_H

#include <glib.h>
#include "artists.h"          
#include "parser.h"          

/**
 * @struct Artists_catalog
 * @brief Estrutura que contém uma hash table de artistas.
 */
typedef struct artists_catalog Artists_catalog;

/**
 * @brief Obtém um artista pelo ID no catálogo.
 * 
 * @param catalogo Ponteiro para o catálogo de artistas.
 * @param id ID do artista a ser buscado.
 * @return Ponteiro para o artista encontrado ou NULL se não encontrado.
 */
Artist* get_artist(Artists_catalog* catalogo, char* id);

/**
 * @brief Verifica se um artista está contido no catálogo.
 * 
 * @param catalogo Ponteiro para o catálogo de artistas.
 * @param id ID do artista a ser verificado.
 * @return TRUE se o artista existir, FALSE caso contrário.
 */
gboolean artist_Contains(Artists_catalog* catalogo, char* id);

/**
 * @brief Obtém a tabela hash de artistas do catálogo.
 * 
 * @param catalog Ponteiro para o catálogo de artistas.
 * @return Ponteiro para a tabela hash de artistas.
 */
GHashTable* get_artists_table(Artists_catalog* catalog);

/**
 * @brief Adiciona duração total para um artista no catálogo.
 * 
 * @param catalogo Ponteiro para o catálogo de artistas.
 * @param id ID do artista.
 * @param duracao Duração a ser adicionada.
 */
void add_duracao_artist_catalog(Artists_catalog* catalogo, char* id, int duracao);

/**
 * @brief Adiciona profit total para um artista no catálogo.
 * 
 * @param catalogo Ponteiro para o catálogo de artistas.
 * @param id ID do artista.
 */
void add_profit_artist_catalog(Artists_catalog* catalogo, char* id);

/**
 * @brief Adiciona num albums total para um artista no catálogo.
 * 
 * @param catalogo Ponteiro para o catálogo de artistas.
 * @param id ID do artista.
 */
void add_num_album_artista(Artists_catalog* catalogo, char* id);



/**
 * @brief Constrói e adiciona um artista ao catálogo a partir de tokens.
 * 
 * @param line Linha contendo os dados do artista.
 * @param tokens Array de tokens extraídos da linha.
 * @param catalogo Ponteiro para o catálogo de artistas onde o artista será adicionado.
 */
void buildArtist(char* line, char** tokens, Artists_catalog* catalogo);

/**
 * @brief Wrapper para uso com a função parser, chamando buildArtist.
 * 
 * @param line Linha contendo os dados do artista.
 * @param tokens Array de tokens extraídos da linha.
 * @param structure1 Ponteiro para a primeira estrutura.
 * @param structure2 Ponteiro para a segunda estrutura.
 * @param structure3 Ponteiro para a terceira estrutura.
 * @param structure4 Ponteiro para a quarta estrutura.
 * @param structure5 Ponteiro para a quinta estrutura.
 */
void buildArtist_wrapper(char* line, char** tokens, void* structure1, void* structure2, void* structure3, void* structure4, void* structure5);

/**
 * @brief Cria e carrega um catálogo de artistas a partir de um arquivo CSV.
 * 
 * @param path Caminho para o arquivo CSV contendo os dados dos artistas.
 * @return Ponteiro para o catálogo de artistas criado.
 */
Artists_catalog* artistsToCatalog(char* path);

/**
 * @brief Libera a memória alocada para o catálogo de artistas.
 * 
 * @param catalogo Ponteiro para o catálogo de artistas a ser liberado.
 */
void free_artists_catalog(Artists_catalog* catalogo);

/**
 * @brief Destrói a hash table de artistas do catálogo.
 * 
 * @param catalog Ponteiro para o catálogo de artistas a ser destruído.
 */
void artist_hash_table_destroy(Artists_catalog* catalog);

#endif // ARTISTS_CATALOG_H

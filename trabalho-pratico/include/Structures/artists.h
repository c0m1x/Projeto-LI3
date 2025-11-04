#ifndef ARTIST_H
#define ARTIST_H

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @struct Artist
 * @brief Estrutura que representa um artista.
 */
typedef struct artist Artist;


/**
 * @brief Obtém o número de albums de um artista.
 * @param artist Artista.
 * @return Número de albums.
 */
int get_artist_num_albums(const Artist* artist);

/**
 * @brief Obtém o profit total de um artista.
 * @param artist Artista.
 * @return profit total.
 */
double get_artist_profitTotal(const Artist* artist);

/**
 * @brief Obtém o nome de um artista.
 * @param artist Artista.
 * @return Nome do artista.
 */
char* get_artist_name(const Artist* artist);

/**
 * @brief Obtém o país de um artista.
 * @param artist Artista.
 * @return País do artista.
 */
char* get_artist_country(const Artist* artist);

/**
 * @brief Obtém o tipo de um artista.
 * @param artist Artista.
 * @return Tipo do artista.
 */
char* get_artist_type(const Artist* artist);

/**
 * @brief Obtém o ID do constituinte de um artista.
 * @param artist Artista.
 * @return ID do constituinte.
 */
const GArray* get_artist_id_constituent(const Artist* artist);

/**
 * @brief Obtém o valor do "receita por stream" de um artista.
 * @param artist Artista.
 * @return Receita por stream.
 */
double get_artist_recipe_per_stream(const Artist* artist);

/**
 * @brief Obtém o valor da duracao total de um artista.
 * @param artist Artista.
 * @return duracao.
 */
int get_artist_duracaoTotal(const Artist* artist);

/**
 * @brief Adiciona uma duracao ao artista.
 * @param artist Artista.
 * @param duracao duracao.
 */
void add_duracao_artist(Artist* artist, int duracao);

/**
 * @brief Adiciona profit ao ao artista.
 * @param artist Artista.
 * @param recipe Receita.
 */
void add_profit_artist(Artist* artist, double recipe);

/**
 * @brief Adiciona um album ao artista.
 * @param artist Artista.
 */
void add_num_albuns_individual(Artist* artist);

/**
 * @brief Libera a memória alocada para um artista.
 * @param artist Artista.
 */
void free_artist(Artist* artist);

/**
 * @brief Libera a memória de um artista genérico.
 * @param artist Ponteiro para o artista.
 */
void free_artist_void(gpointer artist);

/**
 * @brief Encontra um artista na tabela hash.
 * @param table Tabela hash.
 * @param id ID do artista.
 * @return Artista encontrado ou NULL.
 */
Artist* find_artist(GHashTable* table, const char* id);

/**
 * @brief Cria uma cópia de um artista.
 * @param artist Artista.
 * @return Cópia do artista.
 */
Artist* cloneArtista(Artist* artist);

/**
 * @brief Valida os tokens de um artista.
 * @param tokens Tokens.
 * @return 1 se válido, 0 caso contrário.
 */
int valida_tokens_artistas(char** tokens);

/**
 * @brief Completa a estrutura de um artista a partir dos tokens.
 * @param tokens Tokens.
 * @return Artista completo.
 */
Artist* completa_Artist(char* line,char** tokens);

#endif // ARTIST_H

#ifndef MUSICS_CATALOG_H
#define MUSICS_CATALOG_H

#include <glib.h>
#include "musics.h"          
#include "parser.h"         

/**
 * @struct Musics_catalog
 * @brief Estrutura que contém uma hash table de músicas.
 */
typedef struct musics_catalog Musics_catalog;

/**
 * @brief Obtém uma música pelo ID no catálogo.
 * @param catalogo 
 * @param id 
 * @return Ponteiro para a música encontrada ou NULL se não encontrada.
 */
Music* getMusic(Musics_catalog* catalogo, char* id);



/**
 * @brief Obtém o genero de uma música pelo ID no catálogo.
 * @param catalogo 
 * @param id 
 * @return Ponteiro para o genero da música encontrada ou NULL se não encontrada.
 */
char* getMusic_genre(Musics_catalog* catalogo, char* music_id);


/**
 * @brief Obtém os artistas de uma música pelo ID no catálogo.
 * @param catalogo 
 * @param id 
 * @return Ponteiro para o array de artistas da música encontrada ou NULL se não encontrada.
 */
const GArray* get_artist_music_catalog(Musics_catalog* catalogo, char* music_id);

/**
 * @brief Verifica se uma música existe no catálogo.
 * @param catalog 
 * @param music_id 
 * @return 1 se a música existe, 0 caso contrário.
 */
int music_exists(Musics_catalog* catalog, const char* music_id);

/**
 * @brief Constrói e adiciona uma música ao catálogo a partir de tokens.
 * @param tokens 
 * @param catalogo 
 * @param structure 
 */
void buildMusic(char* line, char** tokens, Musics_catalog* catalogo, void* structure1, void* structure2);

/**
 * @brief Wrapper para uso com a função parser, chamando buildMusic.
 * @param tokens 
 * @param structure1 
 * @param structure2 
 * @param structure3 
 */
void buildMusic_wrapper(char* line, char ** tokens, void * structure1, void * structure2, void * structure3, void * structure4, void * structure5) ;

/**
 * @brief Cria e carrega um catálogo de músicas a partir de um arquivo CSV.
 * @param path 
 * @param structure 
 * @return Ponteiro para o catálogo de músicas criado.
 */
Musics_catalog* musicsToCatalog(char* path, void* structure1, void* structure2);

/**
 * @brief Libera a memória alocada para o catálogo de músicas.
 * @param catalogo 
 */
void free_musics_catalog(Musics_catalog *catalogo);

/**
 * @brief Obtém o tamanho do catálogo de músicas.
 * @param catalogo 
 * @return Número de músicas no catálogo.
 */
int getMusicsLenght(Musics_catalog * catalogo);

/**
 * @brief Obtém a hash table de músicas do catálogo.
 * @param catalog 
 * @return Ponteiro para a hash table de músicas.
 */
GHashTable* get_musics_hash_table(Musics_catalog* catalog);


/**
 * @brief Destrói hash table de musicas do catálogo.
 * @param catalog 
 */
void music_hash_table_destroy(Musics_catalog* catalog);

#endif // MUSICS_CATALOG_H

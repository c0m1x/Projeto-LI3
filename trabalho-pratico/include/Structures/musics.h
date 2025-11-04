#ifndef MUSICASH
#define MUSICASH

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "artists.h"
#include "catalogoArtist.h"

#include "catalogoAlbum.h"

/**
 * @struct Music
 * @brief Estrutura que representa uma música.
 */
typedef struct music Music;


/**
 * @brief Obtém os IDs dos artistas de uma música.
 * @param music Música.
 * @return Lista de IDs dos artistas.
 */
const GArray* get_music_artist_idss(const Music* music);

/**
 * @brief Obtém o album de uma música.
 * @param music Música.
 * @return Album da música.
 */
char* get_music_album(const Music* music);

/**
 * @brief Obtém a duração de uma música.
 * @param music Música.
 * @return Duração da música.
 */
char* get_music_duration(const Music* music);

/**
 * @brief Obtém o gênero de uma música.
 * @param music Música.
 * @return Gênero da música.
 */
char* get_music_genre(const Music* music);


/**
 * @brief Cria uma cópia de uma música.
 * @param music Música.
 * @return Cópia da música.
 */
Music* cloneMusica(Music* music);

/**
 * @brief Libera a memória alocada para uma música.
 * @param music Música.
 */
void freemusicas(Music* musicas);

/**
 * @brief Libera a memória de uma música genérica.
 * @param music Música.
 */
void free_music_void(gpointer music);


/**
 * @brief Procura uma música na tabela hash.
 * @param tabelamusica Tabela hash.
 * @param idsmusica ID da música.
 * @return Música encontrada ou NULL.
 */
Music* procmusicatabel(GHashTable* tabelamusica, const char* idsmusica);

/**
 * @brief Converte a duração de uma música de string para inteiro.
 * @param duration_str String com a duração.
 * @return Duração em segundos.
 */
int convert_duration(char* duration_str);

/**
 * @brief Converte uma string com IDs de artistas para um GArray.
 * @param ids_str String com os IDs.
 * @return GArray com IDs de artistas.
 */
GArray* parse_artist_ids(char* ids_str);

/**
 * @brief Valida os tokens de música.
 * @param tokens Tokens.
 * @param artists_catalog Catálogo de artistas.
 * @return 1 se válido, 0 caso contrário.
 */
int valida_tokens_musics(char** tokens, Artists_catalog* artists_catalog, Album_catalog* album_catalog);

/**
 * @brief Preenche a estrutura de uma música com tokens.
 * @param tokens Tokens.
 * @param structure1 Estrutura artistas.
 * @param structure2 Estrutura albums
 * @return Música preenchida.
 */
Music* completa_Musica(char* line,char** tokens, void* structure1, void* structure2) ;

/**
 * @brief Calcula a duracao em segundos de uma música.
 * @param music Musica.
 * @return duracao em segundos.
 */
int get_music_duration_int(Music* music);


#endif // MUSICASH

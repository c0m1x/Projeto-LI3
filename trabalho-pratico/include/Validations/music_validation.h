/**
 * @file music_validation.h
 * @brief Definições de funções para validação de músicas.
 */

#ifndef MUSICS_VALIDATIONH
#define MUSICS_VALIDATIONH

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "artists.h"
#include "catalogoAlbum.h"
#include "catalogoArtist.h"
#include "catalogoMusic.h"

/**
 * @brief Valida o ano de uma música.
 * 
 * Verifica se o ano fornecido está no formato esperado e é um ano válido.
 * 
 * @param year Ano a ser validado.
 * @return 1 se o ano for válido, 0 caso contrário.
 */
int validar_ano(const char *year);

/**
 * @brief Valida a duração de uma música.
 * 
 * Verifica se a duração da música está em um formato válido (ex: mm:ss, ss).
 * 
 * @param duracao Duração da música a ser validada.
 * @return 1 se a duração for válida, 0 caso contrário.
 */
int validacaoduracao(char *duracao);

/**
 * @brief Verifica se o artista existe na hash table.
 * 
 * Verifica se os artistas especificados na lista de IDs existem no catálogo de artistas.
 * 
 * @param artist_catalog Catálogo de artistas.
 * @param artist_idss Lista de IDs de artistas a serem verificados.
 * @return 1 se todos os artistas existirem, 0 caso contrário.
 */
int valida_artistas_existentes(Artists_catalog* artist_catalog, GArray* artist_idss);

/**
 * @brief Verifica se o album existe na hash table.
 * 
 * @param album_catalog Catálogo de albums.
 * @param album Album a ser verificado.
 * @return 1 se todos os artistas existirem, 0 caso contrário.
 */
int valida_album(Album_catalog* album_catalog, char* album);

#endif 

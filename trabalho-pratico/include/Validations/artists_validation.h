/**
 * @file artist_validation.h
 * @brief Definições de funções para validação de artistas.
 */

#ifndef ARTIST_VALIDATION_H
#define ARTIST_VALIDATION_H

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "artists.h"

/**
 * @brief Valida as informações de um artista.
 * 
 * Verifica se o tipo e o constituinte de um artista são válidos.
 * 
 * @param constituent Constituintes do artista (ex: membro de banda).
 * @param type Tipo do artista (ex: solo, banda).
 * @return 1 se válido, 0 caso contrário.
 */
int is_artist_valid(char* constituent, char* type);

#endif

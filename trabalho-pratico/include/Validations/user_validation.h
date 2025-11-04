#ifndef USER_VALIDATION_H
#define USER_VALIDATION_H

#include <glib.h> 
#include "users.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Valida o email de um utilizador.
 * 
 * Verifica se o email fornecido está no formato válido de email (username@domínio).
 * 
 * @param email O email a ser validado.
 * @return 1 se o email for válido, 0 caso contrário.
 */
int validar_email(const char *email);

/**
 * @brief Valida a data de nascimento de um utilizador.
 * 
 * Verifica se a data de nascimento fornecida está no formato válido (AAAA-MM-DD).
 * 
 * @param birth_date A data de nascimento a ser validada.
 * @return 1 se a data de nascimento for válida, 0 caso contrário.
 */
int validar_data(const char *birth_date);

/**
 * @brief Valida o tipo de subscrição de um utilizador.
 * 
 * Verifica se o tipo de subscrição fornecido é um dos tipos válidos.
 * 
 * @param subscription_type O tipo de subscrição a ser validado.
 * @return 1 se o tipo de subscrição for válido, 0 caso contrário.
 */
int validar_sub(const char *subscription_type);

/**
 * @brief Valida as músicas gostadas de um utilizador.
 * 
 * Verifica se todas as músicas na lista de "liked_songs_id" existem no catálogo de músicas.
 * 
 * @param liked_songs_id Lista de IDs de músicas gostadas pelo utilizador.
 * @param music_catalog Catálogo de músicas.
 * @return 1 se todas as músicas gostadas forem válidas, 0 caso contrário.
 */
int validar_liked_songs(GArray* liked_songs_id, Musics_catalog* music_catalog);

#endif // USER_VALIDATION_H

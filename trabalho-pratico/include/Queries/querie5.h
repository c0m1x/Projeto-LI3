#ifndef QUERIE5_H
#define QUERIE5_H

#include <glib.h>
#include "users.h"
#include "musics.h"
#include "catalogoUser.h"
#include "catalogoMusic.h"
#include "history.h"
#include "catalogoHistory.h"
#include "utils.h"
#include "../recomendador/recomendador.h"

/**
 * @brief Gera recomendações de utilizadores semelhantes com base no histórico e preferências musicais.
 *
 * @param users_catalog Catálogo de utilizadores.
 * @param musics_catalog Catálogo de músicas.
 * @param history_catalog Catálogo do histórico de utilizadores.
 * @param idUtilizadorAlvo Identificador do utilizador alvo.
 * @param numRecomendacoes Número de recomendações a retornar.
 * @return String formatada com a lista de utilizadores semelhantes.
 */

char *querie5(Users_catalog *users_catalog, char *id_utilizador_alvo, int num_recomendacoes, int **matriz);

/**
 * @brief Gera recomendações de utilizadores semelhantes com base no histórico e preferências musicais usando o nosso recomendador.
 *
 * @param users_catalog Catálogo de utilizadores.
 * @param musics_catalog Catálogo de músicas.
 * @param history_catalog Catálogo do histórico de utilizadores.
 * @param idUtilizadorAlvo Identificador do utilizador alvo.
 * @param numRecomendacoes Número de recomendações a retornar.
 * @return String formatada com a lista de utilizadores semelhantes.
 */
char *querie5_v2(Users_catalog *users_catalog, char *id_utilizador_alvo, int num_recomendacoes, int **matriz);

#endif // QUERIE5_H

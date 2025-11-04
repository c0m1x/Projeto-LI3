#ifndef OUTPUT_INTERATIVO_H
#define OUTPUT_INTERATIVO_H

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include "artists.h"
#include "musics.h"
#include "users.h"
#include "command_parser.h"
#include "utils.h"
#include "catalogoUser.h"
#include "catalogoArtist.h"
#include "catalogoMusic.h"
#include "catalogoHistory.h"


/**
 * @brief Executa a query apropriada com base nos parâmetros fornecidos.
 *
 * @param querie O número da query a executar (1, 2, 3 ou 5).
 * @param separador O caractere separador para formatar a saída.
 * @param args O argumento principal para a query (por exemplo, ID do utilizador).
 * @param args2 O segundo argumento para a query (por exemplo, um inteiro ou string).
 * @param artists_catalog Apontador para o catálogo de artistas.
 * @param musics_catalog Apontador para o catálogo de músicas.
 * @param users_catalog Apontador para o catálogo de utilizadores.
 * @param matriz Matriz de utilizadores e géneros.
 *
 * @return Uma string alocada dinamicamente contendo o resultado da query. O chamador é responsável por libertar a memória.
 */

char* output(int querie, char separador, char* args, char* args2, char* args3, Artists_catalog* artists_catalog, 
            Musics_catalog* musics_catalog, Users_catalog* users_catalog, History_catalog* history_catalog, int** matriz);

#endif // OUTPUT_INTERATIVO_H

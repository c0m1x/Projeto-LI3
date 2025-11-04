#ifndef QUERIE2_H
#define QUERIE2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "artists.h"
#include "querie2.h"
#include "users.h"
#include "musics.h"

/**
 * @brief Formata as informações de um artista para uma string de saída.
 * 
 * @param artist Ponteiro para o artista cujas informações serão formatadas.
 * @param separador Caractere usado como separador entre os campos.
 * @return Ponteiro para a string formatada. O chamador deve liberar a memória.
 */
char* formatArtistOutput(const Artist* artist, char separador);

/**
 * @brief Função de comparação para ordenar artistas por duração total de músicas e, em caso de empate, por nome.
 * 
 * @param a Ponteiro para o primeiro artista a ser comparado.
 * @param b Ponteiro para o segundo artista a ser comparado.
 * @return Um valor negativo, zero ou positivo, dependendo da ordem dos artistas.
 */
int compare_artists_by_total_duration_then_name(gconstpointer a, gconstpointer b);

/**
 * @brief Executa a Query 2, listando os artistas mais relevantes com base na duração total de suas músicas.
 * 
 * @param catalogo Catálogo de artistas.
 * @param catalogo2 Catálogo de músicas.
 * @param N Número máximo de artistas a serem listados.
 * @param country_filter Filtro opcional para o país dos artistas. Use NULL para incluir todos os países.
 * @param separador Caractere usado como separador na formatação da saída.
 * @return Ponteiro para a string contendo os resultados formatados. O chamador deve liberar a memória.
 */
char* querie2(Artists_catalog* catalogo, Musics_catalog* catalogo2, int N, const char* country_filter, char separador);

#endif // QUERIE2_H
#ifndef QUERIE6_H
#define QUERIE6_H

#include <glib.h>

/**
 * Gera um resumo anual de estatísticas para um utilizador com base no histórico de reprodução.
 * 
 * @param query_user O ID do utilizador alvo da consulta.
 * @param query_year O ano alvo da consulta.
 * @param N Número de artistas mais ouvidos a listar (opcional).
 * @param musics_table Tabela de hash contendo as músicas do catálogo.
 * @param history_list Lista ligada contendo o histórico de reproduções.
 * @return Uma string formatada com as estatísticas anuais ou uma linha vazia ("\n") se não houver dados.
 */
char* querie6(char separador,char* query_user, char* query_year, int N, Musics_catalog* music_catalog, History_catalog* history_catalog);

#endif // QUERIE6_H

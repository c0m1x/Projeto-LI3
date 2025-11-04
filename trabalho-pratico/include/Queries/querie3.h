#ifndef QUERIE3_H
#define QUERIE3_H

#include <glib.h>
#include <stdio.h>

/**
 * @brief Estrutura para armazenar a popularidade de cada gênero.
 */
typedef struct genrePopularity GenrePopularity;

/**
 * @brief Formata as informações de um gênero para uma string de saída.
 * 
 * @param genre_popularity Ponteiro para a estrutura que contém informações do gênero.
 * @param separador Caractere usado como separador entre os campos.
 * @return Ponteiro para a string formatada. O chamador deve liberar a memória.
 */
char* formatGenreOutput(const GenrePopularity* genre_popularity, char separador);

/**
 * @brief Função de comparação usada para ordenar os gêneros por popularidade.
 * 
 * @param a Ponteiro para o primeiro gênero a ser comparado.
 * @param b Ponteiro para o segundo gênero a ser comparado.
 * @return Um valor negativo, zero ou positivo, dependendo da ordem dos gêneros.
 */
int compare_genres(const void* a, const void* b);

/**
 * @brief Libera a memória da estrutura GenrePopularity.
 * 
 * @param data Ponteiro para a estrutura a ser liberada.
 */
void free_genre_popularity(gpointer data);

/**
 * @brief Função principal da Query 3, que determina a popularidade dos gêneros musicais com base na faixa etária dos usuários.
 * 
 * @param min_age Idade mínima dos utilizadores a considerar.
 * @param max_age Idade máxima dos utilizadores a considerar.
 * @param users_catalog Catálogo dos utilizadores.
 * @param musics_catalog Catálogo das músicas.
 * @param separador Caractere usado como separador na formatação da saída.
 * @return Ponteiro para a string contendo os resultados formatados. O chamador deve liberar a memória.
 */
char* querie3(int min_age, int max_age, Users_catalog* users_catalog, Musics_catalog* musics_catalog, char separador);

#endif // QUERIE3_H

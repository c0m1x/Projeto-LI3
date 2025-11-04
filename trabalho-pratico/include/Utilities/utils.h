#ifndef UTILS_H
#define UTILS_H

#include <glib.h>
#include <stdio.h>

/**
 * @brief Escreve o cabeçalho de um arquivo CSV se ele for novo.
 * 
 * @param file Ponteiro para o arquivo onde o cabeçalho será escrito.
 * @param header String contendo o cabeçalho a ser escrito.
 */
void write_csv_header_if_new(FILE *file, const char *header);

/**
 * @brief Imprime os elementos de um array de strings em formato CSV.
 * 
 * @param file Ponteiro para o arquivo onde os elementos serão escritos.
 * @param array Ponteiro para um `GArray` contendo as strings.
 */
void print_string_array_to_csv(FILE *file, GArray* array);

/**
 * @brief Junta os IDs das músicas curtidas em uma única string separada por vírgulas.
 * 
 * @param liked_songs_id Ponteiro para um `GArray` contendo os IDs das músicas curtidas.
 * @return String com os IDs das músicas concatenados. O chamador deve liberar a memória.
 */
char *join_liked_songs(GArray *liked_songs_id);

/**
 * @brief Constrói o caminho completo de um arquivo a partir do caminho da pasta e do nome do arquivo.
 * 
 * @param folder_path Caminho da pasta.
 * @param file_name Nome do arquivo.
 * @return String contendo o caminho completo. O chamador deve liberar a memória.
 */
char* build_file_path(const char* folder_path, const char* file_name);

/**
 * @brief Remove aspas de uma string.
 * 
 * @param str String de entrada.
 * @return Ponteiro para a string modificada. As alterações são feitas in-place.
 */
char* remover_aspa(char* str);

/**
 * @brief Verifica se o país de um artista é permitido.
 * 
 * @param pais_artista String contendo o país do artista.
 * @param paises_permitidos Tabela hash contendo os países permitidos.
 * @return 1 se o país for permitido, 0 caso contrário.
 */
int pais_permitido(const char* pais_artista, GHashTable* paises_permitidos);

/**
 * @brief Remove aspas de uma string e retorna uma nova string sem aspas.
 * 
 * @param str String de entrada.
 * @return Ponteiro para a nova string sem aspas. O chamador deve liberar a memória.
 */
char* remover_aspas_dup(const char* str);

/**
 * @brief Converte uma duração em segundos para uma string formatada como HH:MM:SS.
 * 
 * @param seg Número de segundos.
 * @return Ponteiro para a string formatada. O chamador deve liberar a memória.
 */
char *utils_segundos_para_string(int seg);

/**
 * @brief Converte uma string de números separados por vírgula em um array de inteiros.
 * 
 * @param s String de entrada.
 * @return Ponteiro para um `GArray` contendo os inteiros. O chamador deve liberar a memória.
 */
GArray* funcao(const char* s);

/**
 * @brief Converte um gênero musical de string para um valor inteiro.
 * 
 * @param genero String contendo o gênero musical.
 * @return Valor inteiro correspondente ao gênero.
 */
int genero_to_int(char* genero);

/**
 * @brief Limpa os caracteres indesejados de um artista.
 * 
 * @param genero String contendo o artista.
 * @return artista entre aspas.
 */
char *clean_artist_id(const char *input);

/**
 * @brief Limpa os caracteres indesejados de um conjunto de artistas.
 * 
 * @param genero String contendo os artistas.
 * @return artista entre aspas.
 */
char** process_string(const char* input);

#endif

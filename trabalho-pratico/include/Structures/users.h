#ifndef USER_H
#define USER_H

#include <glib.h> 
#include <stdio.h>
#include <stdlib.h>
#include "musics.h"
#include "artists.h"
#include "catalogoArtist.h"
#include "catalogoMusic.h"

/**
 * @struct User
 * @brief Estrutura que representa um utilizador.
 */
typedef struct user User;

/**
 * @struct User
 * @brief Estrutura que representa um utilizador para a querie5.
 */
typedef struct users_querie5 Users_querie5;


/**
 * @brief Obtém o array de generos de um utilizador.
 * @param user Utilizador da querie5.
 * @return primeiro elemento do array de generos.
 */
int* get_arrayGeneros(User* user);

/**
 * @brief Adiciona um genero ao array de generos.
 * @param user
 * @param genero
 */
void add_genero_User(User* user, int genero);

/**
 * @brief Obtém o nome de utilizador.
 * @param user Utilizador.
 * @return Nome de utilizador.
 */
char* get_user_username(const User* user);

/**
 * @brief Obtém o email do utilizador.
 * @param user Utilizador.
 * @return Email do utilizador.
 */
char* get_email(const User* user);

/**
 * @brief Obtém o primeiro nome do utilizador.
 * @param user Utilizador.
 * @return Primeiro nome do utilizador.
 */
char* get_first_name(const User* user);

/**
 * @brief Obtém o sobrenome do utilizador.
 * @param user Utilizador.
 * @return Sobrenome do utilizador.
 */
char* get_last_name(const User* user);

/**
 * @brief Obtém o país do utilizador.
 * @param user Utilizador.
 * @return País do utilizador.
 */
char* get_country(const User* user);

/**
 * @brief Obtém a idade do utilizador.
 * @param user Utilizador.
 * @return Idade do utilizador.
 */
int get_age(const User* user);

/**
 * @brief Obtém o tipo de assinatura do utilizador.
 * @param user Utilizador.
 * @return Tipo de assinatura.
 */
char* get_subscription_type(const User* user);

/**
 * @brief Obtém a data de nascimento do utilizador.
 * @param user Utilizador.
 * @return Data de nascimento.
 */
char* get_birth_date(const User* user);

/**
 * @brief Obtém os IDs das músicas gostadas pelo utilizador.
 * @param user Utilizador.
 * @return Lista de IDs de músicas gostadas.
 */
const GArray* get_liked_songs_id(const User* user);

/**
 * @brief Obtém o comprimento da lista de músicas gostadas.
 * @param user Utilizador.
 * @return Comprimento da lista de músicas.
 */
int get_liked_songs_id_length(const User* user);

/**
 * @brief Calcula a idade do utilizador com base na data de nascimento.
 * @param birth_date Data de nascimento no formato "YYYY-MM-DD".
 * @return Idade do utilizador.
 */
int calc_age(const char* birth_date);

/**
 * @brief Cria uma cópia de um utilizador.
 * @param user Utilizador.
 * @return Cópia do utilizador.
 */
User* cloneUser(User* user);

/**
 * @brief Adiciona uma música à lista de músicas gostadas do utilizador.
 * @param user Utilizador.
 * @param song_id ID da música.
 */
void add_liked_song_to_user(User* user, char* song_id);



/**
 * @brief Liberta a memória alocada para um utilizador querie5.
 * @param user Utilizador.
 */
void free_user_querie5(Users_querie5* user);

/**
 * @brief Liberta a memória de um utilizador genérico.
 * @param user Utilizador.
 */
void free_user_querie5_void(gpointer user);

/**
 * @brief Liberta a memória alocada para um utilizador.
 * @param user Utilizador.
 */
void free_user(User* user);

/**
 * @brief Liberta a memória de um utilizador genérico.
 * @param user Utilizador.
 */
void free_user_void(gpointer user);

/**
 * @brief Inicializa a tabela hash de utilizadores.
 * @return Tabela hash de utilizadores.
 */
GHashTable* init_users_table();

/**
 * @brief Insere um utilizador na tabela hash.
 * @param table Tabela hash.
 * @param user Utilizador.
 */
void insert_user(GHashTable* table, User* user);

/**
 * @brief Procura um utilizador na tabela hash.
 * @param table Tabela hash.
 * @param username Nome de utilizador.
 * @return Utilizador encontrado ou NULL.
 */
User* find_user(GHashTable* table, const char* username);

/**
 * @brief Converte uma string de IDs de músicas gostadas para um GArray.
 * @param liked_songs_str String com IDs.
 * @return GArray com os IDs das músicas.
 */
GArray* parse_liked_songs(char* liked_songs_str);

/**
 * @brief Valida os tokens de um utilizador.
 * @param tokens Tokens.
 * @param music_catalog Catálogo de músicas.
 * @return 1 se válido, 0 caso contrário.
 */
int valida_user_tokens(char** tokens, Musics_catalog* music_catalog);

/**
 * @brief Preenche a estrutura de um utilizador com tokens.
 * @param tokens Tokens.
 * @param structure1 Estrutura de destino.
 * @return Utilizador preenchido.
 */
User* completa_User(char* line, char** tokens, void* structure1);

#endif

#ifndef USERS_CATALOG_H
#define USERS_CATALOG_H

#include <glib.h>
#include "users.h"
#include "parser.h"

/**
 * @struct Users_catalog
 * @brief Estrutura que contém uma hash table de utilizadores.
 */
typedef struct users_catalog Users_catalog;


/**
 * @brief Obtém um utilizador pelo ID no catálogo.
 * @param catalogo 
 * @param id 
 * @return Ponteiro para o utilizador encontrado ou NULL se não encontrado.
 */
User* getUser(Users_catalog* catalogo, char* id);


/**
 * @brief Adiciona um genero ao array de generos.
 * @param catalogo 
 * @param id 
 * @param genero
 */
void add_genero_User_catalog(Users_catalog* catalogo, char* user_id,  int genero);

/**
 * @brief Verifica se um user da querie5 existe.
 * @param catalogo 
 * @param id 
 * @return boleano.
 */
gboolean user_exists(Users_catalog* catalogo, char* user_id);


/**
 * @brief Constrói e adiciona um utilizador ao catálogo a partir de tokens.
 * @param tokens 
 * @param catalogo 
 * @param structure1 
 */
void buildUser(char* line,char** tokens, Users_catalog* catalogo, void* structure1);

/**
 * @brief Wrapper para uso com a função parser, chamando buildUser.
 * @param tokens 
 * @param structure1 
 * @param structure2 
 * @param structure3 
 */
void buildUser_wrapper(char* line, char ** tokens, void * structure1, void * structure2, void * structure3, void * structure4, void * structure5);

/**
 * @brief Cria e carrega um catálogo de utilizadores a partir de um arquivo CSV.
 * @param path 
 * @param structure 
 * @return Ponteiro para o catálogo de utilizadores criado.
 */
Users_catalog* usersToCatalog(char* path, void* structure);

/**
 * @brief Liberta a memória alocada para o catálogo de utilizadores.
 * @param catalogo 
 */
void free_users_catalog(Users_catalog* catalogo);


/**
 * @brief Obtém a hash table de utilizadores do catálogo.
 * @param catalog 
 * @return Ponteiro para a hash table de utilizadores.
 */
GHashTable* get_users_hash_table(Users_catalog* catalog);

/**
 * @brief Destrói hash table de users do catálogo.
 * @param catalog 
 */
void user_hash_table_destroy(Users_catalog* catalog);

/**
 * @brief Obtém a matriz de utilizadores do catálogo.
 * @param catalog 
 * @return Ponteiro para a matriz de utilizadores.
 */
int **criar_matriz_estatica(Users_catalog *users_catalog);

/**
 * @brief Liberta a memória alocada para a matriz de utilizadores.
 * @param catalogo 
 */
void free_matriz(int **matriz, Users_catalog *users_catalog);

#endif // USERS_CATALOG_H

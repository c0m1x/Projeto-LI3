#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "musics.h"
#include "users.h"
#include "parser.h"
#include "catalogoUser.h"
#include "users.h" 

// Estrutura que representa um catálogo de utilizadores utilizando uma tabela hash.
typedef struct users_catalog{
    GHashTable* users;
    
}Users_catalog;

// Obtém um utilizador do catálogo pelo seu ID, retornando uma cópia do utilizador encontrado.
User* getUser(Users_catalog* catalogo, char* id){
    User* utilizador = g_hash_table_lookup(catalogo->users, id);
    if(!utilizador) return NULL;
    return cloneUser(utilizador);
}

// Adiciona um genero ao array de generos.
void add_genero_User_catalog(Users_catalog* catalogo, char* user_id,int genero){

    User* user = g_hash_table_lookup(catalogo->users, user_id);
    if(!user){
        return;
    }
    add_genero_User(user,genero);
}

// Verifica se um user da querie5 existe.
gboolean user_exists(Users_catalog* catalogo, char* user_id){
    return g_hash_table_contains(catalogo->users, user_id);
}

// Constrói e adiciona um utilizador ao catálogo a partir de tokens.
void buildUser(char* line,char** tokens, Users_catalog* catalogo, void* structure1){
    User* user = completa_User(line,tokens,structure1);
    if(!user) return;
    char* user_id = g_strdup(tokens[0]);
    g_hash_table_insert(catalogo->users, user_id, user);
}

// Wrapper para uso com a função parser, chamando buildUser.
void buildUser_wrapper(char* line, char ** tokens, void * structure1, void * structure2, void * structure3, void * structure4, void * structure5) {
    return buildUser(line, tokens, (Users_catalog*)structure1, structure2);
}

// Cria e carrega um catálogo de utilizadores a partir de um arquivo CSV.
Users_catalog* usersToCatalog(char* path, void* structure) {
    Users_catalog * catalogo = malloc(sizeof(Users_catalog));
    catalogo -> users = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_user_void);
    parser(path, buildUser_wrapper, catalogo, structure, NULL, NULL, NULL);

    return catalogo;
}

// Liberta a memória associada ao catálogo de utilizadores.
void free_users_catalog(Users_catalog * catalogo) {
    g_hash_table_destroy(catalogo -> users);
    free(catalogo);
}

GHashTable* get_users_hash_table(Users_catalog* catalog) {
    if (catalog) {
        return catalog->users;
    }
    return NULL; 
}

//Destroi a tabela hash de utilizadores.
void user_hash_table_destroy(Users_catalog* catalog){
    if(catalog){
        g_hash_table_destroy(catalog->users);
        free(catalog);
    }
}

// Cria uma matriz com os generos de cada utilizador.
int **criar_matriz_estatica(Users_catalog *users_catalog) {
    if (!users_catalog) {
        return NULL;
    }

    GHashTable *users_table = get_users_hash_table(users_catalog);
    if (!users_table) {
        return NULL;
    }

    int num_utilizadores = g_hash_table_size(users_table);
    if (num_utilizadores <= 0) {
        return NULL;
    }

    int **matriz = malloc(num_utilizadores * sizeof(int *));
    if (!matriz) {
        return NULL;
    }
    memset(matriz, 0, num_utilizadores * sizeof(int *));

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, users_table);
    int user_index = 0;

    while (g_hash_table_iter_next(&iter, &key, &value) && user_index < num_utilizadores) {
        User *user_data = (User *)value;
        if (!user_data) {
            continue;
        }

        int *generos_array = get_arrayGeneros(user_data);
        if (!generos_array) {
            continue;
        }

        matriz[user_index] = calloc(10, sizeof(int));
        if (!matriz[user_index]) {
            for (int j = 0; j < user_index; j++) {
                free(matriz[j]);
            }
            free(matriz);
            return NULL;
        }

        memcpy(matriz[user_index], generos_array, 10 * sizeof(int));
        user_index++;
    }

    return matriz;
}

// Liberta a memória alocada para a matriz de utilizadores.
void free_matriz(int **matriz, Users_catalog *users_catalog) {
    if (!matriz || !users_catalog) {
        return;
    }

    GHashTable *users_table = get_users_hash_table(users_catalog);
    if (!users_table) {
        return;
    }

    int num_utilizadores = g_hash_table_size(users_table);
    

    for (int i = 0; i < num_utilizadores; i++) {
        if (matriz[i]) {
            free(matriz[i]);
        }
    }
    
    free(matriz);
}

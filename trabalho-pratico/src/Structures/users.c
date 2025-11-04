#include "users.h"
#include <stdio.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "user_validation.h"
#include "utils.h"
#include "catalogoArtist.h"
#include "catalogoMusic.h"

// Estrutura de dados para utilizadores
typedef struct user{        
    char* email;            
    char* first_name;      
    char* last_name;       
    char* birth_date;      
    char* country;          
    GArray* liked_songs_id;  
    int arrayGeneros[10];
} User;

// Função que devolve um array de generos de um utilizador
int* get_arrayGeneros(User* user){
    return user->arrayGeneros;
}

// Função que adiciona um genero ao utilizador
void add_genero_User(User* user, int genero) {
    user->arrayGeneros[genero] += 1;
}

char* get_email(const User* user) {
    return remover_aspas_dup(user->email);
}

// Função que devolve o primeiro nome de um utilizador
char* get_first_name(const User* user) {
    return remover_aspas_dup(user->first_name);
}

// Função que devolve o último nome de um utilizador
char* get_last_name(const User* user) {
    return remover_aspas_dup(user->last_name);
}

// Função que devolve o país de um utilizador
char* get_country(const User* user) {
    return remover_aspas_dup(user->country);
}

// Função que devolve a idade de um utilizador
int get_age(const User* user) {
    return calc_age(user->birth_date);  
}

// Função que devolve a data de nascimento de um utilizador
char* get_birth_date(const User* user) {
    return strdup(user->birth_date);
}
// Função que devolve o id das músicas gostadas por um utilizador
const GArray* get_liked_songs_id(const User* user) {
    return user->liked_songs_id;
}

// Função que devolve o comprimento da lista de músicas gostadas
int get_liked_songs_id_length(const User* user) {
    return user->liked_songs_id->len;
}

// Função que calcula a idade de um utilizador
int calc_age(const char* birth_date) {

    char date_copy[11];
    if (birth_date[0] == '\"' && birth_date[strlen(birth_date) - 1] == '\"') {
        strncpy(date_copy, birth_date + 1, strlen(birth_date) - 2);
        date_copy[strlen(birth_date) - 2] = '\0';
    } else {
        strncpy(date_copy, birth_date, sizeof(date_copy) - 1);
        date_copy[10] = '\0';  
    }

    char* token = strtok(date_copy, "/");
    int year = 0, month = 0, day = 0;

    if (token != NULL) {
        year = atoi(token);
        token = strtok(NULL, "/");
    }
    if (token != NULL) {
        month = atoi(token);
        token = strtok(NULL, "/");
    }
    if (token != NULL) {
        day = atoi(token);
    }

 
    int idade = 2024 - year;
    if (month > 9 || (month == 9 && day > 9)) {
        idade--;
    }

    return idade;
}

// Função que cria uma cópia de um utilizador
User* cloneUser(User* user) {
    if (!user) return NULL;

    User* new_user = malloc(sizeof(User));
    if (!new_user) return NULL;

    new_user->email = g_strdup(user->email);
    new_user->first_name = g_strdup(user->first_name);
    new_user->last_name = g_strdup(user->last_name);
    new_user->birth_date = g_strdup(user->birth_date);
    new_user->country = g_strdup(user->country);


    GArray* original_liked_songs = user->liked_songs_id;
    if (original_liked_songs) {
        new_user->liked_songs_id = g_array_sized_new(FALSE, FALSE, sizeof(char*), original_liked_songs->len);
        for (guint i = 0; i < original_liked_songs->len; i++) {
            char* song_id = g_strdup(g_array_index(original_liked_songs, char*, i));
            g_array_append_val(new_user->liked_songs_id, song_id);
        }
    } else {
        new_user->liked_songs_id = NULL;
    }

    return new_user;
}


// Função que adiciona uma música à lista de músicas gostadas de um utilizador
void add_liked_song_to_user(User* user, char* song_id) {
    if (user && song_id) {
        char* song_id_dup = g_strdup(song_id);
        g_array_append_val(user->liked_songs_id, song_id_dup); 
    }
}

// Função que liberta a memória associada a um utilizador
void free_user(User* user) {
    if (user) {

        if (user->email) g_free(user->email);
        if (user->first_name) g_free(user->first_name);
        if (user->last_name) g_free(user->last_name);
        if (user->birth_date) g_free(user->birth_date);
        if (user->country) g_free(user->country);

        if (user->liked_songs_id) {
            for (guint i = 0; i < user->liked_songs_id->len; i++) {
                g_free(g_array_index(user->liked_songs_id, char*, i));
            }
            g_array_free(user->liked_songs_id, TRUE);
        }

        g_free(user);
    }
}

// Função que liberta a memória de um utilizador, usado em operações de hash table
void free_user_void(gpointer user){
    free_user(user);
}

// Função que dá parse a uma string de músicas gostadas
GArray* parse_liked_songs(char* liked_songs_str) {
    GArray* liked_songs_array = g_array_new(FALSE, FALSE, sizeof(char*));
    

    char* liked_songs_copy = remover_aspas_dup(liked_songs_str);
    

    char* start = strchr(liked_songs_copy, '[');
    char* end = strrchr(liked_songs_copy, ']');
    if (start && end && start < end) {
        *start = ' ';  
        *end = '\0';  
    }

    char* token = strtok(liked_songs_copy, ", ");
    while (token != NULL) {
        if (token[0] == '\'') token++;  
        if (token[strlen(token) - 1] == '\'') token[strlen(token) - 1] = '\0';  


        char* song_id = g_strdup(token);
        g_array_append_val(liked_songs_array, song_id);
        
        token = strtok(NULL, ", ");
    }

    g_free(liked_songs_copy);
    return liked_songs_array;
}


// Função que valida os tokens de um utilizador
int valida_user_tokens(char** tokens, Musics_catalog* music_catalog) {

    if (validar_email(tokens[1]) == 0) return 1; 
    if (validar_data(tokens[4]) == 0) return 1; 
    if (validar_sub(tokens[6]) == 0) return 1;

    GArray* liked_songs = parse_liked_songs(tokens[7]);
    int liked_songs_validas = validar_liked_songs(liked_songs, music_catalog);


    for (guint i = 0; i < liked_songs->len; i++) {
        char* song_id = g_array_index(liked_songs, char*, i);
        g_free(song_id);
    }
    g_array_free(liked_songs, TRUE);

    if (liked_songs_validas == 0) return 1;

    return 0;
}


// Função que constroi um utilizador a partir de uma linha de dados
User* completa_User(char* line, char** tokens, void* structure1) {
    Musics_catalog* music_catalog = (Musics_catalog*) structure1;


    const char *errors_filepath = "resultados/users_errors.csv";


    if (valida_user_tokens(tokens, music_catalog)==1) {

        FILE *file = fopen(errors_filepath, "a");
        if (!file) {
            perror("Erro ao abrir o ficheiro de erros");
            return NULL;
        }

        write_csv_header_if_new(file, "\"username\";\"email\";\"first_name\";\"last_name\";\"birth_date\";\"country\";\"subscription_type\";\"liked_songs_id\"");

        fprintf(file, "%s", line);
        fclose(file);
        return NULL; 
    }

    User* user = malloc(sizeof(User));
    if (!user) return NULL;

    user->email = g_strdup(tokens[1]);
    user->first_name = g_strdup(tokens[2]);
    user->last_name = g_strdup(tokens[3]);
    user->birth_date = g_strdup(tokens[4]);
    user->country = g_strdup(tokens[5]);
    user->liked_songs_id = funcao(tokens[7]);

    memset(user->arrayGeneros, 0, sizeof(user->arrayGeneros));

    return user;
}


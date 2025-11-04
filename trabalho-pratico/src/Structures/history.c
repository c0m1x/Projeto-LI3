#include <stdio.h>
#include <glib.h>
#include "history.h"
#include "history_validation.h"
#include "../Utilities/utils.h"

// Estrutura que representa um histórico.
typedef struct history{
        char* id;
        char* music_id;
        char* timestamp;
        char* duration;
} History;

// Estrutura que representa um histórico de um utilizador.
typedef struct user_histories {
    GHashTable* histories; 
} UserHistories;

// Estrutura que representa um histórico de um ano.
typedef struct year_histories {
    GHashTable* users; 
} YearHistories;


//Obtém o histórico de um usuário específico num determinado ano a partir de uma tabela hash.
UserHistories* get_users_table_by_year(YearHistories* year_histories, char* user_id){
    return g_hash_table_lookup(year_histories->users, user_id);
}

//Obtém a tabela hash de históricos de um usuário.
GHashTable* get_user_histories_table(UserHistories* user_histories) {
    return user_histories ? user_histories->histories : NULL;
}


//Aloca e inicializa uma estrutura YearHistories, associando um histórico a um usuário se fornecido.
YearHistories* malloc_year(char* user_id, History* history) {
    YearHistories* year = malloc(sizeof(YearHistories));
    if (!year) {
        printf("DEBUG: Falha ao alocar memória para YearHistories.\n");
        return NULL;
    }

    year->users = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_user_histories_void);
    if (!year->users) {
        printf("DEBUG: Falha ao criar tabela hash year->users.\n");
        free(year);
        return NULL;
    }

    if (user_id && history && history->id) {
        UserHistories* user_histories = malloc(sizeof(UserHistories));
        if (!user_histories) {
            g_hash_table_destroy(year->users);
            free(year);
            return NULL;
        }

        user_histories->histories = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_history_void);
        if (!user_histories->histories) {

            free(user_histories);
            g_hash_table_destroy(year->users);
            free(year);
            return NULL;
        }

        g_hash_table_insert(user_histories->histories, g_strdup(history->id), history);

        g_hash_table_insert(year->users, g_strdup(user_id), user_histories);
    } 
    return year;
}


// Adiciona ou atualiza o histórico de um usuário específico dentro de um YearHistories.
void set_user_in_year(YearHistories* year, char* user_id, History* history) {
    if (!year || !user_id || !history || !history->id) return;

    if (!year->users) {
        printf("ERROR: Tabela year->users é NULL ao acessar %s.\n", user_id);
        return;
    }

    UserHistories* user_histories = g_hash_table_lookup(year->users, user_id);
    if (!user_histories) {
        user_histories = malloc(sizeof(UserHistories));
        if (!user_histories) return;

        user_histories->histories = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_history_void);
        g_hash_table_insert(year->users, g_strdup(user_id), user_histories);
    }

    if (!user_histories->histories) {
        printf("ERROR: Tabela user_histories->histories é NULL ao acessar %s.\n", user_id);
        return;
    }
    g_hash_table_insert(user_histories->histories, g_strdup(history->id), history);
}

//Obtém o ID de um histórico.
char* get_history_id(const History* history) {
    return strdup(history->id);
}

//Obtém o mês de um histórico.
char* get_history_month(const History* history) {
    if (!history || !history->timestamp) return NULL;
    
    return strndup(history->timestamp + 1, 2);
}

//Obtém o dia de um histórico.
char* get_history_day(const History* history){
    if (!history || !history->timestamp) return NULL;
    
    return strndup(history->timestamp + 4, 2);
}

//Obtém a hora de um histórico.
char* get_history_hour(const History* history) {
    if (!history || !history->timestamp) return NULL;
    return strndup(history->timestamp + 7, 2);
}

char* get_history_music_id(const History* history) {
    return strdup(history->music_id);
}

//Obtém o timestamp de um histórico.
char* get_history_timestamp(const History* history) {
    return strdup(history->timestamp);
}

//Obtém a duração de um histórico.
char* get_history_duration(const History* history) {
    return strdup(history->duration);
}

//Liberta a memória associada a um histórico de usuário.
void free_user_histories(UserHistories* uh) {
    if (uh) {
        g_hash_table_destroy(uh->histories);
        free(uh);
    }
}

// Wrapper para liberar memória de UserHistories usada como gpointer em tabelas hash.
void free_user_histories_void(gpointer user){
    free_user_histories(user);
}

// Liberta a memória associada à estrutura YearHistories, incluindo sua tabela de usuários.
void free_year_histories(YearHistories* yh) {
    if (yh) {
        g_hash_table_destroy(yh->users);
        free(yh);
    }
}

// Wrapper para liberar memória de YearHistories usada como gpointer em tabelas hash.
void free_year_histories_void(gpointer year){
    free_year_histories(year);
}

// Liberta a memória associada a um histórico individual, incluindo seus campos dinâmicos.
void free_history(History* history) {
    if (history) {
        g_free(history->id);
        g_free(history->music_id);
        g_free(history->timestamp);
        g_free(history->duration);
        g_free(history);
    }
}

// Wrapper para liberar memória de History usada como gpointer em tabelas hash.
void free_history_void(gpointer history){
    free_history(history);
}

//Insere um histórico numa tabela de hash.
void insertHistory(GHashTable* table, History* history) {
    if (table && history) {
        g_hash_table_insert(table, g_strdup(history->id), history);
    }
}

//Procura um histórico na tabela de hash pelo ID.
History* findHistory(GHashTable* table, const char* id) {
    return (History*) g_hash_table_lookup(table, id);
}

//Clona um histórico.
History* cloneHistory(const History* history) {
    if (!history) return NULL;

    History* new_history = malloc(sizeof(History));
    if (!new_history) return NULL;

    new_history->id = g_strdup(history->id);
    new_history->music_id = g_strdup(history->music_id);
    new_history->timestamp = g_strdup(history->timestamp);
    new_history->duration = g_strdup(history->duration);

    return new_history;
}

//Função que valida os tokens de um histórico.
int valida_tokens_history(char** tokens){
    if(is_history_valid(tokens[5])== 1) return 1;
    else return 0;
}

//Completa um histórico a partir de uma linha de dados.
History* completa_History(char* line, char** tokens) {
    const char *errors_filepath = "resultados/history_errors.csv";


    if (valida_tokens_history(tokens)==1) {

        FILE *file = fopen(errors_filepath, "a");
        if (!file) {
            perror("Erro ao abrir o ficheiro de erros");
            return NULL;
        }

        write_csv_header_if_new(file, "\"id\";\"user_id\";\"music_id\";\"timestamp\";\"duration\";\"platform\"");

        fprintf(file, "%s", line);
        
        fclose(file);
        return NULL; 
    }

    History* new_history = malloc(sizeof(History));
    if (!new_history) return NULL; 

    
    new_history->id = g_strdup(tokens[0]);  
    new_history->music_id = g_strdup(tokens[2]); 
    new_history->timestamp = g_strdup(tokens[3] + 5);
    new_history->duration = g_strdup(tokens[4]); 

    return new_history;
}

//Cria uma tabela de hash de históricos de um usuário.
void create_users_table(YearHistories* year) {
    if (!year->users) {
        year->users = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_user_histories_void);
    }
}

//Cria uma tabela de hash de históricos de um usuário.
void create_history_table(YearHistories* year) {
    if (!year || !year->users) return;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, year->users);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        UserHistories* user = (UserHistories*) value;

        if (!user->histories) {
            user->histories = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_history_void);
        }
    }
}







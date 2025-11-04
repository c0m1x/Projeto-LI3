#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "users.h"
#include "catalogoUser.h"
#include "utils.h"
#include "recomendador.h"
#include <math.h>

#define NUM_GENEROS 10

// Géneros musicais.
static const char *generos[] = {
    "Electronic", "Hip Hop", "Metal", "Country", "Blues",
    "Reggae", "Classical", "Jazz", "Pop", "Rock"
};

//Função que calcula a similaridade entre dois utilizadores.
char *querie5(Users_catalog *users_catalog, char *id_utilizador_alvo, int num_recomendacoes, int **matriz) {
    if (!users_catalog || !id_utilizador_alvo || num_recomendacoes <= 0 || !matriz) {
        return strdup("\n");
    }

    size_t id_len = strlen(id_utilizador_alvo);
    char id_utilizador_com_aspas[id_len + 3];
    snprintf(id_utilizador_com_aspas, sizeof(id_utilizador_com_aspas), "\"%s\"", id_utilizador_alvo);

    if (!user_exists(users_catalog, id_utilizador_com_aspas)) {
        return strdup("\n");
    }

    GHashTable *users_table = get_users_hash_table(users_catalog);
    if (!users_table) {
        return strdup("\n");
    }

    int num_utilizadores = g_hash_table_size(users_table);
    if (num_utilizadores <= 0) {
        return strdup("\n");
    }

    char **ids_utilizadores = calloc(num_utilizadores, sizeof(char *));
    if (!ids_utilizadores) {
        return strdup("\n");
    }

    GHashTableIter iter;
    gpointer key, value;
    int user_index = 0;
    g_hash_table_iter_init(&iter, users_table);
    
    while (g_hash_table_iter_next(&iter, &key, &value) && user_index < num_utilizadores) {
        char *user_id = remover_aspas_dup((char *)key);
        if (user_id) {
            ids_utilizadores[user_index++] = user_id;  
        }
    }

    if (num_recomendacoes > num_utilizadores - 1) {
        num_recomendacoes = num_utilizadores - 1;
    }

    char **recomendacoes = recomendaUtilizadores(
        id_utilizador_alvo, matriz, ids_utilizadores,
        (char **)generos, num_utilizadores, NUM_GENEROS,
        num_recomendacoes
    );

    
    char *resultado = NULL;
    if (recomendacoes) {

        size_t total_length = 1;  
        for (int i = 0; i < num_recomendacoes && recomendacoes[i]; i++) {
            total_length += strlen(recomendacoes[i]) + 1; 
        }

        resultado = malloc(total_length);
        if (resultado) {
            resultado[0] = '\0';
            char *ptr = resultado;
            for (int i = 0; i < num_recomendacoes && recomendacoes[i]; i++) {
                ptr += sprintf(ptr, "%s\n", recomendacoes[i]);
                free(recomendacoes[i]);
            }
        }
        free(recomendacoes);
    }

    free(ids_utilizadores);

    return resultado ? resultado : strdup("\n");
}




////////////////////////Versão com o nosso recomendador////////////

// Função que calcula a similaridade entre dois utilizadores.
double calculate_similarity(int *user1, int *user2, int num_genres) {
    double dot_product = 0;
    double norm1 = 0;
    double norm2 = 0;
    
    for (int i = 0; i < num_genres; i++) {
        dot_product += user1[i] * user2[i];
        norm1 += user1[i] * user1[i];
        norm2 += user2[i] * user2[i];
    }
    
    if (norm1 == 0 || norm2 == 0) return 0;
    return dot_product / (sqrt(norm1) * sqrt(norm2));
}

// Função que encontra utilizadores semelhantes.
char **find_similar_users(const char *target_user, int **matrix, char **user_ids, 
                                    int num_users, int num_genres, int num_recommendations) {
    int target_idx = -1;


    for (int i = 0; i < num_users; i++) {
        if (strcmp(user_ids[i], target_user) == 0) {
            target_idx = i;
            break;
        }
    }
    if (target_idx == -1) return NULL;

    int is_target_zero = 1;
    for (int i = 0; i < num_genres; i++) {
        if (matrix[target_idx][i] != 0) {
            is_target_zero = 0;
            break;
        }
    }

    if (is_target_zero) {
        char **zero_users = malloc((num_recommendations + 1) * sizeof(char *));
        int count = 0;

        for (int i = 0; i < num_users && count < num_recommendations; i++) {
            if (i != target_idx) {
                int is_zero = 1;
                for (int j = 0; j < num_genres; j++) {
                    if (matrix[i][j] != 0) {
                        is_zero = 0;
                        break;
                    }
                }
                if (is_zero) {
                    zero_users[count++] = strdup(user_ids[i]);
                }
            }
        }
        zero_users[count] = NULL;
        return zero_users;
    }

    typedef struct {
        double similarity;
        char *user_id;
    } UserSimilarity;

    UserSimilarity *recommendations = malloc(num_recommendations * sizeof(UserSimilarity));
    int recommendation_count = 0;


    for (int i = 0; i < num_users; i++) {
        if (i == target_idx) continue;

        double similarity = calculate_similarity(matrix[target_idx], matrix[i], num_genres);

        if (recommendation_count < num_recommendations) {

            recommendations[recommendation_count++] = (UserSimilarity){similarity, strdup(user_ids[i])};

            for (int j = recommendation_count - 1; j > 0; j--) {
                if (recommendations[j].similarity > recommendations[j - 1].similarity) {
                    UserSimilarity temp = recommendations[j];
                    recommendations[j] = recommendations[j - 1];
                    recommendations[j - 1] = temp;
                }
            }
        } else if (similarity > recommendations[recommendation_count - 1].similarity) {

            free(recommendations[recommendation_count - 1].user_id);
            recommendations[recommendation_count - 1] = (UserSimilarity){similarity, strdup(user_ids[i])};

            for (int j = recommendation_count - 1; j > 0; j--) {
                if (recommendations[j].similarity > recommendations[j - 1].similarity) {
                    UserSimilarity temp = recommendations[j];
                    recommendations[j] = recommendations[j - 1];
                    recommendations[j - 1] = temp;
                }
            }
        }
    }

    char **result = malloc((num_recommendations + 1) * sizeof(char *));
    for (int i = 0; i < recommendation_count; i++) {
        result[i] = strdup(recommendations[i].user_id);
        free(recommendations[i].user_id);
    }
    result[recommendation_count] = NULL;

    free(recommendations);
    return result;
}


// Função que executa a querie 5.
char *querie5_v2(Users_catalog *users_catalog, char *id_utilizador_alvo, int num_recomendacoes, int **matriz) {
    char *resultado = NULL;
    
    if (!users_catalog || !id_utilizador_alvo || num_recomendacoes <= 0 || !matriz) {
        return NULL;
    }
    
    char *id_utilizador_com_aspas = g_strdup_printf("\"%s\"", id_utilizador_alvo);
    if (!id_utilizador_com_aspas) {
        return NULL;
    }
    
    if (user_exists(users_catalog, id_utilizador_com_aspas) == FALSE) {
        resultado = strdup("");
        g_free(id_utilizador_com_aspas);
        return resultado;
    }
    
    GHashTable *users_table = get_users_hash_table(users_catalog);
    if (!users_table) {
        g_free(id_utilizador_com_aspas);
        return NULL;
    }
    
    int num_utilizadores = g_hash_table_size(users_table);
    if (num_utilizadores <= 0) {
        g_free(id_utilizador_com_aspas);
        return NULL;
    }

    char **ids_utilizadores = malloc(num_utilizadores * sizeof(char *));
    if (!ids_utilizadores) {
        g_free(id_utilizador_com_aspas);
        return NULL;
    }
    
    memset(ids_utilizadores, 0, num_utilizadores * sizeof(char *));

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, users_table);
    int user_index = 0;

    while (g_hash_table_iter_next(&iter, &key, &value) && user_index < num_utilizadores) {
        char *user_id1 = (char *)key;
        char *user_id = remover_aspas_dup(user_id1);
        if (user_id) {
            ids_utilizadores[user_index] = strdup(user_id);
            free(user_id);
            user_index++;
        }
    }
    
    if (num_recomendacoes > num_utilizadores - 1) {
        num_recomendacoes = num_utilizadores - 1;
    }
    
    char *target_id = remover_aspas_dup(id_utilizador_alvo);
    char **recomendacoes = find_similar_users(target_id, matriz, 
                                            ids_utilizadores, num_utilizadores, 
                                            NUM_GENEROS, num_recomendacoes);
    free(target_id);
    
    if (recomendacoes) {
        size_t total_length = 1;
        for (int i = 0; i < num_recomendacoes && recomendacoes[i]; i++) {
            total_length += strlen(recomendacoes[i]) + 1;
        }
        
        resultado = malloc(total_length);
        if (resultado) {
            resultado[0] = '\0';
            for (int i = 0; i < num_recomendacoes && recomendacoes[i]; i++) {
                strcat(resultado, recomendacoes[i]);
                strcat(resultado, "\n");
            }
        }
        
        for (int i = 0; i < num_recomendacoes && recomendacoes[i]; i++) {
            free(recomendacoes[i]);
        }
        free(recomendacoes);
    }
    
    g_free(id_utilizador_com_aspas);
    free(ids_utilizadores);
    
    return resultado;
}



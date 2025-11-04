#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glib.h> 
#include "user_validation.h"
#include "users.h"
#include "catalogoMusic.h"
#include "utils.h"
#include "musics.h"
#include "catalogoMusic.h"


int validar_email(const char *email_original) {
    char email_copy[256];
    strncpy(email_copy, email_original, sizeof(email_copy) - 1);
    email_copy[sizeof(email_copy) - 1] = '\0';  

    char *email = remover_aspas_dup(email_copy);

    const char *arroba = strchr(email, '@'); 
    if (!arroba || *arroba == email[0] || *(arroba + 1) == '\0') {
        free(email);
        return 0; 
    }

    const char *dominio = arroba + 1; 
    const char *ponto = strrchr(dominio, '.'); 
    if (!ponto || ponto == dominio || *(ponto + 1) == '\0') {
        free(email);
        return 0; 
    }

    int rstring_tamanho = strlen(ponto + 1);
    if (rstring_tamanho < 2 || rstring_tamanho > 3) {  
        free(email);
        return 0;
    }

    for (int i = 1; ponto[i] != '\0'; i++) {
        if (!islower(ponto[i])) {
            free(email);
            return 0;
        }
    }

    for (const char *p = dominio; p < ponto; p++) {
        if (!islower(*p)) {
            free(email);
            return 0;
        }
    }

    for (const char *p = email; p < arroba; p++) {
        if (!islower(*p) && !isdigit(*p)) {
            free(email);
            return 0; 
        }
    }

    free(email);
    return 1;
}



int validar_data(const char *birth_date) {
    char data_limpa[256];
    strncpy(data_limpa, birth_date, sizeof(data_limpa) - 1);
    data_limpa[sizeof(data_limpa) - 1] = '\0';

    char* data_sem_aspas = remover_aspas_dup(data_limpa);

    int tamanho_data = strlen(data_sem_aspas);
    if (tamanho_data != 10) {
        free(data_sem_aspas);
        return 0;
    }

    if (data_sem_aspas[4] != '/' || data_sem_aspas[7] != '/') {
        free(data_sem_aspas);
        return 0;
    }

    if (!isdigit(data_sem_aspas[0]) || !isdigit(data_sem_aspas[1]) || !isdigit(data_sem_aspas[2]) || !isdigit(data_sem_aspas[3]) ||
        !isdigit(data_sem_aspas[5]) || !isdigit(data_sem_aspas[6]) || !isdigit(data_sem_aspas[8]) || !isdigit(data_sem_aspas[9])) {
        free(data_sem_aspas);
        return 0; 
    }

    int ano = atoi(&data_sem_aspas[0]);
    int mes = atoi(&data_sem_aspas[5]);
    int dia = atoi(&data_sem_aspas[8]);

    if (mes < 1 || mes > 12) {
        free(data_sem_aspas);
        return 0;
    }

    if (dia < 1 || dia > 31) {
        free(data_sem_aspas);
        return 0;
    }

    if (ano >= 2024 || (ano >= 2024 && mes >= 9 && dia >= 10)) {
        free(data_sem_aspas);
        return 0; 
    }

    free(data_sem_aspas);
    return 1; 
}



int validar_sub(const char* subscription_type) {

    char tipo_limpo[256];
    strncpy(tipo_limpo, subscription_type, sizeof(tipo_limpo) - 1);
    tipo_limpo[sizeof(tipo_limpo) - 1] = '\0';


    char* tipo_sem_aspas = remover_aspas_dup(tipo_limpo);

    if (strcmp(tipo_sem_aspas, "normal") != 0 && strcmp(tipo_sem_aspas, "premium") != 0) {
        free(tipo_sem_aspas);
        return 0;
    }
    free(tipo_sem_aspas);
    return 1;
}

int validar_liked_songs(GArray* liked_songs_id, Musics_catalog* music_catalog) {
    if (!liked_songs_id || !music_catalog) {
        return 0;
    }

    for (guint i = 0; i < liked_songs_id->len; i++) {
        char* song_id = g_array_index(liked_songs_id, char*, i);

        size_t len = strlen(song_id);
        char* quoted_song_id = malloc(len + 3);  
        if (!quoted_song_id) {
            return 0; 
        }

        snprintf(quoted_song_id, len + 3, "\"%s\"", song_id);

        if (!music_exists(music_catalog, quoted_song_id)) {
            free(quoted_song_id);
            return 0;
        }

        free(quoted_song_id);
    }
    return 1;
}






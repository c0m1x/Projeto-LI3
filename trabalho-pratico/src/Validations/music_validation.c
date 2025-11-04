#include "music_validation.h"
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include "musics.h"
#include <stdio.h>
#include "artists.h"
#include <ctype.h>





int validar_ano(const char *year) {

    char ano_limpo[256];
    strncpy(ano_limpo, year, sizeof(ano_limpo) - 1);
    ano_limpo[sizeof(ano_limpo) - 1] = '\0';


    for (int i = 0; i < strlen(ano_limpo); i++) {
        if (!isdigit(ano_limpo[i])) {
            return 0; 
        }
    }

    int ano = atoi(ano_limpo); 


    if (ano > 2025) {
        return 0; 
    }

    return 1; 
}


int validacaoduracao(char *duracao) {
    if (duracao == NULL) {
        return 0;  // Se a string de duração for nula, retorna erro
    }

    int h, m, s;
    char ponto1, ponto2;

    // Verifica o formato "hh:mm:ss"
    int parsed_items = sscanf(duracao, "\"%2d%c%2d%c%2d\"", &h, &ponto1, &m, &ponto2, &s);

    if (parsed_items != 5) {
        return 0;  // Se não foram lidos 5 itens, o formato está incorreto
    }

    if (ponto1 != ':' || ponto2 != ':') {
        return 0;  // Se os delimitadores não forem ":" ou não houver os dois pontos
    }

    // Verifica se os valores são válidos
    if (h > 99 || h < 0 || m > 59 || m < 0 || s < 0 || s > 59) {
        return 0;  // Se qualquer valor estiver fora do intervalo esperado
    }

    return 1;
}




int valida_artistas_existentes(Artists_catalog* artist_catalog, GArray* artist_idss) {
    if (!artist_idss) {
        return 0; 
    }
    for (guint i = 0; i < artist_idss->len; i++) {
        char* artist_id = g_array_index(artist_idss, char*, i);
        

        size_t len = strlen(artist_id);
        char* quoted_artist_id = malloc(len + 3); 
        if (!quoted_artist_id) {
            return 0; 
        }
        
        snprintf(quoted_artist_id, len + 3, "\"%s\"", artist_id);
        
        if (artist_Contains(artist_catalog, quoted_artist_id) == FALSE )  {
            free(quoted_artist_id);
            return 0; 
        }
        
        free(quoted_artist_id);
    }
    return 1;
}


int valida_album(Album_catalog* album_catalog, char* album){
    if (album == NULL) {
        return 0; 
    }
    if (strlen(album) == 0) {
        return 0; 
    }
    if (album_Contains(album_catalog, album) == FALSE) {
        return 0; 
    }
    return 1; 
}








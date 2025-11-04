#include "musics.h"
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "music_validation.h"
#include "artists.h"
#include "../Utilities/utils.h"
#include "parser.h"



typedef struct music {                
    GArray* artist_idss;  
    char* album_id;  
    char* duration;          
    char* genre;                              
} Music;

//Função que devolve os IDs dos artistas de uma música
const GArray* get_music_artist_idss(const Music* music) {
    
    return music->artist_idss;
}

//Função que devolve o album de uma música
char* get_music_album(const Music* music){
    return strdup(music->album_id);
}

//Função que devolve a duração de uma música
char* get_music_duration(const Music* music) {
    return music->duration;  
}

//Função que devolve o género de uma música
char* get_music_genre(const Music* music) {
    return strdup(music->genre);
}

//Função que clone uma música
Music* cloneMusica(Music* music) {
    if (!music) return NULL;

    Music* new_music = malloc(sizeof(Music));
    if (!new_music) return NULL;

    new_music->duration = g_strdup(get_music_duration(music));
    new_music->genre = g_strdup(get_music_genre(music));
    new_music->album_id = g_strdup(get_music_album(music));


    const GArray* original_artists = get_music_artist_idss(music);
    if (original_artists) {
        new_music->artist_idss = g_array_sized_new(FALSE, FALSE, sizeof(char*), original_artists->len);
        for (guint i = 0; i < original_artists->len; i++) {
            char* artist_id = g_strdup(g_array_index(original_artists, char*, i));
            g_array_append_val(new_music->artist_idss, artist_id);
        }
    } else {
        new_music->artist_idss = NULL;
    }

    return new_music;
}


//Função que liberta a memória alocada para uma música
void freemusicas(Music* musicas) {
    if (musicas) {

        if (musicas->artist_idss) {
            for (guint i = 0; i < musicas->artist_idss->len; i++) {
                g_free(g_array_index(musicas->artist_idss, char*, i));
            }
            g_array_free(musicas->artist_idss, TRUE);
        }

        g_free(musicas->album_id);
        g_free(musicas->duration);
        g_free(musicas->genre);
        g_free(musicas);
    }
}


//Wrapper para a função de libertar a memória de uma música
void free_music_void(gpointer music){
    freemusicas(music);
}

//Função que procura uma música na tabela hash pelo ID
Music* procmusicatabel(GHashTable* tabelamusica, const char* idsmusica) {
    return (Music*) g_hash_table_lookup(tabelamusica, idsmusica);
}

//Função que converte uma duração de música
int convert_duration(char* duration_str) {

    if ( strlen(duration_str) > 0) {
        int hours = 0, minutes = 0, seconds = 0;
        if (sscanf(duration_str, "%d:%d:%d", &hours, &minutes, &seconds) == 3) {
            return  hours * 3600 + minutes * 60 + seconds;
        } else if (sscanf(duration_str, "%d:%d", &minutes, &seconds) == 2) {
            return minutes * 60 + seconds; 
        } else if (sscanf(duration_str, "%d", &seconds) == 1) {
            return seconds;
        }
    }
    return 0; 
}


//Função que dá parse a uma string de artistas
GArray* parse_artist_ids(char* ids_str) {
    GArray* artist_ids_array = g_array_new(FALSE, FALSE, sizeof(char*));
    

    char* ids_str_copy = remover_aspas_dup(ids_str);
    

    char* start = strchr(ids_str_copy, '[');
    char* end = strrchr(ids_str_copy, ']');
    if (start && end && start < end) {
        *start = ' ';  
        *end = '\0';  
    }


    char* token = strtok(ids_str_copy, ", ");
    while (token != NULL) {

        if (token[0] == '\'') token++;
        if (token[strlen(token) - 1] == '\'') token[strlen(token) - 1] = '\0';
        

        char* id = g_strdup(token);
        g_array_append_val(artist_ids_array, id);
        
        token = strtok(NULL, ", ");
    }

    g_free(ids_str_copy);
    return artist_ids_array;
}



// Função que valida os tokens de uma música
int valida_tokens_musics(char** tokens, Artists_catalog* artists_catalog, Album_catalog* album_catalog) {

    GArray* artist_ids = parse_artist_ids(tokens[2]);
    
    if (valida_artistas_existentes(artists_catalog, artist_ids) == 0) {
        for (guint i = 0; i < artist_ids->len; i++) {
            char* artist_id = g_array_index(artist_ids, char*, i);
            g_free(artist_id);
        }
        g_array_free(artist_ids, TRUE);
        return 1;
    }

    for (guint i = 0; i < artist_ids->len; i++) {
        char* artist_id = g_array_index(artist_ids, char*, i);
        g_free(artist_id);
    }
    g_array_free(artist_ids, TRUE);


    if(valida_album(album_catalog, tokens[3])==0){
        return 1;
    }

    if (validacaoduracao(tokens[4]) == 0) return 1;
    char* temp = remover_aspas_dup(tokens[6]);
    int ano = atoi(temp);
    if (ano <= 0 || ano > 2024){
        free(temp);
        return 1;
    } 
    free(temp);
    return 0;
}

// Converte a duração de uma música para um valor inteiro em segundos.
int get_music_duration_int(Music* music){
    char* duracao = music->duration;
    char duracao_horas[3];
    char duracao_minutos[3];
    char duracao_segundos[3];

    duracao_horas[0]=duracao[1];
    duracao_horas[1]=duracao[2];
    duracao_horas[2]='\0';

    duracao_minutos[0]=duracao[4];
    duracao_minutos[1]=duracao[5];
    duracao_minutos[2]='\0';

    duracao_segundos[0]=duracao[7];
    duracao_segundos[1]=duracao[8];
    duracao_segundos[2]='\0';
    return 3600*atoi(duracao_horas) + 60*atoi(duracao_minutos) + atoi(duracao_segundos);

}



// Função que preenche a estrutura de uma música com tokens
Music* completa_Musica(char* line,char** tokens, void* structure1, void* structure2) {
    Artists_catalog* artist_catalog = (Artists_catalog*) structure1;
    Album_catalog* album_catalog = (Album_catalog*) structure2;
    const char *errors_filepath = "resultados/musics_errors.csv";


    if (valida_tokens_musics(tokens, artist_catalog,album_catalog)==1) {
        FILE *file = fopen(errors_filepath, "a");
        if (!file) {
            perror("Erro ao abrir o ficheiro de erros");
            return NULL;
        }

                                        
        write_csv_header_if_new(file, "\"id\";\"title\";\"artist_ids\";;\"album_id\";\"duration\";\"genre\";\"year\";\"lyrics\"");

        fprintf(file, "%s", line);
        fclose(file);
        return NULL; 
    }
    

    Music* music = malloc(sizeof(Music));
    if (!music) return NULL;
    music->artist_idss = funcao(tokens[2]);
    music->album_id = g_strdup(tokens[3]);
    music->duration = g_strdup(tokens[4]); 
    music->genre = g_strdup(tokens[5]);

    return music; 
}


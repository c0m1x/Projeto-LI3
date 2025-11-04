#include <stdio.h>
#include <stdlib.h>
#include "users.h"
#include "utils.h"
#include "catalogoUser.h"
#include "artists.h"
#include "catalogoArtist.h"

// Função que formata a informação de um utilizador para output.
char* formatUserOutput(const User* user, char separador) {
    if (!user) return NULL;

    char* email = get_email(user);


    char* first_name = get_first_name(user);


    char* last_name = get_last_name(user);


    int age = get_age(user);

    char* country = get_country(user);

    const char* format = (separador == ';') ? "%s;%s;%s;%d;%s\n" : "%s=%s=%s=%d=%s\n";
    int len = snprintf(NULL, 0, format, email, first_name, last_name, age, country);
    char* output = malloc(len + 1);
    if (output) {
        snprintf(output, len + 1, format, email, first_name, last_name, age, country);
    }


    free(email);
    free(first_name);
    free(last_name);
    free(country);

    return output;
}


// Função que formata a informação de um artista para output.
char* formatArtistOutput2(const Artist* artist, char separador){

    if(!artist) return NULL;

        char* name = get_artist_name(artist);
        char* type = get_artist_type(artist);
        char* raw_country = get_artist_country(artist);
        char* country = remover_aspas_dup(raw_country);
        int num_albums = get_artist_num_albums(artist);
        double profit = get_artist_profitTotal(artist);        
        
        
        const char* format = (separador == ';') ? "%s;%s;%s;%d;%.2lf\n" : "%s=%s=%s=%d=%.2lf\n";
        int len = snprintf(NULL, 0, format, name, type,country, num_albums, profit);
        char* output = malloc(len + 1);
        if (output) {
            snprintf(output, len + 1, format, name, type,country, num_albums, profit);
        }
    
    free(name);
    free(type);
    free(raw_country);
    free(country);
    return output;

}

// Função que executa a querie 1.
char* querie1(char* id, Users_catalog* user_catalog, Artists_catalog* artist_catalog, char separador) {
    if (!id || !user_catalog) return NULL;

    char id_com_aspas[64]; 
    snprintf(id_com_aspas, sizeof(id_com_aspas), "\"%s\"", id); 
    char* output = NULL;

    if(id[0] == 'U'){
        User* user = getUser(user_catalog, id_com_aspas);
        if (!user){
            return NULL;
        }

        output = formatUserOutput(user, separador);
        free_user(user);
    }
    else{
        
        Artist* artist = get_artist(artist_catalog, id_com_aspas);
        if (!artist){
            return NULL;
        }
        output = formatArtistOutput2(artist, separador);
        free_artist(artist);
    }
    
    return output;
}

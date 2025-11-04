#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include "command_parser.h"
#include "utils.h"
#include "catalogoUser.h"
#include "catalogoArtist.h"
#include "catalogoMusic.h"
#include "catalogoHistory.h"


int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso: %s <caminho_para_pasta> <inputs-small.txt>\n", argv[0]);
        return 1;
    }

    const char* folder_path = argv[1];
    const char* input_file = argv[2];

    char* artists_file = build_file_path(folder_path, "artists.csv");    
    char* musics_file = build_file_path(folder_path, "musics.csv");
    char* users_file = build_file_path(folder_path, "users.csv");
    char* albums_file = build_file_path(folder_path, "albums.csv");
    char* history_file = build_file_path(folder_path, "history.csv");

    

    Artists_catalog* artists_catalog = artistsToCatalog(artists_file);
    Album_catalog* album_catalog = albumToCatalog(albums_file, artists_catalog);
    Musics_catalog* musics_catalog = musicsToCatalog(musics_file, artists_catalog, album_catalog);
    Users_catalog* users_catalog = usersToCatalog(users_file,musics_catalog);  
    History_catalog* history_catalog = historyToCatalog(history_file, users_catalog, musics_catalog, artists_catalog);
    int **matriz = criar_matriz_estatica(users_catalog);

    
    
    read_input_file(input_file, artists_catalog, album_catalog, history_catalog ,musics_catalog, users_catalog, matriz);


    free(artists_file);
    free(musics_file);
    free(users_file);
    free(albums_file);
    free(history_file);

    free_matriz(matriz, users_catalog);
    free_artists_catalog(artists_catalog);
    free_album_catalog(album_catalog);
    free_musics_catalog(musics_catalog);
    free_users_catalog(users_catalog);
    free_history_catalog(history_catalog);

    return 0;
}

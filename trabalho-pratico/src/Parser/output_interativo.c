#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include "artists.h"
#include "musics.h"
#include "users.h"
#include "command_parser.h"
#include "utils.h"
#include "catalogoUser.h"
#include "catalogoArtist.h"
#include "catalogoMusic.h"
#include "catalogoHistory.h"
#include "querie1.h"
#include "querie2.h"
#include "querie3.h"
#include "querie5.h"
#include "querie6.h"

// Processa um comando e executa a ação correspondente.
char* output(int querie, char separador, char* args, char* args2, char* args3, Artists_catalog* artists_catalog, 
            Musics_catalog* musics_catalog, Users_catalog* users_catalog, History_catalog* history_catalog, int** matriz) {

    if (!args || (querie != 1 && !args2)) {
        return NULL;
    }
    
    char* result = NULL;

    switch(querie) {
        case 1:
            result = querie1(args, users_catalog, artists_catalog, separador);
            break;

        case 2:
            if(strlen(args2) == 0) {
                int args_int = atoi(args);
                result = querie2(artists_catalog, musics_catalog, args_int, NULL, separador);
            } else {
                size_t quoted_args2_size = strlen(args2) + 3;
                char* quoted_args2 = malloc(quoted_args2_size);
                if (!quoted_args2) {
                    return NULL;
                }
                snprintf(quoted_args2, quoted_args2_size, "\"%s\"", args2);                   
                int args_int = atoi(args);
                result = querie2(artists_catalog, musics_catalog, args_int, quoted_args2, separador);
                free(quoted_args2);
            }
            break;

        case 3:
            if(args && args2) {
                int args_int = atoi(args);
                int args2_int = atoi(args2);
                result = querie3(args_int, args2_int, users_catalog, musics_catalog, separador);
            }
            break;

        case 4:

            break;

        case 5: 
            if(args2) {
                int args2_int = atoi(args2);
                result = querie5_v2(users_catalog, args, args2_int, matriz);
            }
            break;

        case 6:

                if(!args3){
                    result = querie6(separador,args,args2,-1, musics_catalog,history_catalog);
                }
                else{
                    int N = atoi(args3);
                    result = querie6(separador,args,args2,N, musics_catalog,history_catalog);
                }
            
           break;
    }

    return result;
}
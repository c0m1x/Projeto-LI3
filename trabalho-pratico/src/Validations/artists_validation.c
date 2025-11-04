#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "artists.h"


int is_artist_valid(char* constituent, char* type) {

    if (strncasecmp(type,"individual", sizeof("individual")-1) == 0) {
        if (strcmp(constituent, "[]") != 0 || strlen(constituent) == 0) {
            return 0;  
        }
    }
    
    if( (strcasecmp(type,"individual") != 0) &&
        (strcasecmp(type,"group") != 0)){
            return 0;
        }
        
    return 1;  
}



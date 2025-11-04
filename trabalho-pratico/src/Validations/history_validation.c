#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "history.h"




int is_history_valid(const char* platform) {
    if (strcmp(platform, "\"mobile\"") == 0 || strcmp(platform, "\"desktop\"") == 0) {

        return 0;
    }

    return 1;
}




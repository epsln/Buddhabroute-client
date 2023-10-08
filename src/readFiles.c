#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/config.h"
#include "include/ini.h"
#include "include/readFiles.h"

const char* getfield(char* line, int num){
	const char* tok;
	for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n")){
		if (!--num)
			return tok;
	}

	return NULL;
}

static int handler(void* params, const char* section, const char* name, const char* value){
    params_t* pconfig = (params_t*)params;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("IMAGE", "resx")) {
        pconfig->resx= atoi(value);
    } else if (MATCH("IMAGE", "resy")) {
        pconfig->resy = atoi(value);
    } else if (MATCH("IMAGE", "resy")) {
        pconfig->maxiter = atoi(value);
    } else if (MATCH("EXPORT", "n_points")) {
        pconfig->n_points = atoi(value);
    } else if (MATCH("EXPORT", "sleep_time")) {
        pconfig->sleep_time = atoi(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

void writeCheckpoint(params_t* params, u_int32_t* histogram){
	for (int i = 0; i < params->resx; i++){
		for (int j = 0; j < params->resy; j++){
			printf("%u ",histogram[j * params->resx + i] );
		}
	}
	printf("\n");
}

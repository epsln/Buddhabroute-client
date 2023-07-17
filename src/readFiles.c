#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/config.h"
#include "include/readFiles.h"

const char* getfield(char* line, int num){
	const char* tok;
	for (tok = strtok(line, ",");
			tok && *tok;
			tok = strtok(NULL, ",\n")){
		if (!--num)
			return tok;
	}
	return NULL;
}

void readCheckpoint(params_t *params, u_int32_t* histogram){
	char line[8192];
	FILE* stream = fopen("checkpoint.txt", "r");
	if (stream == NULL) return;

	for (int i = 0; i < params->resx; i++){
		fgets(line, 8192, stream);
		for (int j = 0; j < params->resy; j++){
			char* tmp = strdup(line);
			histogram[i * params->resy + j] = atoi(getfield(tmp, j));
			free(tmp);
		}
	}
	fclose(stream);
}

void writeCheckpoint(params_t* params, u_int32_t* histogram){
	FILE* stream = fopen("checkpoint.txt", "w+");
	for (int i = 0; i < params->resx; i++){
		for (int j = 0; j < params->resy; j++){
			fprintf(stream, "%u,",histogram[i * params->resy + j] );
		}
		fprintf(stream, "\n");
	}
	fclose(stream);
}

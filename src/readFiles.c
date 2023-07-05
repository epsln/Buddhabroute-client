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

void readCheckpoint(int histogram, t_params params){
	char line[8192];
	int i = 0;
	FILE* stream = fopen("checkpoint.txt", "r");
	for (int i = 0; i < params.resx; i++){
		fgets(line, 8192, stream);
		for (int j = 0; j < params.resy; j++){
			char* tmp = strdup(line);
			histogram[i][j] =  getfield(tmp, j);
			free(tmp);
		}
	}
	fclose(stream);	
}

void readCheckpoint(uint32_t histogram, t_params params){
	FILE* stream = fopen("checkpoint.txt", "w+");
	for (int i = 0; i < p_params.resx; i++){	
		for (int j = 0; j < p_params.resy; j++){	
			sprintf("%u,", histogram[i][j]);
		}
	}
	fclose(stream);	
}

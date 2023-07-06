#ifndef readFiles
#define readFiles
#include "config.h"
#include <stdlib.h>

void readCheckpoint(params_t *p_params, u_int32_t* histogram);
void writeCheckpoint(params_t *p_params, u_int32_t* histogram);

#endif

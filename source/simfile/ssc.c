#include "simfile.h"

#include <stdio.h>
#include <stdlib.h>

#define WARN_SSC puts("SSC is not supported.");

/* TODO: probably not gonna happen until way later */
simfile_t* _ssc_load(FILE* file) { WARN_SSC return NULL; }
void _ssc_export(simfile_t* simfile, FILE* file) { WARN_SSC }

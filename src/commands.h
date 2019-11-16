#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include "modules/data_structures/static_list.h"
#include "modules/sig/object.h"
#include "modules/sig/block.h"
#include "modules/sig/equipment.h"
#include "modules/sig/geometry.h"
#include "modules/util/files.h"
#include "modules/util/svg.h"
#include "query.h"
#include "data.h"

// Processa o arquivo .geo e o .qry, se tiver, escrevendo os resultados nos arquivos de saída
// void processAll(FILE *entryFile, FILE *outputSVGFile, FILE *outputQryFile, FILE *queryFile, 
//                 FILE *txtFile, char outputDir[], char svgFileName[]);

void processAll(Files files);

void processAndGenerateQuery(Files files);

#endif
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "main.h"
#include "Container.h"
#include "../Commons/header/cmm.h"
#include "queryHandler.h"
#include "../Engine/header/visibility.h"

typedef enum fileTYPE{
    READONLY, WRITE, APPEND
} fileTYPE;

/*
    o fileHandler é responsável pelo loop de executação para percorrer os arquivos .geo e .qry
*/

FILE* abrirArquivo(char* path, fileTYPE a);

void processaComandoGeo(char* comando, Runtime rt);

void processaComandoQry(char* comando, Runtime rt, FILE *svgFile, FILE *respostaFile, char* DirComNomeBase);

void handleGeo(FILE* file, Runtime rt, char* path);

void handleQry(FILE* file, Runtime rt, char* pathSVG, char* pathTXT, char* nomeBase);


#endif
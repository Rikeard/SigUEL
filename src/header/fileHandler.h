#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "main.h"

typedef enum fileTYPE{
    READONLY, WRITE, APPEND
} fileTYPE;


void handleGeo(FILE* file, treeNode *raiz, char* path);
void processamentoComandoBB(treeNode *nd, FILE *svg, char* cor);
void processaComandoQRY(char* comando, treeNode* raiz, FILE *svgFile, FILE *respostaFile, char* DirComNomeBase);
FILE* abrirArquivo(char* path, fileTYPE a);
void handleQry(FILE* file, treeNode* raiz, char* pathSVG, char* pathTXT, char* nomeBase);
void processamentoComandoPRINT(treeNode* nd, FILE *svg);

#endif
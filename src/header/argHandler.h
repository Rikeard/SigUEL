#ifndef ARGHANDLER_H
#define ARGHANDLER_H

#include "main.h"

char* obterNome(char* arquivoEntrada);
void argumentoInicial(int argc, char *argv[], char** diretorioEntrada, char **diretorioSaida, char **arquivoEntrada, char **arquivoConsulta);

#endif
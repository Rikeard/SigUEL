#ifndef ARGHANDLER_H
#define ARGHANDLER_H

#include "main.h"
#include "../Commons/header/cmm.h"

char* obterNome(char* arquivoEntrada);
void argumentoInicial(int argc, char *argv[], char** diretorioEntrada, char **diretorioSaida, char **arquivoEntrada, char **arquivoConsulta);
void montagemDePathQRY(char* diretorioEntrada, char* diretorioSaida, char* arquivoConsulta, char* nomeEntrada, char* nomeConsulta, char** fullpathQRY, char** fullpathSVG2, char** fullpathTXT, char** fullpathBB);
void montagemDePathGEO(char* diretorioEntrada, char* diretorioSaida, char* arquivoEntrada, char *nomeEntrada, char** fullpathGEO, char **fullpathSVG);

#endif
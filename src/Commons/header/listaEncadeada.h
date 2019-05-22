#ifndef LISTAENCADEAD_H
#define LISTAENCADEAD_H

#include "../../header/main.h"
#include "../../Commons/header/cmm.h"

typedef void* listaStatic;


listaStatic listaStatic_new(int tamanho);

void listaStatic_add(listaStatic lista, void* elemento);

void listaStatic_setSize(listaStatic lista, int tamanho);

void listaStatic_forEach(listaStatic lista, void* function, void* argumento);

void listaStatic_stats(listaStatic lista);

void* listaStatic_find(listaStatic lista, void* funcaoAchar, void* argumento);

bool listaStatic_remove(listaStatic lista, void* funcaoAchar, void* argumento, void* funcaoDeletar);

#endif

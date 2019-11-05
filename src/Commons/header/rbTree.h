#ifndef RBTREE_H
#define RBTREE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "cmm.h"

typedef void* treeRB;

treeRB criarArvore(int (*comparator)(void *e1, void* e2));
void* buscarElemento(treeRB tree, void* chave);
void forEach(treeRB tree, void* function, void* argumento);
void adicionarElemento(treeRB tree, void* elemento, void* chave);
bool removerElemento(treeRB tree, void* chave);


#endif

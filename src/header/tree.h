#ifndef TREE_H
#define TREE_H

#include "main.h"
#include "svg.h"
#include "free.h"

//Nó padrão da árvore
typedef struct treeNode{
    svgObject *objeto;
    int *id; // = objeto->id;
    treeNode* maior;
    treeNode* menor;
}treeNode;

void adicionarElemento(treeNode* raiz, svgObject* objeto);
treeNode* buscarElemento(treeNode* raiz, int id);
treeNode* inicializarArvore();
void processamentoExterno(treeNode* node, FILE* svg, char *cor, void (*funct)(treeNode *nd, FILE *svg, char* cor));
int* intToPointer(int x);
void treeToPrint(treeNode* node, FILE* svg, void (*funct)(treeNode *nd, FILE *svg));
int pointerToInt(int* x);
void liberarArvore(treeNode* node);

#endif
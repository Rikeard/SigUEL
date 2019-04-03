#include "header/tree.h"

int* intToPointer(int x){
    int* a = malloc(sizeof(int));
    *a = x;
    return a;
}

int pointerToInt(int* x){
    return *x;
}

treeNode* inicializarArvore(){
    treeNode* a = malloc(sizeof(treeNode));
    a->id = NULL;  
    return a;
}

treeNode* criarElemento(svgObject* objeto){
    treeNode* a = malloc(sizeof(treeNode));
    a->id = intToPointer(objeto->id);
    a ->objeto = objeto;
    a->menor = NULL;
    a->menor = NULL;
    return a;
}

void alterarElemento(treeNode* a, svgObject* objeto){
    liberarSVGobject(a->objeto);
    free(a->id);
    a->id = intToPointer(objeto->id);
    a->objeto = objeto;
    return;
}

treeNode* buscarElemento(treeNode* raiz, int id){
    bool done = false;

    treeNode* current = raiz;

    while(!done){

        if(current == NULL){
            reportError(__func__, "Elemento não encontrado");
            return NULL;
        }
        
        if(pointerToInt(current->id) == id){
            return current;
        }else{

            if(pointerToInt(current->id) < id){
                current = current->maior; 
            }else{
                  current = current->menor; 
            }

        }
    }
}

void processamentoExterno(treeNode* node, FILE* svg, char *cor, void (*funct)(treeNode *nd, FILE *svg, char* cor)){
    if(node == NULL){
        return;
    }else{
        funct(node, svg, cor);      
        processamentoExterno(node->maior, svg, cor, funct);
        processamentoExterno(node->menor, svg, cor, funct);
    }
}


void treeToPrint(treeNode* node, FILE* svg, void (*funct)(treeNode *nd, FILE *svg)){
    if(node == NULL){
        return;
    }else{
        funct(node, svg);      
        treeToPrint(node->maior, svg, funct);
        treeToPrint(node->menor, svg, funct);
    }
}

void liberarArvore(treeNode* node){
    if(node == NULL){
        return;
    }else{
        liberarArvore(node->maior);
        liberarArvore(node->menor);
        liberarTreeNode(node);
    }
}


void adicionarElemento(treeNode* raiz, svgObject* objeto){
    int id = objeto->id;
    //Inicializar primeiro elemento
    if(raiz->id == NULL){
        raiz->id = intToPointer(id);
        raiz->objeto = objeto;
        raiz->maior = NULL;
        raiz->menor = NULL;
        return;
    }

    //Outros elementos
    treeNode* current = raiz;
    treeNode* anterior = NULL;
    bool done = false;
    
    while(!done){
        //Vai para esquerda
        if(id < pointerToInt(current->id)){

            anterior = current;
            current = current->menor;

            if(current == NULL){
                anterior->menor = criarElemento(objeto);
                return;
            }

        }
        //Vai para direita
        else if(id > pointerToInt(current->id)){

            anterior = current;
            current = current->maior;

            if(current == NULL){
                anterior->maior = criarElemento(objeto);
                return;
            }

        }
        //Substitui o node atual
        else if(id == pointerToInt(current->id)){
            alterarElemento(current, objeto);
            return;
        }
        //WTF
        else{
            reportError(__func__, "Não foi possível determinar o ID");
        }
    }

}


#include "header/listaEncadeada.h"

typedef struct Node{
    int anterior;
    int proximo;
    bool disponivel;
    void* elemento;
}Node;

typedef struct listaEncadeada{
    int tamanho;
    int primeiro;
    int emUso;
    int ultimo;
    Node *lista;
} listaEncadeada;

void formatList(listaStatic lista){
    listaEncadeada* ls = lista;

    for(int i = 0; i < ls->tamanho; i++){
        Node *xs = &(ls->lista[i]);
        
        xs->anterior = i-1;
        xs->proximo = i+1;
        xs->disponivel = true;
        xs->elemento = NULL;

        if(i == 0){
            xs->anterior = -1;
        }else if(i == ls->tamanho-1){
            xs->proximo = -1;
        }

    }

    // for(int i = 0; i < ls->tamanho; i++){
    //     Node xs = ls->lista[i];
    //     printf("ELEMENTO %d: ANT: %d PROX:%d\n", i, xs.anterior, xs.proximo);
    // }
}

listaStatic listaStatic_new(int tamanho){
    listaEncadeada* ls = malloc(sizeof(listaEncadeada));
    ls->tamanho = tamanho;
    ls->primeiro = 0;
    ls->lista = malloc(sizeof(Node)*tamanho);
    ls->ultimo = 0;
    ls->emUso = 0;

    formatList(ls);

    return ls;
}


void listaStatic_stats(listaStatic lista){
    listaEncadeada *x = lista;
    printf("Status da Lista Estática:\n");
    printf("    Usado: %d/%d\n", x->emUso, x->tamanho);
    printf("    Posição disponível atual: %d\n", x->ultimo);
}

void listaStatic_forEach(listaStatic lista, void* function, void* argumento){
    listaEncadeada *x = lista;

    void (*apply)(void *atual, void* arg) = function;

    int index = x->primeiro;

    while(index != x->ultimo){
        Node nd = x->lista[index];
        if(nd.elemento != NULL)
            apply(nd.elemento, argumento);

        index = nd.proximo;
    }

}

void listaStatic_setSize(listaStatic lista, int tamanho){
    listaEncadeada *x = lista;
    if(x->emUso == 0){
        free(x->lista);
        x->lista = malloc(sizeof(Node)*tamanho);
        x->tamanho = tamanho;
        formatList(x);
    }else{
        reportError(__func__, "Lista ja inicializada");
    }
}

void listaStatic_add(listaStatic lista, void* elemento){
    listaEncadeada* x = lista;

    //Verifica se o tamanho excede
    if(x->emUso + 1 > x->tamanho){
        reportError(__func__, "Tamanho excedido");
        return;
    }

    Node *nd = &x->lista[x->ultimo];

    if(!nd->disponivel)
        printf("VISHHHHHHHHHHHHHH\n");

    nd->elemento = elemento;
    nd->disponivel = false;

    x->ultimo = nd->proximo;

    //Aumenta quantidade em uso
    x->emUso++;


}

void* listaStatic_find(listaStatic lista, void* funcaoAchar, void* argumento){
    listaEncadeada* x = lista;

    bool (*find)(void *elemento, void* arg) = funcaoAchar;

    int index = x->primeiro;

    while(index != x->ultimo){
        Node nd = x->lista[index];

        if(find(nd.elemento, argumento)){
            return nd.elemento;
        }
        index = nd.proximo;
    }
    
    reportError(__func__, "Nenhum elemento encontrado");
    return NULL;
    
}

bool listaStatic_remove(listaStatic lista, void* funcaoAchar, void* argumento, void* funcaoDeletar){
    
    listaEncadeada* x = lista;
    bool (*find)(void *elemento, void* arg) = funcaoAchar;
    //void (*delete)(void* elemento) = funcaoDeletar;

    if(x->emUso - 1 < 0){
        reportError(__func__, "A lista ja está vazia");
        return;
    }

    int index = x->primeiro;

    while(index != x->ultimo){
        Node *current = &(x->lista[index]);

        if(find(current->elemento, argumento)){
            printf("iniciando delete de %d\n", index);
            //delete(current->elemento);
            current->elemento = NULL;
            current->disponivel = true;
            x->emUso--;

            int atualmenteUltimo = x->ultimo;

            //Trata caso eu seja o primeiro da lista, transformando o meu próximo como o começo dela
            if(index == x->primeiro){
                x->primeiro = current->proximo;

                Node *xd = &(x->lista[x->primeiro]);
                xd->anterior = -1;
                return;

            //Se eu for o último da lista, não muda muita coisa
            }else if(index == x->lista[x->ultimo].anterior){
                x->ultimo = index;

            //Se eu tiver no meio do lista, faz meu anterior apontar pro meu próximo e meu próximo apontar pro anterior
            }else{
                Node *ant = &(x->lista[current->anterior]);
                ant->proximo = current->proximo;

                Node *prox = &(x->lista[current->proximo]);
                prox->anterior = current->anterior;
            }

            //Agora nós o colocamos no final da lista, amarrando a ultima posição usada indicando proximo para essa.
            //E a próxima dessa como pro próximo espaço disponível
            
            Node *livre = &(x->lista[atualmenteUltimo]);

            if(livre->anterior != -1){
                Node *ultUsado = &(x->lista[livre->anterior]);

                ultUsado->proximo = index;   
                current->anterior = livre->anterior;
            }


            current->proximo = atualmenteUltimo;

            livre->anterior = index;

            x->ultimo = index;

            return true;


        }

        index = current->proximo;
    }

    return false;
    reportError(__func__, "Falha no delete");
}
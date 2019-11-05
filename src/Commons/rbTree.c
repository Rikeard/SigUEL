#include "header/rbTree.h"

enum Color{
  RED, BLACK
}typedef Color;

struct treeNode{
  struct treeNode* pai;
  struct treeNode* esq;
  struct treeNode* dir;
  void* chave;
  void* elemento;
  Color cor;
}typedef treeNode;

struct treeRoot{
  treeNode* raiz;
  int (*comparator)(void *e1, void* e2);
  int tamanho;
  long long idGen;
  bool idGenMode;
}typedef treeRoot;

long long pointerToNumber(void* e1){
  long long* e2 = e1;
  return *e2;
}

int defaultComparator(void* e1, void* e2){
  long long e1v = pointerToNumber(e1);
  long long e2v = pointerToNumber(e2);

  return (e1v - e2v);
}

treeRB criarArvore(int (*comparator)(void *e1, void* e2)){
  treeRoot *arvore = malloc(sizeof(treeRoot));
  arvore->raiz = NULL;
  arvore->comparator = comparator;
  arvore->tamanho = 0;

  if(comparator == NULL){
    arvore->idGen = 0;
    arvore->comparator = defaultComparator;
    arvore->idGenMode = true;
  }else{
    arvore->idGenMode = false;
  }

  return arvore;
}

void* generateID(treeRoot* rb){
  long long* a = malloc(sizeof(long long));
  *a = rb->idGen;
  rb->idGen++;
  return a;
}

treeNode* getUncle(treeNode* node){
  treeNode* pai = node->pai;
  treeNode* grandparent = pai->pai;

  treeNode* dir = grandparent->dir;
  treeNode* esq = grandparent->esq;

  if(pai == dir){
    return esq;
  }else{
    return dir;
  }

}

bool isRight(treeNode* node){
	treeNode* pai = node->pai;

	treeNode* dir = pai->dir;
  treeNode* esq = pai->esq;

	if(node == dir){
		return true;
	}else{
		return false;
	}
}

Color getCor(treeNode* node){
  if(node == NULL){
    return BLACK;
  }else{
    return node->cor;
  }
}

void swapColor(treeNode* g, treeNode* p){
		//Swap colors de g e p
	Color cx = p->cor;
	Color cg = g->cor;

	p->cor = cg;
	g->cor = cx;
}

void isNull(void* check, char* st){
  if(check == NULL){
    printf("É null - %s\n", st);
  }
}

treeNode* getRight(treeNode* p){
  if(p != NULL){
    return p->dir;
  }else{
    return NULL;
  }
}

treeNode* getLeft(treeNode* p){
  if(p != NULL){
    return p->esq;
  }else{
    return NULL;
  }
}

void rightRotate(treeRoot* arvore, treeNode* p){
  treeNode* x = getLeft(p);
  treeNode* subT5 = getRight(p);
  treeNode* subT3 = getLeft(x);
  treeNode* subT4 = getRight(x);
  treeNode* pai = p->pai;


  if(pai != NULL){
    if(pai->dir == p){
      pai->dir = x;
    }else{
      pai->esq = x;
    }
  }else{
    arvore->raiz = x;
  }

  x->esq = subT3;
  x->dir = p;
  x->pai = pai;

  

  p->pai = x;
  p->esq = subT4;

  if(subT4 != NULL){
    subT4->pai = p;
  }

  p->dir = subT5;

	
}

void leftRotate(treeRoot* arvore, treeNode* p){
  treeNode* x = p->dir;
  treeNode* subT1 = getLeft(p);
  treeNode* subT2 = getLeft(x);
  treeNode* subT3 = getRight(x);
  treeNode* pai = p->pai;

  if(pai != NULL){
    if(pai->dir == p){
      pai->dir = x;
    }else{
      pai->esq = x;
    }
  }else{
    arvore->raiz = x;
  }
 
  
  x->pai = pai;
  x->esq = p;
  x->dir = subT3;

  p->pai = x;
  p->dir = subT2;

  if(subT2 != NULL){
    subT2->pai = p;
  }

  p->esq = subT1;

}

void* buscarElemento(treeRB tree, void* chave){
   treeRoot* arvore = tree;

  //Verifica se a árvore é nula
  if(arvore->raiz == NULL){
      reportError(__func__, "Árvore nula");
    return NULL;
  }

  treeNode* current = arvore->raiz;

  while(true){
    if(current == NULL){
      reportError(__func__, "Não encontrado");
      return NULL;
    }

    int resultado = arvore->comparator(current->chave, chave);

    if(resultado == 0){
      return current->elemento;
    }else if(resultado < 0){
      current = current->dir;
    }else if(resultado > 0){
      current = current->esq;
    }

  }

}

void forEach(treeRB tree, void* function, void* argumento){
   treeRoot* arvore = tree;

    void (*apply)(void *atual, void* arg) = function;

    _forEachLoop(arvore->raiz, apply, argumento);

}

void _forEachLoop(treeNode* node, void (*apply)(void *atual, void* arg), void* argumento){
  if(node == NULL){
    return;
  }else{
    if(node->elemento != NULL)
      apply(node->elemento, argumento);

    _forEachLoop(node->esq, apply, argumento);
    _forEachLoop(node->dir, apply, argumento);
  }
}


void ensureRules(treeRoot* tree, treeNode *node){
  print_t(((treeRoot*) tree)->raiz);
  bool setup = false;
  
  if(node != tree->raiz && getCor(node->pai) == RED && node->pai != NULL){
    setup = true;
    treeNode* pai = node->pai;
    treeNode* uncle = getUncle(node);
	  treeNode* grandparent = pai->pai;

    if(getCor(uncle) == RED){
      //(i) Change color of parent and uncle as BLACK.
      pai->cor = BLACK;
      uncle->cor = BLACK;
      //(ii) color of grand parent as RED.
      //(iii) Change x = x’s grandparent, repeat steps 2 and 3 for new x.
      while(grandparent != NULL && grandparent != tree->raiz){
        grandparent->cor = RED;
        
        if(grandparent->pai != NULL){
            grandparent = grandparent->pai->pai;
        }else{
            break;
        }
      }

    }else{ //Uncle is BLACK

		if(!isRight(pai)){
			if(!isRight(node)){
				//Left Left case
        printf("Left left case\n");
				rightRotate(tree, grandparent);
				swapColor(pai, grandparent);

			}else{
				//Left Right case
        printf("Left right case\n");
				leftRotate(tree, pai);
          print_t(((treeRoot*) tree)->raiz);
				rightRotate(tree, grandparent);
          print_t(((treeRoot*) tree)->raiz);
				swapColor(node, grandparent);

			}
		}else{
			if(isRight(node)){
				//Right Right case
        printf("Right right case\n");
				leftRotate(tree, grandparent);
				swapColor(pai, grandparent);

			}else{
				//Right left case
        printf("Right left case\n");
        rightRotate(tree, pai);
				leftRotate(tree, grandparent);
				swapColor(node, grandparent);
			}

		}


    }
  }

  if(setup){
      ensureRules(tree, node);
  }else if(node->pai != NULL){
      if(node->pai->pai != NULL)
        ensureRules(tree, node->pai->pai);
  }
    
}

void adicionarElemento(treeRB tree, void* elemento, void* chave){
      printf("Adicionando %s\n", chave);
  treeRoot* arvore = tree;

  if(arvore->idGenMode){
    chave = generateID(tree); 
  }

  treeNode* nd = malloc(sizeof(treeNode));
  nd->elemento = elemento;
  nd->chave = chave;
  nd->cor = RED;
  nd->esq = NULL;
  nd->dir = NULL;

  arvore->tamanho++;


  //Verifica se a árvore é nula
  if(arvore->raiz == NULL){
    arvore->raiz = nd;
    nd->cor = BLACK;
    nd->pai = NULL;
    return;
  }

  treeNode* last = arvore->raiz;
  treeNode* current = arvore->raiz;

  while(true){
    int resultado = arvore->comparator(current->chave, chave);

    if(resultado < 0){
      //Vai para a direita
      last = current;
      current = current->dir;

      if(current == NULL){
        last->dir = nd;
        nd->pai = last;
        break;
      }

    }else if(resultado > 0){
      //Vai para a esquerda
      last = current;
      current = current->esq;

      if(current == NULL){
        last->esq = nd;
        nd->pai = last;
        break;
      }

    }else if(resultado == 0){
      //Encontrou (deu = 0)
      current->elemento = elemento;
      reportError(__func__, "Elemento duplicado");
      free(nd);
      arvore->tamanho--;

      return;

    }

  }

  ensureRules(tree, nd);
  print_t(((treeRoot*) tree)->raiz);
  
}

void printarArvore(treeNode* node, int level){
  if(node == NULL){
    return;
  }else{
    for(int i = 0; i < level; i++){
      printf("  ");
    }
    printf("%s", node->chave);
    printf("\n");
    printarArvore(node->esq, level - 1);
    printarArvore(node->dir, level - 1);
  }
}

int _print_t(treeNode *tree, int is_left, int offset, int depth, char s[20][255]){
    char b[20];
    int width = 6;

    if (!tree) return 0;

    sprintf(b, " %s-%c ", tree->chave, tree->cor == RED ? 'R' : 'B');
    //sprintf(b, " %.3s-%.3s ", tree->chave, tree->pai == NULL ? "(N)" : tree->pai->chave);

    int left  = _print_t(tree->esq,  1, offset,                depth + 1, s);
    int right = _print_t(tree->dir, 0, offset + left + width, depth + 1, s);

    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }

    return left + width + right;
}

void print_t(treeNode *tree){
    char s[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    _print_t(tree, 0, 0, 0, s);

    for (int i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}

/* static void leftRotate(treeRoot* tree, treeNode* node) {
    treeNode* y = node->dir;

    node->dir = y->esq;
    if (y->esq != NULL)
        y->esq->pai = node;

    y->pai = node->pai;

    if (node->pai == NULL)
        tree->raiz = y;
    else if (node->pai->esq == node)
        node->pai->esq = y;
    else
        node->pai->dir = y;
    
    y->esq = node;
    node->pai = y;
}

static void rightRotate(treeRoot* tree, treeNode* node) {
    treeNode* y = node->esq;

    node->esq = y->dir;
    if (y->dir != NULL)
        y->dir->pai = node;

    y->pai = node->pai;

    if (node->pai == NULL)
        tree->raiz = y;
    else if (node->pai->esq == node)
        node->pai->esq = y;
    else
        node->pai->dir = y;
    
    y->dir = node;
    node->pai = y;
} */

void ensureDelete(treeRoot* tree, treeNode* node) {
    treeNode* replacement;
    treeNode* x;
    treeNode* w; // x's sibling

    bool xLeftChild;

    // First step
    if (node->esq == NULL && node->dir == NULL) {
        // two nil children (caso a)
        replacement = NULL;
        x = replacement;
        x->pai = node->pai;
        if (node->pai != NULL && node->pai->esq == node)
            xLeftChild = true;
        else
            xLeftChild = false;
        
    } else if (node->esq == NULL && node->dir != NULL) {
        // one nil child on the left (caso b)
        replacement = node->dir;
        x = replacement;
        x->pai = node->pai;
        xLeftChild = false;
    } else if (node->esq != NULL && node->dir == NULL) {
        // one nil child on the right (caso c)
        replacement = node->esq;
        x = replacement;
        x->pai = node->pai;
        xLeftChild = true;
    } else {
        // zero nil children (caso d)
        replacement = node->dir;
        // find successor to node
        while (replacement->esq != NULL)
            replacement = replacement->esq;
        x = replacement->dir;
        if (replacement != node->dir) {
            //replacement->pai->esq = x;
            replacement->esq = node->esq;
            replacement->dir = node->dir;
            replacement->esq->pai = replacement;
            replacement->dir->pai = replacement;
            x->pai = replacement->pai;
            x->pai->esq = x;
            xLeftChild = true;
        } else {
            x->pai = replacement;
            replacement->esq = node->esq;
            node->esq->pai = replacement;
            xLeftChild = false;
        }
    }

    // replace removed node with its replacement
    if (node->pai != NULL) {
        if (node->pai->esq == node)
            node->pai->esq = replacement;
        else
            node->pai->dir = replacement;
    } else {
        tree->raiz = replacement;
    }

    if (replacement != NULL) {
        replacement->pai = node->pai;
    }


    // Second step
    if (node->cor == RED && (replacement->cor == RED || replacement == NULL))
        return;
    else if (node->cor == RED && replacement->cor == BLACK) {
        replacement->cor = RED;
        // proceed to the appropriate case
    } else if (node->cor == BLACK && replacement->cor == RED) {
        replacement->cor = BLACK;
        return;
    }

    // Third step (cases)
    while (x != tree->raiz) {
        // find x's sibling
        if (xLeftChild)
            w = x->pai->dir;
        else
            w = x->pai->esq;
        
        if (x->cor == RED) {
            // Case 0
            x->cor = BLACK;
            return;
        } else {
            if (w->cor == RED) {
                // Case 1
                w->cor = BLACK;
                x->pai->cor = RED;
                if (xLeftChild) {
                    leftRotate(tree, x->pai);
                    w = x->pai->dir;
                } else {
                    rightRotate(tree, x->pai);
                    w = x->pai->esq;
                }
            }
            if (w->cor == BLACK) {
                if (w->esq->cor == BLACK && w->dir->cor == BLACK) {
                    // Case 2
                    w->cor = RED;
                    x = x->pai;
                    xLeftChild = x == x->pai->esq;
                    if (x->cor == RED) {
                        x->cor = BLACK;
                        return;
                    }
                    continue;
                } else if (xLeftChild && w->esq->cor == RED && w->dir->cor == BLACK
                        || !xLeftChild && w->dir->cor == RED && w->esq->cor == BLACK) {
                    // Case 3
                    w->cor = RED;
                    if (xLeftChild) {
                        w->esq->cor = BLACK;
                        rightRotate(tree, w);
                        w = x->pai->dir;
                    } else {
                        w->dir->cor = BLACK;
                        leftRotate(tree, w);
                        w = x->pai->esq;
                    }
                }
                // Case 4
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                if (x->pai->esq == x) {
                    w->dir->cor = BLACK;
                    leftRotate(tree, x->pai);
                } else {
                    w->esq->cor = BLACK;
                    rightRotate(tree, x->pai);
                }
                return;
            }
        }
    }
}


bool removerElemento(treeRB tree, void* chave){
  treeRoot* arvore = tree;

  //Verifica se a árvore é nula
  if(arvore->raiz == NULL){
    printf("Arvore nula\n");
    return false;
  }

  treeNode* current = arvore->raiz;

  while(true){
    if(current == NULL){
      reportError(__func__, "Elemento não encontado\n");
      return false;
    }

    int resultado = arvore->comparator(current->chave, chave);

    if(resultado == 0){
      ensureDelete(tree, current);
      arvore->tamanho--;
      return true;
    }else if(resultado < 0){
      current = current->dir;
    }else if(resultado > 0){
      current = current->esq;
    }

  }

  
}




/* int main(void) {
  treeRB arvre = criarArvore(strcmp);
  adicionarElemento(arvre, "027", "10");
    adicionarElemento(arvre, "237", "20");
      adicionarElemento(arvre, "978", "30");
      adicionarElemento(arvre, "181", "15");
            adicionarElemento(arvre, "385", "15");
                  adicionarElemento(arvre, "692", "15");
                       adicionarElemento(arvre, "500", "15");
                            adicionarElemento(arvre, "031", "15");
                                 adicionarElemento(arvre, "519", "15");
                                      adicionarElemento(arvre, "537", "15");
  adicionarElemento(arvre, "C", "A");
    adicionarElemento(arvre, "B", "B");
      adicionarElemento(arvre, "X", "C");
        adicionarElemento(arvre, "Z", "D");
          adicionarElemento(arvre, "A", "E");
            adicionarElemento(arvre, "D", "F");
              adicionarElemento(arvre, "G", "G");
              adicionarElemento(arvre, "K", "G");
              adicionarElemento(arvre, "E", "G");
              adicionarElemento(arvre, "L", "G");
              adicionarElemento(arvre, "O", "G");
              adicionarElemento(arvre, "M", "G");
              adicionarElemento(arvre, "F", "G"); 

  printarArvore(((treeRoot*) arvre)->raiz, 4);
  print_t(((treeRoot*) arvre)->raiz);
  removerElemento(arvre, "500");
    print_t(((treeRoot*) arvre)->raiz);

  return 0;
} */
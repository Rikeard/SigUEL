#include "header/free.h"

void liberarTreeNode(treeNode *node){
    liberarSVGobject(node->objeto);
    free(node->id);
    free(node);
}

void liberarSVGobject(svgObject *obj){
    switch(obj->tipo){
        case CIRCLE:
            {
                liberarCircle(obj->elemento);
                break;
            }
        case RECTANGLE:
            {
                liberarRectangle(obj->elemento);
                break;
            }
        case ELLIPSE:
            {
                liberarEllipse(obj->elemento);
                break;
            }
        case TEXT:
            {
                liberarText(obj->elemento);
                break;
            }
        case LINE:
            {
                liberarLine(obj->elemento);
                break;
            }
        default:
            {
                reportError(__func__, "Tipo não tratado");
            }

    }

    if(obj->corBorda != NULL)
        free(obj->corBorda);
    if(obj->corPreenchimento != NULL)
        free(obj->corPreenchimento);
    if(obj->style != NULL)
        free(obj->style);

    free(obj);
}

void liberarCircle(Circle *c1){
    liberarPoint(c1->coordenada);
    free(c1);
}

void liberarRectangle(Rectangle *t){
    liberarPoint(t->coordenada);
    free(t);
}

void liberarText(Text *t){
    liberarPoint(t->coordenada);
    free(t->texto);
    free(t);
}

void liberarLine(Line *l){
    liberarPoint(l->coordenada2);
    liberarPoint(l->coordenada1);
    free(l);
}

void liberarEllipse(Ellipse *el){
    liberarPoint(el->coordenada);
    free(el);
}

void liberarPoint(Point *i){
    free(i);
}

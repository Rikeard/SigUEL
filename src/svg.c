#include "header/svg.h"

SVG* inicializarContainerSVG(int width, int height){
    SVG *objeto = calloc(1, sizeof(SVG));

    //Inicializacao de valores estáticos
    objeto->width = width;
    objeto->height = height;

    //Inicializacao do header e ending
    char *initialize = calloc(1000, sizeof(char));
    sprintf(initialize, "<svg width=\"%d\" height=\"%d\">", width, height);
    objeto->header = calloc(strlen(initialize)+1, sizeof(char));
    strcpy(objeto->header, initialize);

    objeto->ending = calloc(8, sizeof(char));    
    sprintf(objeto->ending, "</svg>");

    objeto->data = NULL;

    return objeto;
}

char* exportarSVG(SVG *objeto){

    if(objeto == NULL){
        reportError(__func__, "SVG passado é nulo");
        return NULL;
    }

    int tamanho = 0;

    if(objeto->header != NULL && objeto->ending != NULL){
        tamanho += strlen(objeto->header);
        tamanho += strlen(objeto->ending);
    }else{
        reportError(__func__, "header ou ending tem valor nulo");
    }

    if(objeto->data != NULL){
        tamanho += strlen(objeto->data);
    }else{
        reportError(__func__, "char* data é nulo");
    }

    char* saida = calloc(tamanho, sizeof(char));

    strcpy(saida, objeto->header);

    strcat(saida, "\n");

    if(objeto->data != NULL){
        strcat(saida, objeto->data);
    }

    strcat(saida, "\n");

    strcat(saida, objeto->ending);
    
    //Teste unitário 
    // 30 é o valor de um header + ending com data vazio
    if(strlen(saida) >= 30){
        reportSucess(__func__);
    }

    return saida;
}

svgObject* criarSVGObject(geometryType tipo, int id, void* elemento, char* corBorda, char* CorPreenchimento, char* style){
    svgObject* obj = malloc(sizeof(svgObject));
    obj->tipo = tipo;
    obj->id = id;
    obj->elemento = elemento;
    
    char *cb = NULL, *cp = NULL, *st = NULL;

    if(corBorda != NULL){
        if(strlen(corBorda) > 1){
            cb = malloc((strlen(corBorda)+1)*sizeof(char));
            sprintf(cb, "%s", corBorda);
        }
    }

    if(CorPreenchimento != NULL ){
        if(strlen(CorPreenchimento) > 1){
            cp = malloc((strlen(CorPreenchimento)+1)*sizeof(char));
            sprintf(cp, "%s", CorPreenchimento);
        }
    }

    if(style != NULL){
        if(strlen(style) > 1){
            st = malloc((strlen(style)+1)*sizeof(char));
            sprintf(st, "%s", style);
        }
    }

    obj->corBorda = cb;
    obj->corPreenchimento = cp;
    obj->style = st;


}
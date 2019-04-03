#include "header/fileHandler.h"
#include "header/geometry.h"

char* toString(svgObject* objeto){
    char buffer[512];

    switch(objeto->tipo){
        case RECTANGLE:
            {
                Rectangle *rt = objeto->elemento;
                if(objeto->style == NULL)
                    sprintf(buffer, "<rect x=\"%lf\" y=\"%lf\"  width=\"%lf\" height=\"%lf\" style=\"fill:%s;stroke:%s;\"/>\n", rt->coordenada->x, rt->coordenada->y, rt->width, rt->height, objeto->corPreenchimento, objeto->corBorda);
                else
                    sprintf(buffer, "<rect x=\"%lf\" y=\"%lf\"  width=\"%lf\" height=\"%lf\" style=\"fill:%s;stroke:%s;%s\"/>\n", rt->coordenada->x, rt->coordenada->y, rt->width, rt->height, objeto->corPreenchimento, objeto->corBorda, objeto->style);
                break;
            }
        case CIRCLE:
            {
                Circle *rt = objeto->elemento;
                if(objeto->style == NULL)
                    sprintf(buffer, "<circle cx=\"%lf\" cy=\"%lf\"  r=\"%lf\" style=\"fill:%s;stroke:%s;\"/>\n", rt->coordenada->x, rt->coordenada->y, rt->raio, objeto->corPreenchimento, objeto->corBorda);
                else
                    sprintf(buffer, "<circle cx=\"%lf\" cy=\"%lf\"  r=\"%lf\" style=\"fill:%s;stroke:%s;%s\"/>\n", rt->coordenada->x, rt->coordenada->y, rt->raio, objeto->corPreenchimento, objeto->corBorda, objeto->style);
                
            }
            break;

        case TEXT:
            {
                Text *rt = objeto->elemento;
                sprintf(buffer, "<text x=\"%lf\" y=\"%lf\" fill=\"black\">%s</text>\n", rt->coordenada->x, rt->coordenada->y, rt->texto);
            }
            break;

        case LINE:
            {
                Line *rt = objeto->elemento;
                if(objeto->style == NULL)
                    sprintf(buffer, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"fill:%s;stroke:%s;\"/>\n", rt->coordenada1->x, rt->coordenada1->y, rt->coordenada2->x, rt->coordenada2->y, objeto->corPreenchimento, objeto->corBorda);
                else
                    sprintf(buffer, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"fill:%s;stroke:%s;%s\"/>\n", rt->coordenada1->x, rt->coordenada1->y, rt->coordenada2->x, rt->coordenada2->y, objeto->corPreenchimento, objeto->corBorda, objeto->style);

            }
            break;

        case ELLIPSE:
            {
                Ellipse *rt = objeto->elemento;
                if(objeto->style == NULL)
                    sprintf(buffer, "<ellipse cx=\"%lf\" cy=\"%lf\" rx=\"%lf\" ry=\"%lf\" style=\"fill:%s;stroke:%s;\"/>\n", rt->coordenada->x, rt->coordenada->y, rt->raioX, rt->raioY, objeto->corPreenchimento, objeto->corBorda);
                else
                    sprintf(buffer, "<ellipse cx=\"%lf\" cy=\"%lf\" rx=\"%lf\" ry=\"%lf\" style=\"fill:%s;stroke:%s;%s\"/>\n", rt->coordenada->x, rt->coordenada->y, rt->raioX, rt->raioY, objeto->corPreenchimento, objeto->corBorda, objeto->style);

            }
            break;

        default:
            reportError(__func__, "Elemento com tipo não determinado");
            break;
    }
    char *resp = malloc(sizeof(char)*(strlen(buffer)+1));
    sprintf(resp, "%s", buffer);
    return resp;
}

void iniciarSVGFILE(char* path){
    
    FILE* print = abrirArquivo(path, WRITE);

    //Inicializa SVG
    fprintf(print,"<svg>\n");
    fclose(print);    
}

void adicionarSVGFile(svgObject* a, FILE* f){
    char *buffer = toString(a);
    fprintf(f, "%s", buffer);
    free(buffer);
}

void finalizarSVGFILE(char* path){
    FILE* print = abrirArquivo(path, APPEND);
    fprintf(print, "</svg>");
    fclose(print);
}
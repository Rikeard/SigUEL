#include "header/fileMaker.h"

char* toString(svgObject objeto){
    char *buffer = calloc(512, sizeof(char));

    void *rt = svgObject_getElemento(objeto);
    char* corBorda = svgObject_getCorBorda(objeto);
    char* corPreenchimento = svgObject_getCorPreenchimento(objeto);

    char* style = NULL;
    bool possuiStyle = !svgObject_haveStyle(objeto);
    if(!possuiStyle){
        style = svgObject_getStyle(objeto);
    }

    if(svgObject_getClasse(objeto) == DEFAULT){
        switch(svgObject_getTipo(objeto)){

            case RECTANGLE:
                {
                    Point cord = Rectangle_getCoordenada(rt);
                    double x = Point_getX(cord);
                    double y = Point_getY(cord);
                    double l = Rectangle_getLargura(rt);
                    double a = Rectangle_getAltura(rt);


                    if(possuiStyle)
                        sprintf(buffer, "<rect x=\"%lf\" y=\"%lf\"  width=\"%lf\" height=\"%lf\" style=\"fill:%s;stroke:%s;\"/>\n", x, y, l, a, corPreenchimento, corBorda);
                    else
                        sprintf(buffer, "<rect x=\"%lf\" y=\"%lf\"  width=\"%lf\" height=\"%lf\" style=\"fill:%s;stroke:%s;%s\"/>\n", x, y, l, a, corPreenchimento, corBorda, style);
                }
                break;
            case CIRCLE:
                {
                    Point cord = Circle_getCoordenada(rt);
                    double x = Point_getX(cord);
                    double y = Point_getY(cord);
                    double r = Circle_getRaio(rt);

                    if(possuiStyle)
                        sprintf(buffer, "<circle cx=\"%lf\" cy=\"%lf\"  r=\"%lf\" style=\"fill:%s;stroke:%s;\"/>\n", x, y, r, corPreenchimento, corBorda);
                    else
                        sprintf(buffer, "<circle cx=\"%lf\" cy=\"%lf\"  r=\"%lf\" style=\"fill:%s;stroke:%s;%s\"/>\n", x, y, r, corPreenchimento, corBorda, style);
                    
                }
                break;

            case TEXT:
                {
                    Point cord = Text_getCoordenada(rt);
                    double x = Point_getX(cord);
                    double y = Point_getY(cord);
                    char* txt = Text_getText(rt);

                    if(possuiStyle)
                        sprintf(buffer, "<text x=\"%lf\" y=\"%lf\" fill=\"black\">%s</text>\n", x, y, txt);
                    else
                        sprintf(buffer, "<text x=\"%lf\" y=\"%lf\" style=\"%s\">%s</text>\n", x, y, style, txt);
                }
                break;

            case LINE:
                {
                    Point cord1 = Line_getCoordenada1(rt);
                    Point cord2 = Line_getCoordenada2(rt);
                    double x1 = Point_getX(cord1);
                    double y1 = Point_getY(cord1);
                    double x2 = Point_getX(cord2);
                    double y2 = Point_getY(cord2);


                    if(possuiStyle)
                        sprintf(buffer, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"fill:%s;stroke:%s;\"/>\n", x1, y1, x2, y2, corPreenchimento, corBorda);
                    else
                        sprintf(buffer, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"fill:%s;stroke:%s;%s\"/>\n", x1, y1, x2, y2, corPreenchimento, corBorda, style);

                }
                break;

            case ELLIPSE:
                {
                    Point cord = Ellipse_getCoordenada(rt);
                    double x = Point_getX(cord);
                    double y = Point_getY(cord);
                    double rx = Ellipse_getRaioX(rt);
                    double ry = Ellipse_getRaioY(rt);

                    if(possuiStyle)
                        sprintf(buffer, "<ellipse cx=\"%lf\" cy=\"%lf\" rx=\"%lf\" ry=\"%lf\" style=\"fill:%s;stroke:%s;\"/>\n", x, y, rx, ry, corPreenchimento, corBorda);
                    else
                        sprintf(buffer, "<ellipse cx=\"%lf\" cy=\"%lf\" rx=\"%lf\" ry=\"%lf\" style=\"fill:%s;stroke:%s;%s\"/>\n", x, y, rx, ry, corPreenchimento, corBorda, style);

                }
                break;
            
            case TRIANGLE:
                {
                    Point p1 = Triangle_getP1(rt);
                    Point p2 = Triangle_getP2(rt);
                    Point p3 = Triangle_getP3(rt);

                    double x1 = Point_getX(p1), y1 = Point_getY(p1);
                    double x2 = Point_getX(p2), y2 = Point_getY(p2);
                    double x3 = Point_getX(p3), y3 = Point_getY(p3);

                    if(possuiStyle)
                        sprintf(buffer, "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf\" style=\"fill:%s;stroke:%s;\" fill-opacity=\"0.5\" />\n", x1, y1, x2, y2, x3, y3, corPreenchimento, corBorda);
                    else
                        sprintf(buffer, "<polygon points=\"%lf,%lf %lf,%lf %lf,%lf\" style=\"fill:%s;stroke:%s;%s\" fill-opacity=\"0.5\" />\n", x1, y1, x2, y2, x3, y3, corPreenchimento, corBorda, style);
                }
                break;

            default:
                reportError(__func__, "Elemento com tipo não determinado");
                break;
        }
    }else{
        switch(svgObject_getClasse(objeto)){
            case QUADRA:
                {
                    Point cord = Rectangle_getCoordenada(rt);
                    double x = Point_getX(cord);
                    double y = Point_getY(cord);
                    double l = Rectangle_getLargura(rt);
                    double a = Rectangle_getAltura(rt);


                    if(possuiStyle)
                        sprintf(buffer, "<rect x=\"%lf\" y=\"%lf\"  width=\"%lf\" height=\"%lf\" style=\"fill:%s;stroke:%s;\"/>\n", x, y, l, a, corPreenchimento, corBorda);
                    else
                        sprintf(buffer, "<rect x=\"%lf\" y=\"%lf\"  width=\"%lf\" height=\"%lf\" style=\"fill:%s;stroke:%s;%s\"/>\n", x, y, l, a, corPreenchimento, corBorda, style);

                    svgObject txt = svgObject_new(TEXT, DEFAULT,  NULL, Text_new(x,y+16,svgObject_getId(objeto)),NULL,NULL,"font-size:16;z-index:100;");
                    char *xt = toString(txt);
                    strcat(buffer, xt);
                }
                break;
            case RADIO:
                {
                    Point cord = Circle_getCoordenada(rt);
                    double x = Point_getX(cord);
                    double y = Point_getY(cord);
                    double r = Circle_getRaio(rt);

                    if(possuiStyle)
                        sprintf(buffer, "<circle cx=\"%lf\" cy=\"%lf\"  r=\"%lf\" style=\"fill:%s;stroke:%s;\"/>\n", x, y, r, corPreenchimento, corBorda);
                    else
                        sprintf(buffer, "<circle cx=\"%lf\" cy=\"%lf\"  r=\"%lf\" style=\"fill:%s;stroke:%s;%s\"/>\n", x, y, r, corPreenchimento, corBorda, style);
                    
                    svgObject txt = svgObject_new(TEXT, DEFAULT, NULL, Text_new(x-3.6,y+2,"RB"),NULL,NULL,"font-size:6;z-index:100;");
                    char *xt = toString(txt);
                    strcat(buffer, xt);

                }
                break;

            case SEMAFORO:
                {
                    Point cord = Circle_getCoordenada(rt);
                    double x = Point_getX(cord);
                    double y = Point_getY(cord);
                    double r = Circle_getRaio(rt);

                    if(possuiStyle)
                        sprintf(buffer, "<circle cx=\"%lf\" cy=\"%lf\"  r=\"%lf\" style=\"fill:%s;stroke:%s;\"/>\n", x, y, r, corPreenchimento, corBorda);
                    else
                        sprintf(buffer, "<circle cx=\"%lf\" cy=\"%lf\"  r=\"%lf\" style=\"fill:%s;stroke:%s;%s\"/>\n", x, y, r, corPreenchimento, corBorda, style);
                    
                    svgObject txt = svgObject_new(TEXT, DEFAULT, NULL, Text_new(x-3,y+3,"S"),NULL,NULL,"font-size:10;z-index:100;");
                    char *xt = toString(txt);
                    strcat(buffer, xt);

                }
                break;

            case HIDRANTE:
                {
                    Point cord = Circle_getCoordenada(rt);
                    double x = Point_getX(cord);
                    double y = Point_getY(cord);
                    double r = Circle_getRaio(rt);

                    if(possuiStyle)
                        sprintf(buffer, "<circle cx=\"%lf\" cy=\"%lf\"  r=\"%lf\" style=\"fill:%s;stroke:%s;\"/>\n", x, y, r, corPreenchimento, corBorda);
                    else
                        sprintf(buffer, "<circle cx=\"%lf\" cy=\"%lf\"  r=\"%lf\" style=\"fill:%s;stroke:%s;%s\"/>\n", x, y, r, corPreenchimento, corBorda, style);
                    
                    svgObject txt = svgObject_new(TEXT, DEFAULT, NULL, Text_new(x-3.2,y+3,"H"),NULL,NULL,"font-size:9;z-index:100;");
                    char *xt = toString(txt);
                    strcat(buffer, xt);

                }
                break;
            case PREDIO:
                {
                    Point cord = Rectangle_getCoordenada(rt);
                    double x = Point_getX(cord);
                    double y = Point_getY(cord);
                    double l = Rectangle_getLargura(rt);
                    double a = Rectangle_getAltura(rt);

                    if(possuiStyle)
                        sprintf(buffer, "<rect x=\"%lf\" y=\"%lf\"  width=\"%lf\" height=\"%lf\" style=\"fill:%s;stroke:%s;\"/>\n", x, y, l, a, corPreenchimento, corBorda);
                    else
                        sprintf(buffer, "<rect x=\"%lf\" y=\"%lf\"  width=\"%lf\" height=\"%lf\" style=\"fill:%s;stroke:%s;%s\"/>\n", x, y, l, a, corPreenchimento, corBorda, style);
                }

                break;
            case MURO:
                {
                    Point cord1 = Line_getCoordenada1(rt);
                    Point cord2 = Line_getCoordenada2(rt);
                    double x1 = Point_getX(cord1);
                    double y1 = Point_getY(cord1);
                    double x2 = Point_getX(cord2);
                    double y2 = Point_getY(cord2);

                    if(possuiStyle)
                        sprintf(buffer, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"fill:%s;stroke:%s;\"/>\n", x1, y1, x2, y2, corPreenchimento, corBorda);
                    else
                        sprintf(buffer, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"fill:%s;stroke:%s;%s\"/>\n", x1, y1, x2, y2, corPreenchimento, corBorda, style);

                }
                break;

            default:
                reportError(__func__, "Classe não encontrada");
                break;
        }
    }


    char *resp = calloc((safeStrlen(buffer)+1), sizeof(char));
    sprintf(resp, "%s", buffer);
    free(buffer);
    return resp;
}

/*
    Abre o arquivo SVG e inicia com <svg>
*/

void iniciarSVGFILE(char* path){
    
    FILE* print = abrirArquivo(path, WRITE);

    //Inicializa SVG
    fprintf(print,"<svg xmlns=\"http://www.w3.org/2000/svg\">\n");
    fclose(print);    
}

/*
    Printa o svgObject dentro do File
*/

void adicionarSVGFile(svgObject a, FILE* f){
    char *buffer = toString(a);
    fprintf(f, "%s", buffer);
    free(buffer);
}

/*
    Fecha o SVG com </svg>
*/
void finalizarSVGFILE(char* path){
    FILE* print = abrirArquivo(path, APPEND);
    fprintf(print, "</svg>");
    fclose(print);
}
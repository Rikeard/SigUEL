#include "header/svgObject.h"
#include "../Commons/header/cmm.h"

void SVG_Liberar(geometryType x, void* elemento){
    switch(x){
        case RECTANGLE:
            Rectangle_liberar(elemento);
            break;
        case CIRCLE:
            Circle_liberar(elemento);
            break;
        case ELLIPSE:
            Ellipse_liberar(elemento);
            break;
        case LINE:
            Line_liberar(elemento);
            break;
        case TEXT:
            Text_liberar(elemento);
            break;
        case POINT:
            Point_liberar(elemento);
            break;
        default:
            reportError(__func__, "Tipo não tratado");
            break;
    }
}
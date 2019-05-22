#include "Commons/header/cmm.h"
#include "Commons/header/listaEncadeada.h"
#include "Elementos/SVG/header/Text.h"
#include "header/Teste.h"

bool achar(void* argumento, void* elemento){
    return stringEquals(Text_getText(argumento), elemento);
}

// void main(){
//     listaStatic lista = listaStatic_new(100);
//     Text a = Text_new(10, 10, "vsf");
//     listaStatic_add(lista, a);
//     listaStatic_add(lista, Text_new(5,5, "aye"));
//     char* pt = "vsf";

//     Text as = listaStatic_find(lista, achar, pt);
//     if(as == NULL){
//         printf("FK\n");
//     }
//     Point x = Text_getCoordenada(as);
//     printf("%lf -- %lf\n", Point_getX(x), Point_getY(x));
// }
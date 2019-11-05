#ifndef TEXT_H
#define TEXT_H

#include "Point.h"
#include <stdio.h>
#include "../../../Commons/header/cmm.h"

typedef void* Text;


Text Text_new(double x, double y, char* txt);

char* Text_getText(Text tt);

Point Text_getCoordenada(Text t);

void Text_liberar(Text t);

#endif
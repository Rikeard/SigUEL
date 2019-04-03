#ifndef FREE_H
#define FREE_H

#include "main.h"
#include "geometry.h"
#include "tree.h"

void liberarCircle(Circle *c1);

void liberarRectangle(Rectangle *t);

void liberarText(Text *t);
void liberarLine(Line *l);

void liberarEllipse(Ellipse *el);

void liberarPoint(Point *i);

void liberarSVGobject(svgObject *obj);
void liberarTreeNode(treeNode *node);
#endif
#ifndef ST_H
#define ST_H

#include <stdio.h>
#include <stdlib.h>

#include "vertex.h" //deve avere KEYget ITEMdisplay

typedef struct symboltable *ST; 

ST STnew();
void STinit(ST st, int numV);
void STfree(ST st);
void STinsert(ST st, Vert v);
int STsearchByName(ST st, char* name);
Vert* STgetByIndex(ST st, int id);
Vert STdelete(ST st, int id);

int* STorderByName(ST st);
int STcount(ST st);
//void STremove();
//STcopy?

#endif

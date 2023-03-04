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
int STsearch(ST st, char* vLabel);
Vert* STselect(ST st, int id);
Vert STdelete(ST st, int id);
int STcount(ST st);

void STorderByName(ST st);
void STcopy(ST dst,ST src);
//void STremove();

#endif

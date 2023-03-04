#ifndef E3_VERTEX_H
#define E3_VERTEX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define V_NAME_SIZE 20

typedef struct vertex {
   char name[V_NAME_SIZE];
}Vert;

char* VERTgetName(Vert* v);
int VERTisNull(Vert v);
Vert VERTgetNull();
void VERTprint(FILE *fp,Vert v);
void VERTread(FILE *fp,Vert* v);
int VERTcmp(Vert v1,Vert v2);
void VERTset(Vert *v,char *name);

#endif


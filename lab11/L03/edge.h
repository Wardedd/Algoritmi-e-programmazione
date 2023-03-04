#ifndef EDGE_H
#define EDGE_H

#include <stdio.h>

typedef struct edge { //edge da v a w
   int v,w,weight;
}Edge;

int EDGEgetStart(Edge e);
int EDGEgetWeight(Edge e);
int EDGEgetEnd(Edge e);
void printEdge(FILE *fp,Edge e);
void EDGEset(Edge *e,int v,int w,int weight);

#endif


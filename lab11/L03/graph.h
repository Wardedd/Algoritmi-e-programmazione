#ifndef GRAPH_H
#define GRAPH_H
#include <stdio.h>

#include "vertex.h"
#include "st.h"
#include "edge.h"
#include "support.h"

typedef struct G *Graph;

Graph GRAPHnew();
Graph GRAPHinit();
void GRAPHfree(Graph g);

Graph GRAPHload(FILE *fp);

void GRAPHprintOrder(FILE *fp,Graph g);
void GRAPHprintListAdj(FILE *fp, Graph g);
int GRAPHSubThree(Graph g,char *a,char *b,char *c);

#endif

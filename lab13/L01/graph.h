#ifndef GRAPH_H
#define GRAPH_H
#include <stdio.h>

#include "vertex.h"
#include "st.h"
#include "edge.h"
#include "support.h"

#define MIN_WEIGHT 0

typedef struct G *Graph;

Graph GRAPHnew();
Graph GRAPHinit();
void GRAPHfree(Graph g);


Graph GRAPHload(FILE *fp);

void GRAPHprintOrder(FILE *fp,Graph g);
void GRAPHprintListAdj(FILE *fp, Graph g);
int GRAPHSubThree(Graph g,char *a,char *b,char *c);
Graph GRAPHgenerateDag(Graph g);
void GRAPHdagPrintMaxPaths(Graph g);

Graph GRAPHgetMinDag(Graph g);
int GRAPHdagCheck(Graph g);

#endif

#include "edge.h"

int EDGEgetStart(Edge e){
	return e.v;
}

int EDGEgetWeight(Edge e){
	return e.weight;
}
int EDGEgetEnd(Edge e){
	return e.w;
}

void printEdge(FILE *fp,Edge e){
	fprintf(fp,"v:%d v:%d weight:%d",e.v,e.w,e.weight);
}

void EDGEset(Edge *e,int v,int w,int weight){
	e->v=v;
	e->w=w;
	e->weight=weight;
}

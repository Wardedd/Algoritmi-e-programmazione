#include <stdio.h>

#include "support.h"
#include "graph.h"

#define FILE_NAME_SIZE 30

int main(){
    char fileName[FILE_NAME_SIZE];
    Graph graph;
    FILE *fp;

    printf("Inserisci il nome del file: ");
    scanf("%s",fileName);

    openFile(&fp,fileName,"r");
    graph=GRAPHload(fp);

    printf("Grafo ordinato pesato connesso\n");
    GRAPHprintListAdj(stdout, graph);
    printf("\n-----------\nDAG\n");

    Graph dag=GRAPHgetMinDag(graph);
    if(dag==graph)
        printf("Grafo è già un dag\n");

    GRAPHprintListAdj(stdout, dag);
    GRAPHdagPrintMaxPaths(dag);

    if(dag!=graph)
        GRAPHfree(dag);
    GRAPHfree(graph);
    return 0;
}


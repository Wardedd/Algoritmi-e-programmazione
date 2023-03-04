/* *
 2 ricorsioni
 - dati elementi genero diagonali
 - dati diagonali genero programma
 * */

#include <stdio.h>
#include <stdlib.h>
#include "support.h"

#include "diag.h"
#include "prog.h"

#define MAX_FILENAME 50
#define PROG_S 3

int main(){
    int nElems, DD, DP;
    char fileName[MAX_FILENAME]="elementi.txt";
    FILE *fp;

    elem* elems;
    pdiagArr diags;
    pprog bestp;

    openFile(&fp,fileName,"r");
    readIndex(fp,&nElems);
    printf("Inserire DD DP: ");
    if(scanf("%d %d",&DD,&DP)!=2)
        return 1;
    elems=malloc(nElems*sizeof(elem));
    if(elems==NULL) exit(-5);
    readElems(fp,elems,nElems);

    diags = generateDiags(elems,nElems,DD);
    bestp = generateProg(diags,DP);

    printProg(stdout,bestp);

    diagArrFree(diags);
    free(elems);
    return 0;
}

//
// Created by carmine on 16/12/20.
//

#include "diag.h"

static int checkResult(pdiag diag);
static int checkIteration(pdiag diag,elem* newElem,int k,int DD);

void readElem(FILE *fp,elem* elem){
    if(fscanf(fp,"%s %u %u %u %u %u %lf %d ",elem->nome,
              &elem->type,&elem->dirIn,&elem->dirOut,&elem->isSecond,&elem->isLast,
              &elem->val,&elem->dif)!=8){
        printf("Errore lettura elemento");
        exit(1);
    }
}

void printElem(FILE *fp,elem* elem){
    fprintf(fp,"%s %d %d %d %d %d %lf %d",elem->nome,
            elem->type,elem->dirIn,elem->dirOut,elem->isSecond,elem->isLast,
            elem->val,elem->dif);
    exit(1);
}

void readElems(FILE *fp,elem *elems,int nElems){
    for(int i=0;i<nElems;i++)
        readElem(fp,&elems[i]);
}

void printDiag(FILE *fp,pdiag diag){
    for(int i=0;i<diag->size;i++){
        printElem(fp,diag->arr[i]);
        fprintf(fp,"\n");
    }
}

static int diagCheckResult(pdiag diag){
    //non checko il primo elemento, già gestito per definizione in generateDiags
    //DD già checkato in checkIteration
    //if (diag->arr[diag->size-1]->isLast == efalse)
    //  return 0;

    int acr=0;
    int type;

    for(int i=0;i<diag->size;i++)
    {
        type=diag->arr[i]->type;
        if(type==acrob||type==acrof)
            acr++;

//        diag->val+=diag->arr[i]->val;
    }

    if(acr>0)
        return 1;
    return 0;
}

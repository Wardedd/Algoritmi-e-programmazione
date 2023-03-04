//
// Created by carmine on 16/12/20.
//

#ifndef E1_DIAG_H
#define E1_DIAG_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_ELEM_S 101
#define MAX_DIAG_S 5

typedef enum {transition,acrob,acrof}type;
typedef enum {dirb,dirf}dir;
typedef enum {sfalse,strue}second;
typedef enum {efalse,etrue}end;

typedef struct{
    char nome[MAX_ELEM_S];
    type type;
    dir dirIn;
    dir dirOut;
    second isSecond;
    end isLast;
    double val;
    int dif;
}elem;

typedef struct sdiag{
    elem *arr[MAX_DIAG_S];
    int size;
    double val;
    int DD;
}*pdiag,tdiag;

typedef struct{
    pdiag *arr;
    int pos;
    int size;
}*pdiagArr,tdiagArr;

void readElem(FILE *fp,elem* elem);
void printElem(FILE *fp,elem* elem);
void readElems(FILE *fp,elem *elems,int fileLines);

void printDiag(FILE *fp,pdiag diag);

pdiagArr generateDiags(elem* elems, int nElem,int DD);
void diag_disprep(pdiagArr diags,elem *elems, int nElems,pdiag tmpd,int k,int DD);

pdiag diagInit();
pdiag diagDup(pdiag in);
void diagFree(pdiag diag);

pdiagArr diagsInit(int nElems);
void resizeDiags(pdiagArr diags,int dnum);
void diagArrFree(pdiagArr diags);

void autoSetDiagVal(pdiag diag);

#endif //E1_DIAG_H

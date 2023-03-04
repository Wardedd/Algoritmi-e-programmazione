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

pdiagArr generateDiags(elem* elems, int nElem,int DD){
    pdiagArr diags;
    pdiag tmpd;
    tmpd=diagInit();
    diags=diagsInit(nElem*4);

    for(int k=1;k<=MAX_DIAG_S;k++){
        for(int i=0;i<nElem;i++){
            if(elems[i].isSecond==sfalse && elems[i].dirIn==dirf)
            {
                tmpd->arr[tmpd->size]=&elems[i];
                tmpd->DD+=elems[i].dif;
                tmpd->val+=elems[i].val;
                tmpd->size++;

                diag_disprep(diags,elems,nElem,tmpd,k,DD);

                tmpd->size--;
                tmpd->val-=elems[i].val;
                tmpd->DD-=elems[i].dif;
            }
        }
    }
    diagFree(tmpd);
    return diags;
}

pdiag diagInit(){
    pdiag diag = (pdiag)malloc(sizeof(tdiag));
    if(diag==NULL)
        exit(-3);
    diag->size=0;
    diag->DD=0;
    diag->val=0;
    return diag;
}

pdiag diagDup(pdiag in){
    pdiag d=diagInit();
    for(int i=0;i<in->size;i++)
        d->arr[i]=in->arr[i];
    d->val=in->val;
    d->size=in->size;
    d->DD=in->DD;
    return d;
}

void diagFree(pdiag diag){
    free(diag);
}

pdiagArr diagsInit(int nElems){
    pdiagArr diags = malloc(sizeof(tdiagArr));
    if(diags==NULL)
        exit(-4);
    diags->size=0;
    diags->arr=0;
    diags->pos=0;
    diags->arr=NULL;
    resizeDiags(diags,nElems*4);
    return diags;
}

void resizeDiags(pdiagArr diags,int dnum){
    diags->arr = realloc(diags->arr,dnum*sizeof(pdiag));

    for(int i=diags->size;i<dnum;i++)
        diags->arr[i]=diagInit();
    diags->size=dnum;
}

void diagArrFree(pdiagArr diags){
    for(int i=0;i<diags->size;i++){
        diagFree(diags->arr[i]);
    }
    free(diags);
}

void diag_disprep(pdiagArr diags,elem *elems, int nElems,pdiag tmpd,int k,int DD) { //pos tmpd dentro la sua struct
    if (tmpd->size == k){
        if (checkResult(tmpd)) {
            if(diags->pos==diags->size)
                resizeDiags(diags,diags->size*2); //diags->size cambiato qui dentro

            diags->arr[diags->pos]=diagDup(tmpd); //qui diagDup serve perchè vogliamo che questa diagonale resti in memoria
            diags->pos++;
        }
        return;
    }
    for(int i=0;i<nElems;i++){
        if(checkIteration(tmpd,&elems[i],k,DD)){
            tmpd->arr[tmpd->size]=&(elems[i]);
            tmpd->DD+=elems[i].dif;
            tmpd->val+=elems[i].val;
            tmpd->size++;


            diag_disprep(diags,elems,nElems,tmpd,k,DD);

            tmpd->size--;
            tmpd->val-=elems[i].val;
            tmpd->DD-=elems[i].dif;
        }
    }
}

static int checkResult(pdiag diag){
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

static int checkIteration(pdiag diag,elem* newElem,int k,int DD){

    if ((diag->DD + newElem->dif) > DD)
        return 0;

    if(diag->arr[diag->size-1]->dirOut != newElem->dirIn)
        return 0;

    if (diag->size<k-1 && newElem->isLast == etrue)
        return 0;

    return 1;
}


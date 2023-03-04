#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inv.h"

int readInv(FILE *fp,tabInv_t* inv)
{
    for(int i=0;i<inv->nInv;i++)
    {
        readOgg(fp,&inv->vettInv[i]);
    }
    return 1;
}

void printStat(FILE *fp,stat_t stat){
    fprintf(fp,"%d %d %d %d %d %d\n",stat.hp,stat.mp,stat.atk,stat.def,stat.mag,stat.spr);
}

void printInv_t(link_inv t){

    printf(" %s %s ",t->nome,t->tipo);
    printStat(stdout,t->stat);
}

void printInv(tabInv_t* inv){
    for (int i=0;i<inv->nInv;i++) {
        printf("%d) ",i+1);
        printInv_t(&inv->vettInv[i]);
    }
}

void readStat(FILE *fp,stat_t* stat){
    if(fscanf(fp," %d %d %d %d %d %d",&stat->hp,&stat->mp,&stat->atk,&stat->def,&stat->mag,&stat->spr)!=6){
        printf("Problemi nella lettura delle statistiche");
        exit(1);
    }
}

void readOgg(FILE *fp,link_inv inv){
    char nome[51],tipo[51];

    if(fscanf(fp,"%s %s",nome,tipo)!=2){
        printf("Problemi nella lettura dell file inventario");
        exit(1);
    }

    inv->nome=(char*)malloc((strlen(tipo)+1)*sizeof(char));
    inv->tipo=(char*)malloc((strlen(tipo)+1)*sizeof(char));
    strcpy(inv->nome,nome);
    strcpy(inv->tipo,tipo);

    readStat(fp,&inv->stat);
}

link_inv getInv_t(tabInv_t* inv,char* name){
    for (int i=0;i<inv->nInv;i++) {
        if(strcmp(name,inv->vettInv[i].nome)==0){
            return &inv->vettInv[i];
        }
    }
    return NULL;
}

tabInv_t* NEWtabInv_t()
{
    tabInv_t* inv = (tabInv_t*)malloc(sizeof(tabInv_t));
    if(inv==NULL)
        exit(1);

    inv->nInv=0;
    inv->vettInv=NULL;
    return inv;
}

void FREEtabInv_t(tabInv_t* inv)
{
    free(inv->vettInv);
    free(inv);
}

void LOADtabInv_t(tabInv_t* inv, int numOgg)
{
    free(inv->vettInv);
    inv->vettInv = (inv_t*)malloc(numOgg*sizeof(inv_t));

    if(inv->vettInv==NULL)
        exit(1);

    inv->nInv=numOgg;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pg.h"

//richieste

void readPgList(FILE *fp,tabPg_t* listP, int numPg)
{
    link_pg newPg;

    for(int i=0;i<numPg;i++){
        newPg=NEWPg();
        readPg(fp,newPg);
        PGlAdd(listP,newPg);
    }

}

//richieste
void readPg(FILE* fp,link_pg pg){
    char nome[51],classe[51];

    if(fscanf(fp," PG%d %s %s",&(pg->codice),nome,classe)!=3) {
        printf("Problemi nella lettura del personaggio");
        exit(1);
    }
    pg->nome = (char *) malloc((strlen(nome)+1) * sizeof(char));
    pg->classe = (char *) malloc((strlen(classe)+1) * sizeof(char));
    strcpy(pg->nome, nome);
    strcpy(pg->classe, classe);

    readStat(fp, &pg->stat);
}

void printPgTotStat(link_pg pg){
    stat_t totstat=pg->stat;
    //non uso la var in uso, poichè esistono inUso dati non NULL

    for(int i=0;i<8;i++){
        if(pg->equip->vettEq[i]!=NULL){
            totstat.spr += pg->equip->vettEq[i]->stat.spr;
            totstat.hp += pg->equip->vettEq[i]->stat.hp;
            totstat.atk += pg->equip->vettEq[i]->stat.atk;
            totstat.mag += pg->equip->vettEq[i]->stat.mag;
            totstat.def += pg->equip->vettEq[i]->stat.def;
            totstat.mp += pg->equip->vettEq[i]->stat.mp;
        }
    }
    totstat.spr = totstat.spr<0? 0:pg->stat.spr;
    totstat.mag = totstat.mag<0? 0:pg->stat.mag;
    totstat.atk = totstat.atk<0? 0:pg->stat.atk;
    totstat.mag = totstat.mag<0? 0:pg->stat.mag;
    totstat.def = totstat.def<0? 0:pg->stat.def;
    totstat.mp = totstat.mp<0? 0:pg->stat.mp;

    printf("Codice:PG%d\n nome:%s\n classe:%s\n inUso:%d\n hp:%d\n mp:%d\n atk:%d\n def:%d\n mag:%d\n spr:%d\n",
           pg->codice,pg->nome,pg->classe,pg->equip->inUso,
           totstat.hp,totstat.mp,totstat.atk,
           totstat.def,totstat.mag,totstat.spr);
}

void printPgListSimple(tabPg_t* listP){
    int i=0;
    for (link_pg x = listP->headPg; x != NULL; x = x->next) {
        printf("%d) %s %s",x->codice,x->nome,x->classe);
        printStat(stdout,x->stat);
        i++;
    }
}
void printPgList(tabPg_t* listP){
    int i=0;
    for (link_pg x = listP->headPg; x != NULL; x = x->next) {
        printPg(x);
        i++;
    }
}

void printPg(link_pg pg){
    printf("%d) %s %s\n",pg->codice,pg->nome,pg->classe);
    printf("Stat personaggio: ");
    printStat(stdout,pg->stat);
    if(pg->equip->inUso>0) {
        printf("Inventario:\n");
        printInvPg(pg);
    }
}

void printInvPg(link_pg pg){
    int i=0,k=0;
    while(i<pg->equip->inUso){
        if(pg->equip->vettEq[k]!=NULL){
            printf("\t%d)",i+1);
            printInv_t(pg->equip->vettEq[k]);
            i++;
        }
        k++;
    }
}

tabPg_t* NEWtabPg_t()
{
    tabPg_t* listP = (tabPg_t*)malloc(sizeof(tabPg_t));
    if(listP==NULL)
        exit(1);
    else
    {
        listP->nPg=0;
        listP->headPg=NULL;
        listP->tailPg=NULL;
    }
    return listP;
}

void FREEtabPg_t(tabPg_t* listP)
{
    link_pg x = listP->headPg;
    for(;x!=NULL;x=x->next)
    {
        FREEPg(x);
    }
    free(listP);
}

tabEquip_t *NEWtabEquip(){
    tabEquip_t* t = malloc(sizeof(tabEquip_t));

    t->inUso=0;
    for(int i=0;i<8;i++)
        t->vettEq[i]=NULL;
}

void FREEtabEquip(tabEquip_t* t){
    free(t);
}


link_pg NEWPg(){
    link_pg x = (link_pg)malloc(sizeof(nodoPg_t));
    x->next=NULL;

    x->equip=NEWtabEquip();

    return x;
}

//lista
void FREEPg(link_pg pg){
    free(pg->nome);
    free(pg->classe);
    FREEtabEquip(pg->equip);
    free(pg);
}

int PGeq(link_pg pg,int code){
    if(pg->codice==code)
        return 1;
    return 0;
}

void PGlAdd(tabPg_t* listP,link_pg newnode){

    if(listP->headPg==NULL && listP->tailPg==NULL){
        listP->headPg=newnode;
        listP->tailPg=newnode;
    }
    else
    {
        listP->tailPg->next = newnode;
        listP->tailPg=listP->tailPg->next;
    }
}

int PGlDel(tabPg_t* listP, int code){
    link_pg x=listP->headPg;
    link_pg p=listP->headPg;

    for(;x!=NULL;p=x,x=x->next){
        if(PGeq(x,code))
        {
            listP->nPg--;
            if(p==x){ //siamo nell'head
                if(listP->headPg==listP->tailPg) //esiste 1 solo elemento nella lista
                {
                    listP->headPg=NULL;
                    listP->tailPg=NULL;
                }
                else
                    listP->headPg = listP->headPg->next;
            }
            else
            {
                p->next = x->next;
                if(x==listP->tailPg)
                    listP->tailPg=p;
            }
            FREEPg(x);
            return 1;
        }
    }
    return 0;
}

link_pg getPg(tabPg_t* listP,int code) {

    for (link_pg x = listP->headPg; x != NULL; x = x->next) {
        if (PGeq(x, code)) {
            return x;
        }
    }
    return NULL;
}
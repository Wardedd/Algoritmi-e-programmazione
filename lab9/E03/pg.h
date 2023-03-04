#ifndef PG_H_DEFINED
#define PG_H_DEFINED

#include "inv.h"

#define EQUIP_SLOT 8

typedef struct pg nodoPg_t,*link_pg;

typedef struct
{
    int inUso;
    link_inv vettEq[EQUIP_SLOT];
}tabEquip_t;

struct pg
{
    int codice;
    char *nome;
    char *classe;
    tabEquip_t* equip;
    stat_t stat;
    link_pg next;
};

typedef struct
{
    link_pg headPg;
    link_pg tailPg;
    int nPg;
}tabPg_t;

void readPgList(FILE *fp,tabPg_t* listP, int numPg);

void readPg(FILE* fp,link_pg pg);
void printPg(link_pg pg);
void printPgTotStat(link_pg pg);
void printPgList(tabPg_t* listP);
void printPgListSimple(tabPg_t* listP);
void printInvPg(link_pg pg);

//mem
tabPg_t* NEWtabPg_t();
void FREEtabPg_t(tabPg_t* listP);

tabEquip_t *NEWtabEquip();
void FREEtabEquip(tabEquip_t* t);

link_pg NEWPg();
void FREEPg(link_pg pg);

//list
int PGeq(link_pg pg,int code);
void PGlAdd(tabPg_t* listP,link_pg newnode);
int PGlDel(tabPg_t* listP, int code);
link_pg getPg(tabPg_t* listP,int code);

#endif
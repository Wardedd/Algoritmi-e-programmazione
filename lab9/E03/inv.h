#ifndef INV_H_DEFINED
#define INV_H_DEFINED

typedef struct
{
    int hp;
    int mp;
    int atk;
    int def;
    int mag;
    int spr;
}stat_t;

typedef struct
{
    char *nome;
    char *tipo;
    stat_t stat;
}inv_t,*link_inv;

typedef struct
{
    inv_t *vettInv;
    int nInv;
}tabInv_t;

int readInv(FILE *fp,tabInv_t* inv);

void printInv(tabInv_t* inv);
void printInv_t(link_inv t);
void printStat(FILE *fp,stat_t stat);
void readStat(FILE *fp,stat_t *stat);
void readOgg(FILE *fp,link_inv inv);
link_inv getInv_t(tabInv_t* inv,char* name);

tabInv_t* NEWtabInv_t();
void LOADtabInv_t(tabInv_t* inv, int numOgg);
void FREEtabInv_t(tabInv_t* inv);

#endif
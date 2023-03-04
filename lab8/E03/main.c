#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MENU_SIZE 7
#define MENU_STRLENGTH 80
#define EQUIP_SLOT 8

typedef enum{eloadPg,eloadOgg,eaddPg,edelPg,emodEquip,egetStat,fine,stallo}menustate;

const char menu_str[MENU_SIZE][10] =
{"loadPg","loadOgg","addPg","delPg","modEquip","getStat","fine",};
const char menu_desc[MENU_SIZE][MENU_STRLENGTH] =
{"Caricare l’elenco di personaggi","Caricare l’elenco di oggetti","Aggiungere un nuovo personaggio","Eliminare un personaggio","Aggiungere/rimuovere un oggetto dall’equipaggiamento di un personaggio","Calcola statistiche di un personaggio","Fine"};

typedef struct pg nodoPg_t,*link_pg;

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

//general
void handleState(menustate menu,tabPg_t* listP,tabInv_t* inv);
menustate toMenu(char str[MENU_STRLENGTH]);
menustate readQuery();

//input output
void openFileNoStr(FILE **fp,char *modes);
int getFileLines(FILE *fp);
void readIndex(FILE *fp,int *p_num);
void readOgg(FILE *fp,link_inv inv);
void readStat(FILE *fp,stat_t *stat);
void readPg(FILE* fp,link_pg pg);

void printMenu();
void printFail();
void printSuccess();
void printPgTotStat(link_pg pg);
void printPgList(tabPg_t* listP);
void printInv(tabInv_t* inv);
void printStat(FILE *fp,stat_t stat);

void printOgg(link_inv ogg);
void printEquip(tabEquip_t* equip);

//mem
tabPg_t* NEWtabPg_t();
void FREEtabPg_t(tabPg_t* listP);

tabInv_t* NEWtabInv_t();
void LOADtabInv_t(tabInv_t* inv, int numOgg);
void FREEtabInv_t(tabInv_t* inv);

link_pg NEWPg();
void FREEPg(link_pg pg);

tabEquip_t *NEWtabEquip();
void FREEtabEquip(tabEquip_t* t);

//list
int PGeq(link_pg pg,int code);
void PGlAdd(tabPg_t* listP,link_pg newnode);
int PGlDel(tabPg_t* listP, int code);
link_pg getPg(tabPg_t* listP,int code);

void loadPg(tabPg_t* listP);
int loadOgg(tabInv_t* inv);
void addPg(tabPg_t* listP);
int delPg(tabPg_t* listP);
int getStat(tabPg_t* listP);
int modEquip(tabPg_t* listP,tabInv_t* inv);
int modEquipDel(link_pg pg);
int modEquipAdd(link_pg pg, tabInv_t* inv);

/*
caricare l’elenco di personaggi
caricare l’elenco di oggetti
aggiungere un nuovo personaggio
eliminare un personaggio
aggiungere/rimuovere un oggetto dall’equipaggiamento di un personaggio
readFileInv
newNodoPg freeNodoPg delNodo addNodo modifyNodo(addremequip)
readFilePg
*/

int main()
{
    menustate menu;
    tabPg_t* listP = NEWtabPg_t();
    tabInv_t* inv = NEWtabInv_t();

    do{
        printMenu();
        menu=readQuery();
        handleState(menu,listP,inv);
        printf("\n");
    }while(menu!=fine);

    FREEtabInv_t(inv);
    FREEtabPg_t(listP);
    return 0;
}

//general
void handleState(menustate menu,tabPg_t* listP,tabInv_t* inv)
{
    int x;

    switch(menu)
    {
        case eloadPg:
            loadPg(listP);
            printSuccess();
            break;
        case eloadOgg:
            if(loadOgg(inv)==1)
               printSuccess();
            else if(loadOgg(inv)==-1)
               printf("Inventario già caricato");
            break;
        case eaddPg:
            addPg(listP);
            printSuccess();
            break;
        case edelPg:
            if(delPg(listP)==1)
                printSuccess();
            else
                printFail();
            break;
        case emodEquip:
            x=modEquip(listP,inv);
            switch(x)
            {
                case 1:
                    printSuccess();
                    break;
                case 0:
                    printFail();
                    break;
                case -1:
                    printf("Inventario vuoto");
                    break;
                case -2:
                    printf("Caricare i personaggi");
                    break;
                case -4:
                    printf("Il personaggio ha già raggiunto il limite di 8 oggetti equipaggiati");
                    break;
                case -5:
                    printf("Il personaggio non ha nessun equipaggiamento");
                    break;
                case -6:
                    printf("Numero invalido");
                    break;
            }
            break;
        case egetStat:
            if(getStat(listP)==0)
                printFail();
            break;
       /* case printdebug:
            for (link_pg x = listP->headPg; x != NULL; x = x->next) {
                printf("codice: %d nome: %s classe: %s inUso: %d \n",x->codice, x->nome,x->classe,x->equip->inUso);
                printStat(stdout,x->stat);
                for(int i=0;i<EQUIP_SLOT;i++){
                    if(x->equip->vettEq[i]!=NULL)
                        printf("ogg n.%d nome: %s\n",i+1,x->equip->vettEq[i]->nome);
                }
                printf("------\n");
            }
            if(listP->headPg!=NULL) {
                printf("head");
                printf(" %s %s", listP->headPg->nome, listP->headPg->classe);
                printStat(stdout, listP->headPg->stat);
            }
            if(listP->tailPg!=NULL){
                printf("tail");
                printf(" %s %s",listP->tailPg->nome,listP->tailPg->classe);
                printStat(stdout,listP->tailPg->stat);
            }
            break;*/
        case stallo:
            printf("Comando errato, immetterne uno valido\n");
            break;
    }
}

//  menu
void loadPg(tabPg_t* listP)
{
    int numPg;
    link_pg newPg;
    FILE *fp;

    openFileNoStr(&fp,"r");

    numPg = getFileLines(fp);
    rewind(fp);

    for(int i=0;i<numPg;i++){
        newPg=NEWPg();
        readPg(fp,newPg);
        PGlAdd(listP,newPg);
    }

}

int loadOgg(tabInv_t* inv)
{
    if(inv->vettInv!=NULL) {
        return -1;
    } //non si può leggere + di una volta (?)

    int numOgg;
    FILE *fp;

    openFileNoStr(&fp,"r");
    readIndex(fp,&numOgg);

    LOADtabInv_t(inv,numOgg); //free vecchio inv, malloc v nuovi

    for(int i=0;i<inv->nInv;i++)
    {
        readOgg(fp,&inv->vettInv[i]);
    }
    return 1;
}
void addPg(tabPg_t* listP)
{
    link_pg newp = NEWPg();
    printf("Inserisci i seguenti dati\n"
           "    Codice nome classe <hp> <mp> <atk> <def> <mag> <spr>\n"
           "    Attenzione! Il formato del codice e': PGX (X numero da 0-9, num. di X indefinite)\n");
    readPg(stdin,newp);
    PGlAdd(listP,newp);
}

int delPg(tabPg_t* listP)
{
    int code;
    printf("Inserisci il numero associato ad un personaggio:\n");
    scanf(" %d",&code);
    return PGlDel(listP,code);
}

int getStat(tabPg_t* listP)
{
    int code;
    printf("Inserisci il numero associato ad un personaggio:\n");
    scanf("%d",&code);
    link_pg pg=getPg(listP,code);
    if(pg==NULL)
        return 0;

    printPgTotStat(pg);

    return 1;
}

int modEquip(tabPg_t* listP,tabInv_t* inv)
{
    if(inv->nInv==0){
        return -1;
    }
    else if(listP->headPg==NULL){
        return -2;
    }

    int add;
    int codePg;
    link_pg pg;

    printf("0) Rimuovi Equip\n1) Inserisci equip\n");
    readIndex(stdin,&add);

    printf("Inserisci il numero associato ad un personaggio:\n");
    printPgList(listP);

    if(scanf("%d",&codePg)!=1 || (pg = getPg(listP,codePg))==NULL){
        return 0;
    }

    if (add == 1) {
        return modEquipAdd(pg,inv);
    }
    else if (add == 0) {
        return modEquipDel(pg);
    }
    else
        return 0;
}

int modEquipAdd(link_pg pg, tabInv_t* inv){
    int codeInv;
    if (pg->equip->inUso == EQUIP_SLOT) {
        return -4;
    } else {
        printf("Inserisci il numero associato all'oggetto: \n");
        printInv(inv);
        readIndex(stdin, &codeInv);
        codeInv--;

        if(codeInv<0||codeInv>=inv->nInv)
            return 0;

        for (int i = 0; i < EQUIP_SLOT; i++)
            if (pg->equip->vettEq[i] == NULL)
            {
                pg->equip->vettEq[i] = &(inv->vettInv[codeInv]);
                pg->equip->inUso++;
                return 1;
            }

    }
}

int modEquipDel(link_pg pg){
    int codeInv;

    if (pg->equip->inUso == 0) {
        return -5;
    } else {
        printf("Inserisci il numero associato all'oggetto: \n");
        printEquip(pg->equip);

        readIndex(stdin, &codeInv);
        --codeInv;

        if(codeInv<0||codeInv>=EQUIP_SLOT)
            return -6;

        if (pg->equip->vettEq[codeInv] != NULL) {
            pg->equip->vettEq[codeInv] = NULL;
            pg->equip->inUso--;
            return 1;
        } else {
            return -6;
        }
    }
}

//print

void printMenu()
{
    printf("Menu:\n");
    for(int i=0;i<MENU_SIZE;i++)
    {
        printf("*%s (%s)\n",menu_desc[i],menu_str[i]);
    }
}

void printSuccess()
{
    printf("Operazione eseguita con successo!");
}

void printFail()
{
    printf("Errore nell'esecuzione dell'operazione");
}

void printStat(FILE *fp,stat_t stat){
    fprintf(fp," %d %d %d %d %d %d\n",stat.hp,stat.mp,stat.atk,stat.def,stat.mag,stat.spr);
}

void printPgTotStat(link_pg pg){
    stat_t totstat=pg->stat;
    //non uso la var in uso, poichè esistono inUso dati non NULL

    for(int i=0;i<EQUIP_SLOT;i++){
        if(pg->equip->vettEq[i]!=NULL){
            totstat.spr += pg->equip->vettEq[i]->stat.spr;
            totstat.hp += pg->equip->vettEq[i]->stat.hp;
            totstat.atk += pg->equip->vettEq[i]->stat.atk;
            totstat.mag += pg->equip->vettEq[i]->stat.mag;
            totstat.def += pg->equip->vettEq[i]->stat.def;
            totstat.mp += pg->equip->vettEq[i]->stat.mp;
        }
    }
    totstat.spr = totstat.spr<0? 0:totstat.spr;
    totstat.mag = totstat.mag<0? 0:totstat.mag;
    totstat.atk = totstat.atk<0? 0:totstat.atk;
    totstat.mag = totstat.mag<0? 0:totstat.mag;
    totstat.def = totstat.def<0? 0:totstat.def;
    totstat.mp = totstat.mp<0? 0:totstat.mp;

    printf("Codice:PG%d\n nome:%s\n classe:%s\n inUso:%d\n hp:%d\n mp:%d\n atk:%d\n def:%d\n mag:%d\n spr:%d\n",
           pg->codice,pg->nome,pg->classe,pg->equip->inUso,
           totstat.hp,totstat.mp,totstat.atk,
           totstat.def,totstat.mag,totstat.spr);
}

void printOgg(link_inv ogg){
    printf("%s %s",ogg->nome,ogg->tipo);
    printStat(stdout,ogg->stat);
}

void printPgList(tabPg_t* listP){
    int i=0;
    for (link_pg x = listP->headPg; x != NULL; x = x->next) {
        printf("%d) %s %s",i+1,x->nome,x->classe);
        printStat(stdout,x->stat);

        i++;
    }
}

void printInv(tabInv_t* inv){
    for (int i=0;i<inv->nInv;i++) {
        printf("%d) ",i+1);
        printOgg(&inv->vettInv[i]);
    }
}

void printEquip(tabEquip_t* equip){
    for(int i=0;i<EQUIP_SLOT;i++){
        if(equip->vettEq[i]!=NULL){
            printf("%d) ",i+1);
            printOgg(equip->vettEq[i]);
        }
    }
}

//input

void openFileNoStr(FILE **fp,char *modes)
{
    char fileName[21];
    printf("Inserisci il nome del file: ");
    scanf("%s",fileName);
    *fp=fopen(fileName,modes);

    if(*fp==NULL)
    {
        printf("Problemi nell'apertura del file");  exit(-1);
    }
}

int getFileLines(FILE *fp){
    int i=0;
    int tmpc;

    while((tmpc=fgetc(fp))!=EOF)
    {
        if(tmpc=='\n')
            i++;
    }
    i++; //conto EOF, per aver letto l'ultima riga
    return i;
}


void readIndex(FILE *fp,int *p_num)
{
    if((fscanf(fp,"%d",p_num))==EOF)
    {
        printf("Errore nella lettura del file");
        exit(-1);
    }
}

void readStat(FILE *fp,stat_t* stat){
    if(fscanf(fp," %d %d %d %d %d %d",&stat->hp,&stat->mp,&stat->atk,&stat->def,&stat->mag,&stat->spr)!=6){
        printf("Problemi nella lettura delle statistiche");
        exit(1);
    }
}

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

menustate readQuery()
{ //legge lo stato del menu, ritorna stallo se non è accettabile
    char tmpString[20];
    int res;

    if(scanf("%s",tmpString)!=1)
        res = stallo;
    else
        res = toMenu(tmpString);

    return res;
}

//general
menustate toMenu(char str[MENU_STRLENGTH])
{
    for(int i=0;i<MENU_SIZE;i++)
    {
        if(strcmp(menu_str[i],str)==0)
            return i;
    }
    return stallo;
}

//data struct
//#uso lista con punt. perchè + comoda da passare nelle funzioni in cui va modificata la testa o num

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
    inv->vettInv = (inv_t*)malloc(numOgg*sizeof(*(inv->vettInv)));

    if(inv->vettInv==NULL)
        exit(1);

    inv->nInv=numOgg;
}

tabEquip_t *NEWtabEquip(){
    tabEquip_t* t = malloc(sizeof(tabEquip_t));

    t->inUso=0;
    for(int i=0;i<EQUIP_SLOT;i++)
        t->vettEq[i]=NULL;
}

void FREEtabEquip(tabEquip_t* t){
    free(t);
}

link_pg NEWPg(){
    link_pg x = (link_pg)malloc(sizeof(nodoPg_t));
    x->next=NULL;

    x->equip = NEWtabEquip();

    return x;
};

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

//lista
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pg.h"
#include "inv.h"

#define MENU_SIZE 8
#define KEY_STRLENGTH 30
#define MENU_STRLENGTH 80

typedef enum{eloadPg,eloadOgg,eaddPg,edelPg,emodEquip,egetStat,eprintPgList,fine,stallo}menustate;

const char menu_str[MENU_SIZE][KEY_STRLENGTH] =
{"loadPg","loadOgg","addPg","delPg","modEquip","getStat","printPgList","fine"};
const char menu_desc[MENU_SIZE][MENU_STRLENGTH] =
{"Caricare l’elenco di personaggi",
 "Caricare l’elenco di oggetti",
 "Aggiungere un nuovo personaggio",
 "Eliminare un personaggio",
 "Aggiungere/rimuovere un oggetto dall’equipaggiamento di un personaggio",
 "Calcola statistiche di un personaggio",
 "Stampa una lista dettagliata dei personaggi e i propri oggetti",
 "Fine"};

//general
void handleState(menustate menu,tabPg_t* listP,tabInv_t* inv);
menustate toMenu(char str[MENU_STRLENGTH]);
menustate readQuery();

//input output
void openFileNoStr(FILE **fp,char *modes);
int getFileLines(FILE *fp);
void readIndex(FILE *fp,int *p_num);

void printMenu();
void printFail();
void printSuccess();

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
        case eprintPgList:
                printPgList(listP);
            break;
        case stallo:
            printf("Comando errato, immetterne uno valido\n");
            break;
    }
}

//  menu
void loadPg(tabPg_t* listP)
{
    int numPg;
    FILE *fp;

    openFileNoStr(&fp,"r");

    numPg = getFileLines(fp);
    rewind(fp);

    readPgList(fp,listP,numPg);
}

int loadOgg(tabInv_t* inv)
{
    if(inv->vettInv!=NULL) {
        return -1;
    } //non si può leggere + di una volta

    int numOgg;
    FILE *fp;

    openFileNoStr(&fp,"r");
    readIndex(fp,&numOgg);

    LOADtabInv_t(inv,numOgg);
    //free vecchio inv, malloc v nuovi
    //passo numOgg dentro inv

    readInv(fp,inv);
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

    printPgListSimple(listP);
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
    printPgListSimple(listP);

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
        printf("Inserisci il numero associato all'oggetto: ");
        printInv(inv);
        readIndex(stdin, &codeInv);
        --codeInv;

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

//input

void openFileNoStr(FILE **fp,char *modes)
{
    char fileName[21];
    int res=0;

    printf("Inserisci il nome del file: ");
    res=scanf("%s",fileName);
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
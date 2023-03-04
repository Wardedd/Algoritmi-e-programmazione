#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NOME_FILE "log.txt"

#define FILE_COL 7
#define MENU_SIZE 8
#define MENU_STRLENGTH 20
#define KEYS_STRLENGTH 20
#define PARAMS_STRLENGTH 30
#define FNAME_STLENGTH 20

#define NUM_KEYS 4

typedef enum{
    cerca,stampa,stampafile,orddata,ordtratta,ordpartenza,ordarrivo,fine,stallo
}menu;

typedef enum{
    data,tratta,partenza,arrivo
}keytype;

const char menu_str[MENU_SIZE][MENU_STRLENGTH]={"cerca","stampa","stampafile","orddata","ordtratta","ordpartenza","ordarrivo","fine"};

int toMenu(char str[MENU_STRLENGTH]){
    for(int i=0;i<MENU_SIZE;i++){
        if(strcmp(menu_str[i],str)==0)
            return i;
    }
    return stallo;
}

typedef struct{
    char codTratta[PARAMS_STRLENGTH];
    char partenza[PARAMS_STRLENGTH];
    char destinazione[PARAMS_STRLENGTH];
    char data[11];
    char orarioPar[9],orarioArr[9];
    int ritardo;
}Corsa;

typedef struct{
    char str[11];
    char type[20];
    char desc[20];
}Param;

typedef struct{
    Param par[2];
    int nParms;
}Req;

int fileFillMatrix(FILE *fp,int size,Corsa matrix[]);
int readQuery(Req *p);
int getParams(Req *p,int pMin);

int trattaToInt(Corsa corsa);
int dateTimeToInt(char date[11],int isDate);
void initIndex(int *v,int size);

void printCorsaStart(Corsa *logs,int *index, int size, Req req,int isKeyPartenza);
int printCorsa(FILE *fp,Corsa corsa);
void printFail();
void printMenu();
void insertionSort(int s,int *index,Corsa *logs,int *itemLt(Corsa,Corsa,keytype),keytype type);

int fermataLt(Corsa corsa1,Corsa corsa2,keytype type);
int trattaLt(Corsa corsa1,Corsa corsa2);
int timeLt(Corsa corsa1,Corsa corsa2);

int selezionaDati(int size,Corsa *logs,int *index,Req req,int menu,int *isKeyPartenza);

void printPartenzaLinear(Corsa logs[],int *index,int size,Req req);
void printPartenzaBinary(Corsa logs[],int *index,int size,Req req);
int searchBinary(Corsa logs[],int *index,int size,Req req);
int searchBinaryFull(Corsa logs[],int *index,int size,char *keystr,int keysize,int r, int l); //wrapper

int main(){
    int  lung,menu;
    FILE *fp;

    fp=fopen(NOME_FILE,"r");

    if(fp==NULL){
        printf("Errore nell'accesso al file");
        return -1;
    }

    if(fscanf(fp,"%d",&lung)!=1){
        printf("Errore nella letture dell'indice nel file");
        return -2;
    }//max corse 1000
    if(lung>1000)
        lung=1000;

    Corsa logMat[lung];
    int logIndex[lung];
    Req req;

    if(!fileFillMatrix(fp,lung,logMat)) {
        printf("Verificare che sia la struttura del file sia corretta");
        return -3;
    }

    initIndex(logIndex,lung);
    int isKeyPartenza=0;

    do{
        printMenu();
        menu=readQuery(&req);
        selezionaDati(lung,logMat,logIndex,req,menu,&isKeyPartenza);
        printf("\n");
    }while(menu!=fine);

    printf("\nArrivederci!");
    return 0;

}

int selezionaDati(int size,Corsa *logs,int *index,Req req,int menu,int *isKeyPartenza){
    switch(menu){
        int pMin; //min. params for request

        case cerca:
            pMin=1;
            strcpy(req.par[0].desc,"fermata di partenza");
            getParams(&req,pMin);
            printCorsaStart(logs,index,size,req,*isKeyPartenza);
            break;
        case stampa:
        case stampafile:
        {
            FILE *fp;
            int err=0;
            if(menu==stampa)
                fp=stdout;
            else {
                char fileName[FNAME_STLENGTH];
                printf("Inserire il nome file: ");
                scanf("%s",fileName);

                if ((fp = fopen(fileName, "w")) == NULL) {
                    printf("Errore nell'apertura del file");
                    return fine;
                }
            }
            for(int i=0;i<size &&err==0;i++)
                if(printCorsa(fp,logs[index[i]])!=0)
                    err=1;
            if(err)
                printf("Ci sono stati problemi nella scrittura su file");
            else
                printf("Fatto!");

            break;
        }

        case orddata:
            insertionSort(size,index,logs,timeLt,data);
            *isKeyPartenza=0;
            break;
        case ordtratta:
            insertionSort(size,index,logs,trattaLt,tratta);
            *isKeyPartenza=0;
            break;
        case ordpartenza:
            insertionSort(size,index,logs,fermataLt,partenza);
            *isKeyPartenza=1;
            break;
        case ordarrivo:
            insertionSort(size,index,logs,fermataLt,arrivo);
            *isKeyPartenza=0;
            break;
    }
    return 1;
}

//sort
void insertionSort(int s,int *index,Corsa *logs,int *itemLt(Corsa,Corsa,keytype),keytype type){
    int i,j,tmpi;

    for(i=0;i<s-1;i++){
        j=i+1;
        tmpi=index[j];

        while(j>0&&itemLt(logs[tmpi],logs[index[j-1]],type)){
            index[j]=index[j-1];
            j--;
        }
        index[j]=tmpi;
    }
}

//search
void printPartenzaLinear(Corsa logs[],int *index,int size,Req req){
    int count = 0;
    int keysize=strlen(req.par[0].str);
    for (int i = 0; i < size; i++) {
        if (strncmp(logs[index[i]].partenza, req.par[0].str, keysize) == 0) {
            printCorsa(stdout, logs[index[i]]);
            count++;
        }
    }
    if (count == 0)
        printFail();
}

void printPartenzaBinary(Corsa logs[],int *index,int size,Req req){
    int keyI = searchBinary(logs,index,size,req);
    if(keyI==-1)
        printFail();
    else
    {//essendo un vettore ordinato, trovato 1 risultato, potrebbero essercene altri intorno ad esso
        int i;
        int keysize=strlen(req.par[0].str);

        printCorsa(stdout,logs[index[keyI]]);
        for(i=keyI-1;i>=0;i--)
            if(strncmp(req.par[0].str,logs[index[i]].partenza,keysize)==0)
                printCorsa(stdout,logs[index[i]]);

        for(i=keyI+1;i<size;i++)
            if(strncmp(req.par[0].str,logs[index[i]].partenza,keysize)==0)
                printCorsa(stdout,logs[index[i]]);
    }
}

int searchBinary(Corsa logs[],int *index,int size,Req req)
{
    int l=0,r=size-1;
    return searchBinaryFull(logs,index,size,req.par[0].str,strlen(req.par[0].str),l,r);
}

int searchBinaryFull(Corsa logs[],int *index,int size,char *keystr,int keysize,int l,int r ){

    //ordinati in modo ascendente
    int innerSize=r-l;
    int difference=strncmp(logs[index[l+innerSize/2]].partenza,keystr,keysize);

    if(innerSize>0){
        if(difference>0)//siamo troppo a dx, andare a sx
        {
            return searchBinaryFull(logs,index,size,keystr,keysize,l,l+innerSize/2-1);
        }
        else if(difference<0){ //siamo troppo a sx,andare a dx
            return searchBinaryFull(logs,index,size,keystr,keysize,l+innerSize/2+1,r);
        }
    }

    if(difference==0){
        return l+innerSize/2;
    }

    return -1;
}

//lesser than | 1-> corsa1<corsa2
int fermataLt(Corsa corsa1,Corsa corsa2,keytype type){
    if(type==partenza)
    {
        if(strcmp(corsa1.partenza,corsa2.partenza)<0)
            return 1;
        return 0;
    }
    else{
        if(strcmp(corsa1.destinazione,corsa2.destinazione)<0)
            return 1;
        return 0;
    }
}

int trattaLt(Corsa corsa1,Corsa corsa2){
    if(trattaToInt(corsa1)<trattaToInt(corsa2))
        return 1;
    return 0;
}

int timeLt(Corsa corsa1,Corsa corsa2){
    int time1=dateTimeToInt(corsa1.data,1);
    int time2=dateTimeToInt(corsa2.data,1);
    if(time1<time2)
        return 1;
    else if(time1==time2)
    {
        time1=dateTimeToInt(corsa1.orarioPar,0);
        time2=dateTimeToInt(corsa2.orarioPar,0);
        if(time1<time2)
            return 1;
    }
    return 0;
}

//output

void printCorsaStart(Corsa logs[], int *index,int size, Req req,int isKeyPartenza) {
    if(isKeyPartenza!=1) //allora usiamo ricerca lineare
    {
        printPartenzaLinear(logs,index,size,req);
    }
    else //altrimenti usiamo ricerca dicotomica
    {
        printPartenzaBinary(logs,index,size,req);
    }
}

int printCorsa(FILE *fp,Corsa corsa){
    if(fprintf(fp,"%s %s %s %s %s %s %d\n",corsa.codTratta,corsa.partenza,corsa.destinazione,corsa.data,
               corsa.orarioPar,corsa.orarioArr,corsa.ritardo)<0)
        return -1;
    return 0;
}
void printFail(){
    printf("Non abbiamo nessuna corsa che soddisfa il filtro");
}

void printMenu()
{
    printf("Menu:\n*Cerca tramite stazione di partenza (cerca)\n*Stampa le corse su schermo (stampa)\n*Stampa le corse su file (stampafile)\n*Ordina per data (orddata)\n*Ordina per arrivo (ordarrivo)\n*Ordina per tratta (ordtratta)\n*Ordina per partenza (ordpartenza)\n*Fine (fine)\n");
}

//input
int fileFillMatrix(FILE *fp,int size,Corsa matrix[]){
    int res=FILE_COL;
    for(int i=0;i<size&&res==FILE_COL;i++)
        res=fscanf(fp,"%s %s %s %s %s %s %d",
                   matrix[i].codTratta,matrix[i].partenza,matrix[i].destinazione,
                   matrix[i].data,matrix[i].orarioPar,matrix[i].orarioArr,
                   &matrix[i].ritardo);

    if(res!=FILE_COL) //se ci sono stati problemi nella lettura delle entry, ritorna 0
        return 0;

    return 1;
}

void initIndex(int *v,int size)
{
    for(int i=0;i<size;i++){
        v[i]=i;
    }
}

int getParams(Req *p,int pMin){
    int res=0;
    p->nParms=0;

    for(int i=p->nParms;i<pMin;i++){
        printf("Inserisci %s\n",p->par[i].desc);
        res=scanf("%s",p->par[i].str);
        p->nParms++;
    }

    if(res!=1) {
        res=0;
    }
    return res;
}

int readQuery(Req *p){ //ritorna fine se non si sceglie il comando giusto
    char tmpString[20];
    int res;

    p->nParms=scanf("%s",tmpString);
    if(p->nParms==0)
        res = fine;
    else
        res=toMenu(tmpString);

    return res;
}

//supporto
int dateTimeToInt(char date[11],int isDate){    //dateortime - date=1 time!=1
    int a,b,c;
    char tmp[11];
    strcpy(tmp,date);
    a=atoi(strtok(tmp,":/"));
    b=atoi(strtok(NULL,":/"));
    c=atoi(strtok(NULL,":/"));

    if(isDate==1)
        return a*12*30+b*30+c;
    else
        return a*60*60+b*60+c;
}

int trattaToInt(Corsa corsa){
    int firstnum;
    firstnum=strpbrk(corsa.codTratta,"1234567890")-corsa.codTratta;

    return atoi(corsa.codTratta+firstnum);
}

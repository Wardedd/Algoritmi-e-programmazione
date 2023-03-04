#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME_FILE "log.txt"
#define ENTRY_COL 7

typedef enum{
data,partenza,capolinea, ritardo, ritardo_tot,fine
}menu;

const char menu_str[6][12]={"data","partenza","capolinea","ritardo","ritardo_tot","fine"};

int toMenu(char str[20]){
    for(int i=0;i<6;i++){
        if(strcmp(menu_str[i],str)==0)
            return i;
    }
    return fine;
}

//2018/09/09
typedef struct{
    char codTratta[30];
    char partenza[30];
    char destinazione[30];
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
int selezionaDati(int logSize,Corsa logs[],Req req,int menu);

int dateTimeToInt(char date[11],int isDate);

void printCorsa(Corsa corsa);
void printCorsaDates(Corsa logs[],int size,Req req);
void printCorsaDelayTot(Corsa logs[], int size, Req req);
void printCorsaEnd(Corsa logs[], int size, Req req);
void printCorsaDelayDays(Corsa logs[], int size, Req req);
void printCorsaStart(Corsa logs[], int size, Req req);

void printFail();

int main(){
    int  lung,menu;
    FILE *fp;

    fp=fopen(NOME_FILE,"r");

    if(fp==NULL){
        printf("Couldn't access file");
        return -1;
    }

    if(fscanf(fp,"%d",&lung)!=1){
        printf("Couldn't access main indexes correctly");
        return -2;
    }
    if(lung>1000)
        lung=1000;
    Corsa logMat[lung];
    Req req;

    if(!fileFillMatrix(fp,lung,logMat)) {
        printf("Verificare che sia la struttura del file sia corretta");
        return -3;
    }

    printf("Menu:\n-data\n-partenza\n-capolinea\n-ritardo\n-ritardo_tot\n-fine\n");

    do{
        menu=readQuery(&req);
        selezionaDati(lung,logMat,req,menu);
        printf("---------\n");
    }while(menu!=fine);

    printf("\nFine!");
    return 0;
}

int readQuery(Req *p){ //ritorna fine se non si sceglie il comando giusto
    char tmpString[20];
    int res;

    p->nParms=scanf("%s",tmpString,p->par[0].str,p->par[1].str);
    if(p->nParms==0)
        res = fine;
    else
        res=toMenu(tmpString);

    return res;
}

int selezionaDati(int size,Corsa logs[],Req req,int menu){
    switch(menu){
        int pMin; //min. params for request

        case data:
            pMin=2;
            strcpy(req.par[0].desc,"prima data");
            strcpy(req.par[1].desc,"seconda data");
            getParams(&req,pMin);
            printCorsaDates(logs,size,req);
            break;

        case partenza:
            pMin=1;
            strcpy(req.par[0].desc,"fermata di partenza");
            getParams(&req,pMin);
            printCorsaStart(logs,size,req);
            break;

        case capolinea:
            pMin=1;
            strcpy(req.par[0].desc,"fermata capolinea");
            getParams(&req,pMin);
            printCorsaEnd(logs,size,req);
            break;

        case ritardo:
            pMin=2;
            strcpy(req.par[0].desc,"prima data");
            strcpy(req.par[1].desc,"seconda data");
            getParams(&req,pMin);
            printCorsaDelayDays(logs,size,req);
            break;

        case ritardo_tot:
            pMin=1;
            strcpy(req.par[0].desc,"codice tratta");
            getParams(&req,pMin);
            printCorsaDelayTot(logs,size,req);
            break;
    }
    return 1;
}

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

//input
int fileFillMatrix(FILE *fp,int size,Corsa matrix[]){
    int res=ENTRY_COL;
    for(int i=0;i<size&&res==ENTRY_COL;i++)
        res=fscanf(fp,"%s %s %s %s %s %s %d",
           matrix[i].codTratta,matrix[i].partenza,matrix[i].destinazione,
           matrix[i].data,matrix[i].orarioPar,matrix[i].orarioArr,
           &matrix[i].ritardo);

    if(res!=ENTRY_COL) //se ci sono stati problemi nella lettura delle entry, ritorna 0
        return 0;

    return 1;
}

int getParams(Req *p,int pMin){
    int res=0;
    p->nParms=0;

    for(int i=p->nParms;i<pMin;i++){
        printf("Inserisci %s ",p->par[i].desc);
        res=scanf("%s",p->par[i].str);
        p->nParms++;
    }

    if(res!=1) {
        res=0;
    }
    return res;
}

//output
void printCorsaDates(Corsa logs[],int size,Req req){
    int dateMin=dateTimeToInt(req.par[0].str,1);
    int dateMax=dateTimeToInt(req.par[1].str,1);
    int count=0;
    for(int i=0;i<size;i++)
    {
        int tmpDate=dateTimeToInt(logs[i].data,1);
        if(dateMin<=tmpDate&&tmpDate<=dateMax)
        {
            printCorsa(logs[i]);
            count++;
        }
    }
    if(count==0)
        printFail();

}//2018/09/09

void printCorsaStart(Corsa logs[], int size, Req req) {
    int count=0;
    for(int i=0;i<size;i++)
    {
        if(strcmp(logs[i].partenza,req.par[0].str)==0)
        {
            printCorsa(logs[i]);
            count++;
        }
    }
    if(count==0)
        printFail();
}

void printCorsaEnd(Corsa logs[], int size, Req req) {
    int count=0;
    for(int i=0;i<size;i++)
    {
        if(strcmp(logs[i].destinazione,req.par[0].str)==0)
        {
            printCorsa(logs[i]);
            count++;
        }
    }
    if(count==0)
        printFail();
}

void printCorsaDelayDays(Corsa logs[], int size, Req req) {
    int dateMin=dateTimeToInt(req.par[0].str,1);
    int dateMax=dateTimeToInt(req.par[1].str,1);
    int count=0;

    for(int i=0;i<size;i++)
    {
        int tmpDate=dateTimeToInt(logs[i].data,1);
        if(dateMin<=tmpDate&&tmpDate<=dateMax && logs[i].ritardo>0)
        {
            printCorsa(logs[i]);
            count++;
        }
    }
    if(count==0)
        printFail();
}

void printCorsaDelayTot(Corsa logs[], int size, Req req) {
    int totDelay=0,count=0;

    for(int i=0;i<size;i++)
    {
        if(strcmp(logs[i].codTratta,req.par[0].str)==0)
        {
            totDelay+=logs[i].ritardo;
            count++;
        }
    }
    if(count==0)
        printFail();
    else{
        printf("Cod.Tratta %s Ritardo %d min.",req.par[0].str,totDelay);
    }
}

void printCorsa(Corsa corsa){
    printf("%s %s %s %s %s %s %d\n",corsa.codTratta,corsa.partenza,corsa.destinazione,corsa.data,
           corsa.orarioPar,corsa.orarioArr,corsa.ritardo);
}

void printFail(){
    printf("Non abbiamo nessuna corsa che soddisfa il filtro");
}
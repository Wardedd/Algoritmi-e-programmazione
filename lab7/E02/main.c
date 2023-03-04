#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MENU_SIZE 7 //non si conta stallo
#define MENU_STRLENGTH 20
#define MAX_MENUDESC_SIZE 40
#define MAX_REQ_SIZE 20
#define MAX_FILENAME_SIZE 20

typedef enum {leggitast,leggifile,cercacode,cancellacode,cancelladata,stampafile,fine,stallo}menu;
const char menu_str[MENU_SIZE][MENU_STRLENGTH]={"leggiTast","leggiFile","cercaCode","cancellaCode","cancellaData","stampaFile","fine"};

char menu_desc[MENU_SIZE][MAX_MENUDESC_SIZE]={
        "Leggi persona da Tastiera",
        "Leggi persone da File",
        "Cerca persona tramite codice",
        "Estrai persona tramite codice",
        "Estrai persone tramite data di nascita",
        "Stampa lista su File",
        "Fine"
};

typedef struct p Persona,*link;

struct p{
    char codice[6];
    char nome[51];
    char cognome[51];
    char nascita[11];
    int cap;
    char via[51];
    char citta[51];
    link next;
};

//file
void openFile(FILE **fp,char *fileName,char *mode);
//print
void printFail();
void printMenu();
int printPersona(FILE *fp,Persona persona);
void printMenu();

//input & malloc
int getFileLines(FILE *fp);
int fillList(FILE *fp,Persona *persona);
link newPersona();
link getPersona(FILE *fp);
void getFileListPersona(FILE *fp,link *head,link *last); //modifica last
link findPersona(link head,char *key);
link estrPersonaCode(link *head,char *key);
link estrPersonaData(link *head,char* dateStart,char* dateEnd);
void insertPersona(link *head, link newp);

int readQuery();
int toMenu(char str[MENU_STRLENGTH]);

void printList(FILE *fp,link head);
void selezionaDati(int menu,link *head,link *last,int debug);
int dateToInt(char date[11]);

int main(){

    int size,menu;
    int debug=0;
    link head=NULL,last=NULL;

    do{
        printMenu();
        menu=readQuery();
        selezionaDati(menu,&head,&last,debug);
        printf("_______\n");
    }while(menu!=fine);

    printf("Arrivederci!");

    return 0;
}

void selezionaDati(int menu,link *head,link *last,int debug){

    FILE *fp;
    int max = MAX_FILENAME_SIZE>6?MAX_FILENAME_SIZE:6;
    char tmps[max];
    char tmps2[max];

    link p;
    int i=0;

    switch(menu){
        case leggitast:
            fp = stdin;
            printf("Inserire i AXXXX nome cognome data di nascita via citta cap: ");
            insertPersona(head,getPersona(fp));
            break;
        case leggifile:
            printf("Inserire il nome del file: ");
            scanf("%s",tmps);
            openFile(&fp,tmps,"r");
            getFileListPersona(fp,head,last);
            break;
        case cercacode:
            printf("Inserisci il codice della persona cercata: ");
            scanf("%s",tmps);
            p = findPersona(*head,tmps);
            if(p!=NULL)
                printPersona(stdout,*p);
            else
                printFail();
            break;
        case cancellacode:
            printf("Inserisci il codice della persona ricercata: ");
            scanf("%s",tmps);
            p=estrPersonaCode(head,tmps);
            if(p!=NULL)
                printPersona(stdout,*p);
            else
                printFail();
            break;
        case cancelladata:
            printf("Inserisci le 2 date di nascita: ");
            scanf("%s %s",tmps,tmps2);
            while((p=estrPersonaData(head,tmps,tmps2))!=NULL) {
                printPersona(stdout, *p);
                i++;
            }
            if(i==0)
                printFail();
            break;
        case stampafile:
            if(debug==1)
                fp=stdout;
            else{
                printf("Inserire il nome del file: ");
                scanf("%s",tmps);
                openFile(&fp,tmps,"w");
            }
            printList(fp,*head);
            break;
        case fine:
            break;
    }
}

link estrPersonaData(link *head,char* dateStart,char* dateEnd) {
    int date1 = dateToInt(dateStart);
    int date2 = dateToInt(dateEnd);
    int tmpData;

    for (link p = *head, oldp = *head; p != NULL; oldp = p, p = p->next){
        tmpData = dateToInt(p->nascita);
        if (date1 <= tmpData && date2 >= tmpData) {
            oldp->next = p->next;
            if (p == *head)
                *head = p->next;
            return p;
        }
    }
    return NULL;
}

link estrPersonaCode(link *head,char *key){
    for(link p=*head,oldp=*head;p!=NULL;oldp=p,p= p->next)
        if(strcmp(key,p->codice)==0)
        {
            oldp->next=p->next;
            if(p==*head)
                *head=p->next;
            return p;
        }
    return NULL;
}

link findPersona(link head,char *key){
    link p;
    for(p=head;p!=NULL;p=p->next)
        if(strcmp(key,p->codice)==0)
            return p;
    return NULL;
}
//input
int getFileLines(FILE *fp){
    int i=0;
    Persona dummyp;
    char test[10];
    while(fscanf(fp,"%s %s %s %s %s %s %d",dummyp.codice,dummyp.nome,dummyp.cognome,dummyp.citta,dummyp.via,dummyp.nascita,&dummyp.cap)==7)
        i++;
    return i;
}

link getPersona(FILE *fp){
    link newp = newPersona();
    if(fscanf(fp,"%s %s %s %s %s %s %d",newp->codice,newp->nome,newp->cognome,newp->nascita,newp->via,newp->citta,&newp->cap)!=7)
    {
        printf("Errore nella creazione della nuova persona, lista invariata");
        //free
        return NULL; //NULL
    }
    else {
        return newp;
    }
}

void insertPersona(link *head, link newp){

    link x,p;
    int newBirthday=dateToInt(newp->nascita);

    if(newp!=NULL && newBirthday<0) { //se la nuova entry ha data errata
        printf("La persona non è stat inserita correttamente\n");
        return;
    }

    if(*head==NULL || dateToInt((*head)->nascita)>newBirthday) //se head==null o bisogna modificare l'head
    {
        newp->next=*head;
        *head=newp;
        return;
    }

    //caso default
    for(x=(*head)->next,p=*head;
        x!=NULL && newBirthday>dateToInt(x->nascita);
        p=x,x=x->next);

    newp->next=x;
    p->next=newp;

    return;
}

void getFileListPersona(FILE *fp,link *head,link *last){
    int listSize = getFileLines(fp);
    int ok=1;
    link tmpP;
    rewind(fp);//resetto la posizione per rileggere il file

    for(int i=0;i<listSize&&ok;i++){
        tmpP=getPersona(fp);
        if(tmpP==NULL)
            ok=0;
        else
        insertPersona(head,tmpP);
    }
}

int readQuery(){
    int res;
    char tmpString[MAX_REQ_SIZE];
    if(scanf("%20s",tmpString)==0)
    {
        printf("Errore nell'acquisizione del comando\n");
        res = fine;
    }
    else
        res=toMenu(tmpString);

    return res;
}


int toMenu(char str[MENU_STRLENGTH]){
    for(int i=0;i<MENU_SIZE;i++){
        if(strcmp(menu_str[i],str)==0)
            return i;
    }
    printf("Comando inesistente\n");
    return stallo;
}

//file
void openFile(FILE **fp,char *fileName,char *mode){
    *fp=fopen(fileName,mode);
    if(*fp==NULL){
        printf("Problemi nell'apertura del file");
        exit(-1);
    }
}

//support
int dateToInt(char date[11]){    //dateortime - date=1 time!=1
    int giorni,mesi,anni;
    int err=sscanf(date,"%d/%d/%d",&giorni,&mesi,&anni);
    if(err!=3) //error
        return -1;
    return giorni+mesi*30+anni*12*30;

}

link newPersona(){
    link p = malloc(sizeof(Persona));
    p->next=NULL;
    return p;
}

//print
void printFail(){
    printf("La sua ricerca non ha avuto riscontri\n");
}

int printPersona(FILE *fp,Persona persona){
    if(fprintf(fp,"%s %s %s %s %s %s %d\n",persona.codice,persona.nome,persona.cognome,persona.nascita,
               persona.via,persona.citta,persona.cap)<0)
        return -1;
    return 0;
}

void printList(FILE *fp,link head){

    for(link p=head;p!=NULL;p=p->next)
        if(printPersona(fp,*p)){
            printf("Si e' verificato un errore nella stampa della lista");
            return;
        }
}

void printMenu(){
    printf("Menu:\n");
    for(int i=0;i<MENU_SIZE;i++){
        printf("*%s (%s)\n",menu_desc[i],menu_str[i]);
    }
}

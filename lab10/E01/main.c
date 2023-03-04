#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int s,f;
}att;

int getTime(att a){
    return a.f-a.s;
}

int attCompatible(att* atts,int a, int b){
    return !(atts[a].s<atts[b].f&&atts[b].s<atts[a].f);
}

void fillArr(FILE *fp, att* atts, int n);
void ordArrBySi(att* atts,int n);
int dynp_getMaxSchedule(att* atts,int n);
int getSol(int *sez,int *att,int n); //prendo time totale dall'ultima sezione

void openFile(FILE **fp,char *fileName);
void readIndex(FILE *fp,int *p_size);

//powerset combinazioni semplici, ordine non conta

int main(){
    int n;
    FILE *fp;
    att *atts;

    openFile(&fp,"att2.txt");
    readIndex(fp,&n);

    atts = (att*)malloc(n*sizeof(att));
    fillArr(fp,atts,n);
    ordArrBySi(atts,n);

    printf("%d",dynp_getMaxSchedule(atts,n));

    free(atts);
    return 0;
}

void swapAtt(att* atts,int a,int b){
    att tmp=atts[b];
    atts[b]=atts[a];
    atts[a]=tmp;
}

void ordArrBySi(att* atts, int s)
{//insertion sort
    int tmps;
    int i, j;

    for (i=0; i<s-1; i++)
    {
        tmps=atts[i+1].s;
        for (j=i; j>=0&&tmps<atts[j].s; j--)
        {
            swapAtt(atts, j+1, j);
        }
        swapAtt(atts, i, j);
    }
}

void fillArr(FILE *fp, att* atts, int n)
{
    for (int i=0; i<n; i++)
        if (fscanf(fp, "%d %d", &atts[i].s, &atts[i].f)!=2)
        {
            printf("Problema nella lettura delle att nel file");
            exit(-1);
        }
}

/*
 * Sezione:
 * insieme di attività che nell'asse temporale continuano ad intersecarsi con l'attività successiva
 * Esempio sezione:
 * (2 4),(3,5),(4,6)
 * -----------------------
 * time[i]=0; //vet. att
 * risolvi localmente ogni attività, contando nell'asse temporale le att. comp. fino ad i
 * da att. 0 a att. n: for grande
 *    primo for: controlla esistenza att. precedenti compatibili all'interno della sezione,se j esiste time[i]+=time[j]; //j ottimo precedente trovato
 *    secondo for: se non è stato trovato niente, cerca nella sezione precedente
 *    time[i]+=atts[i];
 * fine
 * getsol(sez,att,n)
 */
int dynp_getMaxSchedule(att* atts,int n)
{
    int time[n]; //usato per tenere il tempo accumulato locale
    int sez[n]; //id. sezione per attività i

    int flag;
    int sezChangeI;

    for (int i=0; i<n; i++)
        time[i]=0;

    sezChangeI=0;
    sez[0]=sezChangeI;

    //riempio vett sezioni
    for (int i=1; i<n; i++)
    {
        if (attCompatible(atts, i, i-1))
            sezChangeI=i;
        sez[i]=sezChangeI;
    }

    for (int i=0; i<n; i++)
    {
        flag = 0;

        for (int j=sez[i]; j<i; j++) //cerco att precedente in sezione attuale
            if (attCompatible(atts,i,j))
            {
                if (time[i]<time[j])
                    time[i]=time[j];

                flag = 1;
            }

        sezChangeI = sez[i];

        if (flag==0 && sezChangeI>0) //altrimenti lo cerco nella sez precedente
            for (int j=sez[sezChangeI-1];j<sezChangeI;j++)
                if (time[i]<time[j])
                    time[i]=time[j];

        time[i] += getTime(atts[i]);
    }

    //printSol
    return getSol(sez, time, n);
}

int getSol(int *sez,int *time,int n)
{ //prendo time totale dall'ultima sezione
    int max=0;
    for (int i=sez[n-1]; i<n; i++)
    {
        if (max<time[i])
            max=time[i];
    }
    return max;
}

//old
void openFile(FILE **fp,char *fileName)
{
    *fp=fopen(fileName, "r");
    if (*fp==NULL)
    {
        printf("Problemi nell'apertura del file");
        exit(-1);
    }
}

void readIndex(FILE *fp,int *p_size)
{
    if ((fscanf(fp, "%d", p_size))==EOF)
    {
        printf("Errore nella lettura del file");
        exit(-1);
    }
}
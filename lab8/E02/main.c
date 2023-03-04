#include <bits/types/FILE.h>
#include <stdio.h>
#include <stdlib.h>

void openFile(FILE **fp,char *fileName);
void readIndex(FILE *fp,int *size);
void fillMatrix(FILE *fp,int **gemsNum,int **gemsVal,int *maxreps,int size);

void malloc2Dp(int*** mat, int nr,int nc);
void free2Dp(int** mat, int nr);

void printBestCollana(int *nGems,int *gemsVal,int maxrep);
int r_collValue(int *nGems,int *gemsVal,char *collTmp,char *collMax,int tot,int *maxval,int *maxpos,int maxrep,int pos,int currval,int *rep,int *maxvalpos);
int checkIteration(int *nGems,char *collTmp,int pos,int tot,int i,int *rep,int maxrep);
int checkValidity(char *collMax,int pos);

int getMaxValPos(int *nGems,int *gemsVal);

//file z s r t
const char gems[4]="zrts";

int main() {
    FILE *fp;
    int size;
    int **gemsNum;
    int **gemsVal;
    int *maxreps;

    openFile(&fp,"test_set_result.txt");
    readIndex(fp, &size);

    malloc2Dp(&gemsNum,size,4);
    malloc2Dp(&gemsVal,size,4);
    maxreps=(int*) malloc(size*sizeof(int));

    fillMatrix(fp, gemsNum,gemsVal, maxreps,size);

    for (int i = 0; i < size; i++){
        printf("Collana della sequenza numero %d\n",i+1);
        printBestCollana(gemsNum[i],gemsVal[i],maxreps[i]);
    }
    free(maxreps);
    free2Dp(gemsNum,size);
    free2Dp(gemsVal,size);
    return 0;
}

int getMaxValPos(int *nGems,int *gemsVal){
    int val=0;
    for(int i=0;i<4;i++){
        val+=nGems[i]+gemsVal[i];
    }
}

void printBestCollana(int *nGems,int *gemsVal,int maxrep){

    int tot = nGems[0]+nGems[1]+nGems[2]+nGems[3];
    int maxvalpos=getMaxValPos(nGems,gemsVal); //maxvalpossible

    char collMax[tot];
    char collTmp[tot];
    int currval;
    int maxval=0;
    int maxpos=0;
    int rep;

    if(tot!=0) {
        for(int i=0;i<4;i++){
            if(nGems[i]>0) {

                collTmp[0] = gems[i];
                currval=gemsVal[i];
                nGems[i]--;
                rep=1;

                if(r_collValue(nGems,gemsVal,collTmp,collMax,tot,&maxval,&maxpos,maxrep,1,currval,&rep,&maxvalpos))
                    break;
                nGems[i]++;
            }
        }
    }
    else {
        printf("No gems found");
    }
    printf("Collana lunghezza: %d costo: %d maxrep: %d\n",maxpos,maxval,maxrep);

    for(int i=0;i<maxpos;i++){
        printf("%c",collMax[i]);
    }
    printf("\n");
}

//nGems funziona da mark
int r_collValue(int *nGems,int *gemsVal,char *collTmp,char *collMax,int tot,int *maxval,int *maxpos,int maxrep,int pos,int currval,int *rep,int *maxvalpos){
    if(currval>*maxval) {
        if (checkValidity(collTmp,pos)){

            for (int i = 0; i < pos; i++) //cpyVector
                collMax[i] = collTmp[i];

            *maxval = currval;
            *maxpos = pos;

            if(*maxval==*maxvalpos)
                return 1;
        }
    }

    int tmp=*rep;

    for(int i=0;i<4;i++){
        (*rep)=tmp;
        if(checkIteration(nGems,collTmp,pos,tot,i,rep,maxrep)){
            collTmp[pos]=gems[i];
            nGems[i]--;
            currval+=gemsVal[i];

            if(r_collValue(nGems,gemsVal,collTmp,collMax,tot,maxval,maxpos,maxrep,pos+1,currval,rep,maxvalpos))
                return 1;

            currval-=gemsVal[i];
            nGems[i]++;
        }
    }
    return 0;
}

//check
int checkValidity(char *collMax,int pos){
    int s=0,z=0;
    for(int i=0;i<pos;i++){
        switch(collMax[i]){
            case 's':
                s++;
                break;
            case 'z':
                z++;
                break;
        }
    }
    if(z<=s)
        return 1;
    return 0;
}

int checkIteration(int *nGems,char *collTmp,int pos,int tot,int i,int *rep,int maxrep){
    if(pos==tot) //perchè altrimenti scriviamo il tot+1esimo dato nel vettore collTmp[tot]
        return 0;
    int legal=0;

    if(nGems[i]>0){ //check ordine sequenza
        switch(collTmp[pos-1]){
            case 'z':
            case 't':
                if(gems[i]=='z'||gems[i]=='r'){
                    legal=1;
                }
                break;
            case 's':
            case 'r':
                if(gems[i]=='s'||gems[i]=='t') {
                    legal=1;
                }
                break;
        }
    }

    if(legal) //checko la ripetizione della gemma
    {
        if(collTmp[pos-1]==gems[i])
        {
            if(*rep==maxrep) //se stiamo per sforare maxrep al prossimo ciclo
                legal=0;
            else
                (*rep)++;
        }
        else
            (*rep)=1;
    }

    return legal;
}


//file handling
void openFile(FILE **fp,char *fileName){
    *fp=fopen(fileName,"r");
    if(*fp==NULL){
        printf("Problemi nell'apertura del file");
        exit(-1);
    }
}

void readIndex(FILE *fp,int *p_size){
    if((fscanf(fp,"%d",p_size))==EOF)
    {
        printf("Errore nella lettura del file");
        exit(-1);
    }
}

void fillMatrix(FILE *fp,int **gemsNum,int **gemsVal,int *maxreps,int size)
{
    for(int i=0;i<size;i++)
    {
        if(fscanf(fp,"%d %d %d %d %d %d %d %d %d",&gemsNum[i][0],&gemsNum[i][1],&gemsNum[i][2],&gemsNum[i][3],
                  &gemsVal[i][0],&gemsVal[i][1],&gemsVal[i][2],&gemsVal[i][3],&maxreps[i])!=9)
        {
            printf("Errore nella lettura del file");
            exit(-1);
        }
    }
}

// mem
void malloc2Dp(int*** mat, int nr,int nc){
    *mat = (int**)malloc(nr*sizeof(**mat));
    if(*mat==NULL)
    {
        printf("Errore nell'allocazione della memoria");
        exit(-1);
    }
    else{
        for(int i=0;i<nr;i++)
        {
            (*mat)[i]=(int*)malloc(nc*sizeof(***mat));
            if((*mat)[i]==NULL)
            {
                printf("Errore nell'allocazione della memoria");
                exit(-1);
            }
        }
    }
}

void free2Dp(int** mat, int nr){
    for(int i=0;i<nr;i++){
        free(mat[i]);
    }
    free(mat);
}

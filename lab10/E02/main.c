#include <bits/types/FILE.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_GEMS 4
#define NUM_RULES 2

void openFile(FILE **fp,char *fileName);
void readIndex(FILE *fp,int *size);
void fillMatrix(FILE *fp,int **gemSets,int size);
void fillMaxGems(int* maxGems,int **gemSets,int size);

int gemToId(char gem);
int getMaxColl_rmem(int *mem,int pos,char oldc,int *nGems,int *maxGems); 
int getMaxColl_mem(int *nGems,int *maxGems,int *mem); //wrapper

//mem - 5 dimensioni 'virtuali' dentro singolo vett
int* getMemCell(int *mem,char lastgem,int *nGems,int *maxGems); //ritorna la cella di memoria con pos [lastgem,nGems[0],nGems[1],nGems[2],nGems[3]]
int getResMem(int *mem,char lastgem,int *nGems,int *maxGems); //ritorna il val nella cella di mem
void setResMem(int *mem,char lastgem,int *nGems,int *maxGems,int num);
void initMem(int *mem,int *maxGems,int val);

void malloc2Dp(int*** gemSets, int nr,int nc);
void free2Dp(int** gemSets, int nr);
void mallocMem(int **mem,int *maxGems);
void freeMem(int *mem);

const char gems[N_GEMS]="zrts";

/*
  struttura dati mem 5D vettore 1D
  Malloco struttura su dimensione f(maxGems),così per non riallocare la struttura
  Inoltre i risultati trovati si propagano per i vari set di gemme
  "mem"[lastgem,nGems[0],nGems[1],nGems[2],nGems[3]]
*/
//gemSets: gemSets[i] richiesta i-esima di risoluzione collana | gemSets[i] = {2,3,4,5} //nGems

int main() {

    FILE *fp;
    int size;

    int **gemSets;
    int *maxGems;

    int *mem; //vett dove verrà salvata la nostra struttura dati

    openFile(&fp,"hard_test_set.txt");
    readIndex(fp, &size);

    malloc2Dp(&gemSets,size,N_GEMS);
    maxGems = (int*) malloc(N_GEMS*sizeof(int));

    if(gemSets==NULL||maxGems==NULL)
        return 1;

    fillMatrix(fp, gemSets, size);
    fillMaxGems(maxGems,gemSets,size);

    mallocMem(&mem,maxGems);
    if(mem==NULL)
        return 2;
    initMem(mem,maxGems,-1);

    for (int i = 0; i < size; i++){
        printf("%d) Lunghezza: %d\n",i+1,getMaxColl_mem(gemSets[i],maxGems,mem));
    }

    free(maxGems);
    free2Dp(gemSets,size);
    freeMem(mem);

    return 0;
}

int getMaxColl_mem(int *nGems, int *maxGems,int *mem){
    int maxsize=0,size;

    for(int i=0;i<4;i++){
        nGems[i]--;
        size = 1 + getMaxColl_rmem(mem,1,gems[i],nGems,maxGems);
        if(maxsize<size)
            maxsize=size;
        nGems[i]++;
    }

    return maxsize;
}

//nGems funziona da mark
int getMaxColl_rmem(int *mem,int pos,char oldc,int *nGems,int *maxGems){
    int size;
    int maxsize=0;

    if((size=getResMem(mem,oldc,nGems,maxGems))!=-1)
        return size;

    for(int i=0;i<N_GEMS;i++){
        if(nGems[i]>0)
            switch(oldc){
                case 'z':
                case 't':
                    if(gems[i]=='z'||gems[i]=='r'){
                        nGems[i]--;

                        size = 1 + getMaxColl_rmem(mem,pos+1,gems[i],nGems,maxGems);

                        if(maxsize<size) //esiste una soluzione intera
                            maxsize=size;
                        nGems[i]++;
                    }
                    break;
                case 's':
                case 'r':
                    if(gems[i]=='s'||gems[i]=='t') {
                        nGems[i]--;
                        size = 1 + getMaxColl_rmem(mem,pos+1,gems[i],nGems,maxGems);
                        if(maxsize<size) //esiste una soluzione intera
                            maxsize=size;
                        nGems[i]++;
                    }
                    break;
            }
    }

    setResMem(mem,oldc,nGems,maxGems,size);
    return maxsize;
}

void fillMatrix(FILE *fp,int **gemSets,int size)
{
    for(int i=0;i<size;i++)
    {
        if(fscanf(fp,"%d %d %d %d",&gemSets[i][0],&gemSets[i][1],&gemSets[i][2],&gemSets[i][3])!=N_GEMS)
        {
            printf("Errore nella lettura del file");
            exit(1);
        }
    }
}

void fillMaxGems(int* maxGems,int **gemSets,int size){
    for (int i = 0; i < N_GEMS; i++)
        maxGems[i]=0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < N_GEMS; j++) {
            if (maxGems[j]<gemSets[i][j])
                maxGems[j]=gemSets[i][j];
        }
    }
}

//mem
void malloc2Dp(int*** gemSets, int nr,int nc){
    *gemSets = (int**)malloc(nr*sizeof(**gemSets));
    if(*gemSets==NULL)
    {
        printf("Errore nell'allocazione della memoria");
        exit(2);
    }
    else{
        for(int i=0;i<nr;i++)
        {
            (*gemSets)[i]=(int*)malloc(nc*sizeof(***gemSets));
            if((*gemSets)[i]==NULL)
            {
                printf("Errore nell'allocazione della memoria");
                exit(2);
            }
        }
    }
}

void free2Dp(int** gemSets, int nr){
    for(int i=0;i<nr;i++){
        free(gemSets[i]);
    }
    free(gemSets);
}


void mallocMem(int **mem,int *maxGems){
    int dim=N_GEMS;
    for(int i=0;i<4;i++) {
        dim*=maxGems[i]+1;
    }
    *mem=(int*)malloc(dim*sizeof(int));
    if(*mem==NULL){
        printf("Errore nell'allocazione della memoria");
        exit(2);
    }
}

void freeMem(int *mem){
    free(mem);
}

void initMem(int *mem,int *maxGems,int val){
    int dim=N_GEMS;
    int i;
    for(i=0;i<4;i++) {
        dim*=maxGems[i]+1;
    }
    for(i=0;i<dim;i++)
        mem[i]=val;

}

int* getMemCell(int *mem,char lastgem,int *nGems,int *maxGems){
    int layer = N_GEMS;
    int res;
    int gemId = gemToId(lastgem);

    for(int i=0;i<N_GEMS;i++) {
        layer*=maxGems[i]+1;
    }
    layer/=N_GEMS;
    res=layer*gemId;
    for(int i=0;i<N_GEMS;i++) {
        layer/=maxGems[i]+1;
        res+=layer*nGems[i];
    }
    return &(mem[res]);
}

int getResMem(int *mem,char lastgem,int *nGems,int *maxGems){
    return *getMemCell(mem,lastgem,nGems,maxGems);
}

void setResMem(int *mem,char lastgem,int *nGems,int *maxGems,int num){
     *getMemCell(mem,lastgem,nGems,maxGems)=num;
}

void openFile(FILE **fp,char *fileName){
    *fp=fopen(fileName,"r");
    if(*fp==NULL){
        printf("Problemi nell'apertura del file");
        exit(1);
    }
}

void readIndex(FILE *fp,int *p_size){
    if((fscanf(fp,"%d",p_size))==EOF)
    {
        printf("Errore nella lettura del file");
        exit(1);
    }
}

int gemToId(char gem) {
    for (int i = 0; i < N_GEMS; i++) {
        if (gem == gems[i])
            return i;
    }
    return -1;
}
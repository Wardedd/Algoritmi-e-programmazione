#include <bits/types/FILE.h>
#include <stdio.h>
#include <stdlib.h>

void openFile(FILE **fp,char *fileName);
void readIndex(FILE *fp,int *size);
void fillMatrix(FILE *fp,int **mat,int size);
void malloc2Dp(int*** mat, int nr,int nc);
void free2Dp(int** mat, int nr);

int r_longesColl(int *nGems,int pos,char *collTmp,int k);

int findCollanaK(int nGems[4],char *collTmp,int k);
void findCollanaDicotomy_r(int nGems[4],char *collTmp,int l, int r);
void findCollanaDicotomy(int nGems[4]);

//file z s r t
const char gems[4]="zrts";

int main() {
    FILE *fp;
    int size;
    int **mat;

    openFile(&fp,"easy_test_set.txt");
    readIndex(fp, &size);

    malloc2Dp(&mat,size,4);
    fillMatrix(fp, mat, size);

    for (int i = 0; i < size; i++){
        printf("Collana della sequenza numero %d\n",i+1);
        findCollanaDicotomy(mat[i]);
        printf("\n");
    }
    free2Dp(mat,size);
    return 0;
}

void free2Dp(int** mat, int nr){
    for(int i=0;i<nr;i++){
        free(mat[i]);
    }
    free(mat);
}

void findCollanaDicotomy(int nGems[4]){
    int tot = nGems[0]+nGems[1]+nGems[2]+nGems[3];
    if(tot==0)
    {
        printf("Nessuna collana trovata");
        return;
    }
    int l=0;
    int r=tot+1; //r non incluso
    char collTmp[tot];
    findCollanaDicotomy_r(nGems,collTmp,l,r);
}

void findCollanaDicotomy_r(int nGemsBackup[4],char *collTmp,int l, int r){

    if(r-l==1)
    {
        if(findCollanaK(nGemsBackup,collTmp,l))
            for(int i=0;i<l;i++)
                printf("%c",collTmp[i]);
        else
        {
            printf("Nessuna collana trovata");
        }
    }
    else{
        // quando uso findCollanaK su un insieme di gemme, modifico nGems permanentemente.
        // Quindi mi tengo un backup di nGems per ogni ciclo della dicotomia
        int nGems[4];
        for(int i=0;i<4;i++){
            nGems[i]=nGemsBackup[i];
        }

        if(findCollanaK(nGems,collTmp,(r+l)/2))
        {
            findCollanaDicotomy_r(nGemsBackup,collTmp,(r+l)/2,r);
        }
        else
        {
            findCollanaDicotomy_r(nGemsBackup,collTmp,l,(r+l)/2);
        }
    }
}

int findCollanaK(int nGems[4],char *collTmp,int k){
    for(int i=0;i<4;i++){
        if(nGems[i]>0) {
            collTmp[0] = gems[i];
            nGems[i]--;
            if(r_longesColl(nGems,1,collTmp,k)){
                return 1;
            }
            nGems[i]++;
        }
    }
    return 0;
}

//nGems funziona da mark
int r_longesColl(int *nGems,int pos,char *collTmp,int k){
    if(pos==k)
    {
        return 1;
    }
    for(int i=0;i<4;i++){
        if(nGems[i]>0)
            switch(collTmp[pos-1]){
                case 'z':
                case 't':
                    if(gems[i]=='z'||gems[i]=='r'){
                        collTmp[pos]=gems[i];
                        nGems[i]--;
                        if(r_longesColl(nGems,pos+1,collTmp,k))
                            return 1;
                        nGems[i]++;
                    }
                    break;
                case 's':
                case 'r':
                    if(gems[i]=='s'||gems[i]=='t') {
                        collTmp[pos]=gems[i];
                        nGems[i]--;
                        if(r_longesColl(nGems,pos+1,collTmp,k))
                            return 1;
                        nGems[i]++;
                    }
                    break;
            }
    }

    return 0;
}

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

void fillMatrix(FILE *fp,int **mat,int size)
{
    for(int i=0;i<size;i++)
    {
        if(fscanf(fp,"%d %d %d %d",&mat[i][0],&mat[i][1],&mat[i][2],&mat[i][3])!=4)
        {
            printf("Errore nella lettura del file");
            exit(-1);
        }
    }
}

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
#include "support.h"
void readIndex(FILE *fp,int *p_num)
{
    if((fscanf(fp,"%d",p_num))==EOF)
    {
        printf("Errore nella lettura del file");
        exit(-1);
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

void openFile(FILE **fp,char *fileName,char *modes)
{
    *fp=fopen(fileName,modes);
    if(*fp==NULL)
    {
        printf("Problemi nell'apertura del file");
        exit(-1);
    }
}

void malloc2Dp(int*** mat, int nr,int nc){
    *mat = (int**)malloc(nr*sizeof(**mat));
    if(*mat==NULL)
    {
        printf("Errore nell'allocazione della memoria");
        exit(2);
    }
    else{
        for(int i=0;i<nr;i++)
        {
            (*mat)[i]=(int*)malloc(nc*sizeof(***mat));
            if((*mat)[i]==NULL)
            {
                printf("Errore nell'allocazione della memoria");
                exit(2);
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

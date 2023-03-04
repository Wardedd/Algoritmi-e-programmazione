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
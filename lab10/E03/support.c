#include <string.h>
#include "support.h"

void readNum(FILE *fp, int *p_num)
{
    if((fscanf(fp,"%d",p_num))==EOF)
    {
        printf("Errore nella lettura del file");
        exit(-1);
    }
}

int getFileLines(FILE *fp, char* p){
    if(fp==NULL)
        exit(6);

    char pattern[20];
    int dataint;
    char datastr[20];
    int flag=1;
    int lines=0;
    char *strcode;

    while(!feof(fp)&&flag){
        strcpy(pattern,p);
        strcode=strtok(pattern," ");
        while(strcode!=NULL && flag) {
            if (strcmp(strcode, "%s") == 0)
            {
                if (fscanf(fp, "%s", datastr) != 1)
                    flag = 0;
            }
            else if(strcmp(strcode,"%d")==0)
            {
                if(fscanf(fp,"%d",&dataint)!=1)
                    flag=0;
            }
            else
                flag=0;

            strcode=strtok(NULL," ");
        }

        if(flag==1)
            lines++;
    }
    rewind(fp);
    return lines;
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
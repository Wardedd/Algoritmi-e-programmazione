#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int s,f;
}att;

int getD(att a){
    return a.f-a.s;
}
int attCompatible(att a, att b){
    return !(a.s<b.f&&b.s<a.f);
}

void fillArr(FILE *fp, att* attArr, int n);
int getMaxSchedule(att* attArr,int n);
void powerset_perms(att* attArr,att* currArr,att* maxArr,int start,int n, int pos,int *maxpos,int currd,int *maxd);
int checkIteration(att *currArr,int pos,att a);
int isPromising(int currd,att *attArr,int n,int pos,int *maxd);

void openFile(FILE **fp,char *fileName);
void readIndex(FILE *fp,int *p_size);

//idea iniziale powerset combinazioni semplici, ordine non conta

int main(){
    int n;
    FILE *fp;
    att *attArr;

    openFile(&fp,"att2.txt");
    readIndex(fp,&n);

    attArr = (att*)malloc(n*sizeof(att));
    fillArr(fp,attArr,n);

    printf("%d",getMaxSchedule(attArr,n));
    free(attArr);
    return 0;
}

void fillArr(FILE *fp, att* attArr, int n){
    for(int i=0;i<n;i++){
        if(fscanf(fp,"%d %d",&attArr[i].s,&attArr[i].f)!=2){
            printf("Problema nella lettura delle att nel file");
            exit(-1);
        }
    }
}

int getMaxSchedule(att* attArr,int n){
    int maxd=0;
    int maxpos=0;
    att* maxArr;
    att* currArr;

    maxArr = (att*)malloc(n*sizeof(att));
    currArr = (att*)malloc(n*sizeof(att));

    powerset_perms(attArr,currArr,maxArr,0,n,0,&maxpos,0,&maxd);
    free(maxArr);
    free(currArr);

    return maxd;
}

void powerset_perms(att* attArr,att* currArr,att* maxArr,int start,int n, int pos,int *maxpos,int currd,int *maxd){
    if(currd>*maxd){
        for(int i=0;i<pos;i++){
            maxArr[i]=currArr[i];
        }
        *maxd=currd;
        *maxpos=pos;
    }

    for(int i=start;i<n;i++){
        if(checkIteration(currArr,pos,attArr[i]))
        {
            if(isPromising(currd,attArr,n,pos,maxd)) {
                currArr[pos] = attArr[i];
                powerset_perms(attArr, currArr, maxArr, i + 1, n, pos + 1, maxpos, currd + getD((attArr[i])), maxd);
            }
        }
    }
}

int isPromising(int currd,att *attArr,int n,int pos,int *maxd){
    for(int i=pos;i<n;i++)
        currd+=getD(attArr[i]);

    if(currd>=*maxd)
        return 1;
    return 0;
}

int checkIteration(att *currArr,int pos,att a){
    for(int i=0;i<pos;i++){
        if(!attCompatible(currArr[i],a))
            return 0;
    }
    return 1;
}

//old
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
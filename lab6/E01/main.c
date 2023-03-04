#include <stdlib.h>
#include <bits/types/FILE.h>
#include <stdio.h>
#include <memory.h>

typedef struct{
    char **songNames;
    int n_songs;
}Friend;

void openFile(FILE **fp);
void readIndex(FILE **fp,int *pNum);

void mallocList(Friend **list,int size);
void mallocFriend(Friend *friend);
void mallocAndCopySong(Friend *friend,char songs[256],int num);
void fillMatrix(FILE **fp,Friend *list,int n_friends);

void printCombinations(Friend *list,int lastliv); //intervallo da 0 a 4 (5 livelli), lastliv(2° param) = 4
void printCombinationsWrap(Friend *list,int lastliv, int liv,char **strres,int *count);

void freeSong(Friend *friend);
void freeFriend(Friend *friend);
Friend* freeAll(Friend *list, int size);

int main(){
    FILE *fp;
    openFile(&fp);
    Friend *list;
    int friendsNum;

    openFile(&fp);
    readIndex(&fp,&friendsNum); //get num amici

    mallocList(&list,friendsNum); //alloco spazio per i puntatori a Friend
    fillMatrix(&fp,list,friendsNum);

    printCombinations(list,friendsNum-1);
    freeAll(list,friendsNum);
    return 0;
}

void openFile(FILE **fp){
    *fp=fopen("brani.txt","r");
    if(*fp==NULL){
        printf("Problemi nell'apertura del file");
        exit(-1);
    }
}

void readIndex(FILE **fp,int *p_num){
    if((fscanf(*fp,"%d",p_num))==EOF)
    {
        printf("Errore nella lettura del file");
        exit(-1);
    }
}

void fillMatrix(FILE **fp,Friend *list,int n_friends){
    int n_songs;
    char tmpstr[256];
    for(int i=0;i<n_friends;i++){
        readIndex(&(*fp),&n_songs);

        list[i].n_songs=n_songs;
        mallocFriend(&list[i]);

        for(int j=0;j<n_songs;j++){
            fscanf(*fp,"%s ",tmpstr);
            mallocAndCopySong(&list[i],tmpstr,j);
        }
    }
}

void mallocList(Friend **list,int size){
    *list=(Friend*)malloc(size*sizeof(**list));
}

void mallocFriend(Friend *friend){
    friend->songNames=(char **)malloc(friend->n_songs*sizeof(friend->songNames));
}

void mallocAndCopySong(Friend *friend,char song[256],int numsong) {
    friend->songNames[numsong] = (char *) malloc((strlen(song) + 1) * sizeof(friend->songNames[numsong]));
    strcpy(friend->songNames[numsong], song);
}

void printCombinations(Friend *list,int lastliv){
    //malloco strres
    char **strres=(char**)malloc((lastliv+1)*sizeof(*strres));
    int count=0;
    printCombinationsWrap(list,lastliv,0,strres,&count);
    printf("Numero scelte:%d",count);
    free(strres);
}

void printCombinationsWrap(Friend *list,int lastliv, int liv,char **strres,int *count){

    if(liv>lastliv) //superato last liv printa il risultato trovato, vai a capo
    {
        for(int i=0;i<=lastliv;i++)
            printf("%s\n",strres[i]);
        printf("\n");
        (*count)++;
        return;
    }
    else{
        for(int i=0;i<list[liv].n_songs;i++){  //per ogni canzone usa ricorsione per andare nel livello successivo e continuare la ricorsione
            strres[liv]=list[liv].songNames[i];
            liv++;
            printCombinationsWrap(list,lastliv,liv,strres,count);
            liv--;
        }
    }
}

//free
void freeSong(Friend *friend){
    for(int i=0;i<friend->n_songs;i++)
        free(friend->songNames[i]);
}
void freeFriend(Friend *friend){
    free(friend->songNames);
}

Friend* freeAll(Friend *list, int size){
    for(int i=0;i<size;i++){
        freeSong(&(list[i]));
        freeFriend(&(list[i]));
    }
    free(list);
}
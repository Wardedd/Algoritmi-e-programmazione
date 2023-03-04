#include <stdlib.h>
#include <bits/types/FILE.h>
#include <stdio.h>
#include <memory.h>

#define MALLOC_ERR_DESC "Errore nell'allocazione della memoria"
#define FILE_OPEN_ERR_DESC "Problemi nell'apertura del file"

void malloc2Dp(int*** mat, int nr,int nc);
void free2Dp(int ***mat,int nr);

void fillMatrix(FILE *fp,int **matrix,int nr,int nc, int *nwcells,int *nbcells); //riempie matrix, conta il numero delle celle bianche e nere
void separa(int **mat, int nr, int nc, int *w_cells,int *b_cells,int w_size,int b_size); //riempie, printa e dealloca w_cells,b_cells

void printArr(char *desc,int *arr,int size);
void openFile(FILE **fp);
void readIndex(FILE *fp,int *p_num);
void checkPointer(void *p,char *s);

int main(){
    int nr,nc,w_size,b_size;
    int **scacchiera,*w_cells,*b_cells;
    FILE *fp;

    openFile(&fp);
    readIndex(fp,&nr);
    readIndex(fp,&nc);
    malloc2Dp(&scacchiera,nr,nc);

    w_size=0;b_size=0;
    w_cells=(int*)malloc(w_size*sizeof(*w_cells));
    b_cells=(int*)malloc(b_size*sizeof(*b_cells));

    fillMatrix(fp,scacchiera,nr,nc,&w_size,&b_size);
    separa(scacchiera,nr,nc,w_cells,b_cells,w_size,b_size); //wcells bcells deallocate qui dentro

    free2Dp(&scacchiera,nr);
}

void separa(int **mat, int nr, int nc, int *w_cells,int *b_cells,int w_size,int b_size){ //incrementando la lunghezza dinamicamente - data scacchiera cubica
    int w_counter=0,b_counter=0;
    for(int i=0;i<nr;i++){
        for(int j=0;j<nc;j++)
            if((i+j)%2==0) //bianco (prima cella)
                w_cells[w_counter++]=mat[i][j];
            else
                b_cells[b_counter++]=mat[i][j];
    }

    printArr("Celle bianche:",w_cells,w_size);
    printArr("Celle nere:",b_cells,b_size);
    free(b_cells);
    free(w_cells);
    b_cells=NULL;
    w_cells=NULL;
}

// read/print
void printArr(char *desc,int *arr,int size){
    printf("%s\n",desc);
    if(size==0)
        printf("Nessuna");
    for(int i=0;i<size;i++)
        printf("%d ",arr[i]);
    printf("\n");

}

void openFile(FILE **fp){
    *fp=fopen("mat.txt","r");
    checkPointer(*fp,FILE_OPEN_ERR_DESC);
}

void fillMatrix(FILE *fp,int **matrix,int nr,int nc, int *nwcells,int *nbcells){
    int tmp;
    int worb=0; //w = 1 -> b = 0;
    for(int i=0;i<nr;i++){
        for(int j=0;j<nc;j++){
            readIndex(fp,&tmp);
            matrix[i][j]=tmp;
            if(worb==0)
            {
                (*nwcells)++;
                worb=1;
            }else
            {
                (*nbcells)++;
                worb=0;
            }
        }
    }
}

void readIndex(FILE *fp,int *p_num){
    if((fscanf(fp,"%d",p_num))==EOF)
    {
        printf("Errore nella lettura del file");
        exit(-1);
    }
}

void checkPointer(void *p,char *s){
    if(p==NULL)
    {
        printf("%s",s);
        exit(-1);
    }
}

//mem dinamica
void malloc2Dp(int*** mat, int nr,int nc){
    *mat = (int**)malloc(nr*sizeof(**mat));
    checkPointer(*mat,MALLOC_ERR_DESC);

    for(int i=0;i<nr;i++)
    {
        (*mat)[i]=(int*)malloc(nc*sizeof(***mat));
        checkPointer((*mat)[i],MALLOC_ERR_DESC);
    }
}

void free2Dp(int ***mat,int nr){
    for(int i=0;i<nr;i++){
        free((*mat)[i]);
    }
    free(*mat);
    *mat=NULL;
}
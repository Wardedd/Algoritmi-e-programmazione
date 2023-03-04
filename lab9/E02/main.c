//disposizioni semplici,ordine conta, pruning
#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char cv,co; //colore vert, orizz
    int vv,vo;  //valore vert, orizz
}tile;

tile* NEWTileArray(int n){
    return (tile*)malloc(n*sizeof(tile));
}
void FREEtilearray(tile* tilearr){
    free(tilearr);
}

void openFile(FILE **fp,char *fileName);
void readIndex(FILE *fp,int *p_size);
int fillBoard(FILE *fp,int *mark,int* board,int *rot,int R, int C);
void fillTiles(FILE *fp,tile *tiles, int T);

int getBoardValue(int *board ,int *rot, tile *tiles,int R,int C);

void printBestBoard(int *board ,int *rot,int *mark, tile *tiles,int R,int C,int T);
void dispsempl(int *board ,int *rot, int *maxBoard,int *maxrot,tile *tiles,int *mark,int R,int C,int T,int *maxval,int pos,int n);

//vet mark indica se la cella è: permanente, vuota o piena
//viene quindi usato nella ricorsione

//disp sempl (possiamo avere + ),

int main(){
    int T,R,C;
    FILE *fp1,*fp2;

    openFile(&fp1,"tiles.txt");
    openFile(&fp2,"board.txt");

    readIndex(fp1,&T);
    readIndex(fp2,&R);
    readIndex(fp2,&C);

    tile* tiles;
    int *board,*rot,*mark;

    tiles=NEWTileArray(T);
    board=(int*) malloc(R*C*sizeof(int));
    mark=(int*) malloc(T*sizeof(int));
    rot=(int*) malloc(R*C*sizeof(int));

    fillTiles(fp1,tiles,T);
    fillBoard(fp2,mark,board,rot,R,C);

    printBestBoard(board,rot,mark,tiles,R,C,T);

    fclose(fp1);
    fclose(fp2);

    FREEtilearray(tiles);
    free(board);
    free(mark);
    free(rot);
    return 0;
}

void printBestBoard(int *board ,int *rot,int *mark, tile *tiles,int R,int C,int T){
    int *maxboard;
    int *maxrot;
    int maxval=0;
    int n=R*C;

    maxboard=(int*)malloc(n*sizeof(int));
    maxrot=(int*)malloc(n*sizeof(int));

    dispsempl(board,rot,maxboard,maxrot,tiles,mark,R,C,T,&maxval,0,n);

    printf("Valore: %d\n",maxval);

    for(int i=0;i<R;i++){
        for(int j=0;j<C;j++){
            printf("%d\\%d ",maxboard[i*C+j],maxrot[i*C+j]);
        }
        printf("\n");
    }

    free(maxboard);
    free(maxrot);
}

void dispsempl(int *board ,int *rot, int *maxBoard,int *maxrot,tile *tiles,int *mark,int R,int C,int T,int *maxval,int pos,int k) {
    if (pos == k) {
        int currval = getBoardValue(board, rot, tiles, R, C);
        if (currval > *maxval) {
            *maxval = currval;
            for (int i = 0; i < k; i++) {
                maxBoard[i] = board[i];
                maxrot[i] = rot[i];
            }
        }
        return;
    }
    else {
        if (mark[board[pos]] == 2) { //se stiamo per operare in una pos. con tile permanete, skippa
            dispsempl(board, rot, maxBoard, maxrot, tiles, mark, R, C, T, maxval, pos + 1, k);
        }
        else{
            for (int i = 0; i < T; i++) { //se possiamo operare nella pos, inserisci una tile disponibile / mark[i]==0
                if (mark[i] == 0) {
                    board[pos] = i;
                    rot[pos] = 0;
                    mark[i] = 1;

                    dispsempl(board, rot, maxBoard, maxrot, tiles, mark, R, C, T, maxval, pos + 1, k);

                    rot[pos] = 1;

                    dispsempl(board, rot, maxBoard, maxrot, tiles, mark, R, C, T, maxval, pos + 1, k);

                    mark[i] = 0;
                }
            }
        }
    }
}

int getBoardValue(int *board ,int *rot, tile *tiles,int R,int C){
    int val=0,tmpval,cellval;
    int streak;
    char oldc,cellc;

    for(int i=0;i<C;i++){ //conto le righe verticali
        streak=1;
        tmpval=(rot[i]==1)?tiles[board[i]].vo:tiles[board[i]].vv;
        oldc=(rot[i]==1)?tiles[board[i]].co:tiles[board[i]].cv;

        for(int j=1;j<R&&streak;j++){
            cellc=(rot[i+C*j]==1)?tiles[board[i+C*j]].co:tiles[board[i+C*j]].cv;
            cellval=(rot[i+C*j]==1)?tiles[board[i+C*j]].vo:tiles[board[i+C*j]].vv;

            if(oldc!=cellc){
                streak=0;
            }
            else{
                tmpval+=cellval;
            }
        }
        if(streak){
            val+=tmpval;
        }
    }


    for(int i=0;i<R;i++){ //conto le righe orizzontali
        streak=1;
        tmpval=(rot[i*C]==1)?tiles[board[i*C]].vv:tiles[board[i*C]].vo;
        oldc=(rot[i*C]==1)?tiles[board[i*C]].cv:tiles[board[i*C]].co;

        for(int j=1;j<C&&streak;j++){
            cellc=(rot[i*C+j]==1)?tiles[board[i*C+j]].cv:tiles[board[i*C+j]].co;
            cellval=(rot[i*C+j]==1)?tiles[board[i*C+j]].vv:tiles[board[i*C+j]].vo;

            if(oldc!=cellc){
                streak=0;
            }
            else{
                tmpval+=cellval;
            }
        }
        if(streak){
            val+=tmpval;
        }
    }
    return val;
}

void fillTiles(FILE *fp,tile *tiles, int T){
    for(int i=0;i<T;i++){
        if((fscanf(fp," %c %d %c %d ",&tiles[i].co,&tiles[i].vo,&tiles[i].cv,&tiles[i].vv))!=4)
        {
            printf("Errore nella lettura del file delle Tiles");
            exit(-1);
        }
    }
}

int fillBoard(FILE *fp,int *mark,int* board,int *rot,int R, int C){
    int permaTiles=0;

    for(int i=0;i<R;i++){
        for(int j=0;j<C;j++){
            if((fscanf(fp," %d/%d ",&board[i*C+j],&rot[i*C+j]))!=2)
            {
                printf("Errore nella lettura del file della Board");
                exit(-1);
            }
            else{
                if(rot[i*C+j]==-1){
                    mark[board[i*C+j]] = 0;
                }
                else {
                    mark[board[i*C+j]] = 2; //marco che la tile è usata permanentemente, board è l'indice della tile
                    permaTiles++;
                }
            }
        }
    }
    return permaTiles;
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
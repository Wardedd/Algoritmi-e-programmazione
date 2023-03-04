#include <stdio.h>
//non finito
typedef struct {
    int x, width;
    int y, length;
}Rect; //xf,yf not included in rect

#define FILENAME "mat.txt"
#define MAXR 50

void setRect(Rect *rect,int x,int y, int width, int length);
void printRect(Rect rect,char desc[20]);
int calcArea(Rect rect);

Rect findRect(int nr,int nc,int matrix[nr][nc],int x,int y);
int checkUsedRect(Rect rect,int nr,int nc,int matrix[nr][nc]);
int printMaxRects(int nr,int nc,int matrix[nr][nc]);

int fillMatrixFile(FILE *fp,int rows,int columns,int matrix[rows][columns]);

int leggiMatrice(char M[MAXR][MAXR],int *nr,int *nc); //maxr è contenuto in nr e nc
int riconosciRegione(int **M,int nr,int nc,int r,int c,int *b,int *h);

int leggiMatrice(char M[MAXR][MAXR],int *nr,int *nc){
    FILE *fp;
    fp=fopen(FILENAME,"r");

    if(fp==NULL){
        printf("Couldn't access file");
        return -1;
    }

    if(fscanf(fp,"%d %d\n",nr,nc)!=2){
        printf("Couldn't access main indexes correctly\n");
        return -2;
    }

    if((*nr)>50)
        (*nr)=50;
    if((*nc)>50)
        (*nc)=50;

    int tmp;
    int maxRow=0,maxCol=0,newCol=0;
    int col=0,row=0;

    while((tmp=(int)fgetc(fp))!=EOF&&col<(*nc)){
        switch(tmp){
            case '1':
                row++;
                M[col][row]=tmp-48;

                if(row>maxRow)
                    maxRow=row;

                if(col>maxCol && newCol) {
                    maxCol = col;
                    newCol=0;
                }
                break;

            case '0':
                row++;
                M[col][row]=tmp-48;
                break;

            case '\n':
                newCol=1;
                row=0;
                col++;
                break;
        }
        if(row>=(*nr))
        {
            fclose(fp);
            return 0;
        }
    }

    (*nc)=maxCol;
    (*nr)=maxRow;

    return 1;
}

int main(){

    int nr=MAXR,nc=MAXR;
    int matrix[MAXR][MAXR];

/*    if(!fillMatrixFile(fp,nr,nc,matrix))
    {
        printf("Couldn't get matrix correctly");
    }
*/
    if(!leggiMatrice(   matrix,&nr,&nc))
    {
        printf("Couldn't get matrix correctly");
    }

    if(!printMaxRects(nr,nc,matrix)){
        printf("No matrix was checked :(");
        return -3;
    }

    return 0;
}

void setRect(Rect *rect,int x,int y, int width, int length){
    (*rect).x=x;
    (*rect).y=y;
    (*rect).width=width;
    (*rect).length=length;
}

void printRect(Rect rect,char desc[20]){
    printf("Max %s: estr. sup. SX=<%d,%d> b=%d, h=%d, Area=%d\n",desc,rect.x,rect.y,rect.width,rect.length,calcArea(rect));
}

Rect findRect(int nr,int nc,int matrix[nr][nc],int x,int y)
{
    Rect rect;
    int xf,yf;
    for(xf=x+1;xf<nc && matrix[y][xf]==1;xf++);
    for(yf=y+1;yf<nr && matrix[yf][x]==1;yf++);
    setRect(&rect,x,y,xf-x,yf-y);
    return rect;
}

int calcArea(Rect rect){
    return rect.width*rect.length;
}

int printMaxRects(int nr,int nc,int matrix[nr][nc])
{
    Rect max[3]; //1:width 2:length 3:area
    char desc[3][20]={"base","altezza","area"};

    for(int i=0;i<3;i++) {
        setRect(&max[i], 0, 0, 0, 0); //1s because of Rect definition
    }

    for(int y=0;y<nr;y++)
    {
        for(int x=0;x<nc;x++)
        {
            if(matrix[y][x]==1) {
                Rect tmpRect=findRect(nr,nc, matrix, x, y);

                if (!checkUsedRect(tmpRect,nr,nc,matrix)){
                    if (tmpRect.width > max[0].width)
                        max[0] = tmpRect;
                    if (tmpRect.length > max[1].length)
                        max[1] = tmpRect;
                    if (calcArea(tmpRect) > calcArea(max[2]))
                        max[2] = tmpRect;
                    x=x+tmpRect.width-1;
                }
            }
        }
    }

    if(max[0].width==0)
        return 0;
    else
        for(int i=0;i<3;i++)
        {
            printRect(max[i],desc[i]);
        }

    return 1;
}

int checkUsedRect(Rect rect,int nr,int nc,int matrix[nr][nc])
{
    int used=0;
    if(rect.x!=0)
    {
        if(matrix[rect.y][rect.x-1]==1)
            used=1;
    }
    if(rect.y!=0 && !used)
    {
        if(matrix[rect.y-1][rect.x]==1)
            used=1;
    }
    return used;
}

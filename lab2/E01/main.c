#include <stdio.h>
typedef struct {
    int x, width;
    int y, length;
}Rect; //xf,yf not included in rect
#define FILENAME "mappa.txt"

void setRect(Rect *rect,int x,int y, int width, int length);
void printRect(Rect rect,char desc[20]);
int calcArea(Rect rect);

Rect findRect(int nr,int nc,int matrix[nr][nc],int x,int y);
int checkUsedRect(Rect rect,int nr,int nc,int matrix[nr][nc]);
int printMaxRects(int nr,int nc,int matrix[nr][nc]);

int fillMatrixFile(FILE *fp,int rows,int columns,int matrix[rows][columns]);

int main(){
    int nr,nc;
    FILE *fp;

    fp=fopen(FILENAME,"r");

    if(fp==NULL){
        printf("Couldn't access file");
        return -1;
    }

    if(fscanf(fp,"%d %d",&nr,&nc)!=2){
        printf("Couldn't access main indexes correctly");
        return -2;
    }
    int matrix[nr][nc];

    if(!fillMatrixFile(fp,nr,nc,matrix))
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

int fillMatrixFile(FILE *fp,int rows,int columns,int matrix[rows][columns]){
    for(int i=0;i<rows;i++){
        for(int j=0;j<columns;j++){
            if(fscanf(fp,"%d ",&matrix[i][j])!=1)
                return 0;
        }
    }
    return 1;
}
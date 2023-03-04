#include <stdio.h>

/*
 * condizione di terminazione
 * a%b==0 -> return b
 * restoa==0 && restob==1 ->calcola mcd
*/

int mcd(int a,int b);
int mcdr(int a,int b); //se primi->tutti e 2 odd->2 odd -> left even right odd

int main(){
    int a,b,res;
    printf("Inserisci 2 numeri interi: ");
    scanf("%d %d",&a,&b);
    res=mcd(a,b);

    if(res==-1)
        printf("Mcd non calcolabile");
    else
        printf("a:%d b:%d gcd(a,b):%d",a,b,res);


    return 0;
}

int mcd(int a,int b)
{
    if(a==0)
        return b;
    if(b==0)
        return a;

    return mcdr(a,b);
}

int mcdr(int a,int b){
    //casi speciali, 1 terminazione
    if(b>a)
        return mcdr(b,a);
    if(a%b==0) {
        return b;
    }

    int restoa=a%2,restob=b%2;

    if(restoa==0&&restob==0)
        return 2*mcdr(a/2,b/2);
    else if(restoa==1&&restob==0)
        return mcdr(a,b/2);
    else if(restoa==1&&restob==1) {
        return mcdr((a - b) / 2, b);
    }
    else{ //resto a == 0 e restob==1, terminazione, calcolo mcd senza ricorsione
        int mcdr=b;
        if(b>1)
        {
            mcdr/=2;
            if(mcdr%2==0)
                mcdr--;
        }
        while(b>1 && (a%mcdr!=0 || b%mcdr!=0)){
            mcdr-=2;
        }
        return mcdr;
    }
}
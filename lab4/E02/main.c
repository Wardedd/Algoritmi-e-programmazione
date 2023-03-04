#include <stdio.h>

int majority_r( int *a, int N, int *occ); //-1 = no majority
int majority(int *a,int N);
/*
 divide and conquer n=2 a=2
 Condizione di terminazione N=1, setto occ=1 e ritorno come maggiorante a[0]
 Divido il vettore in 2 fino a cond. di terminazione
 se non esiste maggiorante di uno dei 2 sottovettori, -1
 se maggiorante esiste, vedi se sia un maggiorante anche in questa iterazione, controllando la parte mancante, e verificando con occ
*/

int main() {
    int N;
    printf("Quale sarà la dimensione del vettore? (deve essere >0 ): ");
    scanf("%d", &N);

    if (N <= 0){
        printf("Il vettore deve essere di grandezza >0");
        return -1;
    }
    int res,v[N];
    printf("Inserisci %d numeri, ognuno seguito da uno spazio: ",N);
    for(int i=0;i<N;i++){
        scanf("%d",&v[i]);
    }

    res = majority(v,N);
    if(res==-1)
        printf("Non è stato trovato un maggioritario");
    else
        printf("Il maggioritario è %d",majority(v,N));
    return 0;
}

int majority(int *a,int N) {
    int occ;
    return majority_r(a,N,&occ);
}

int majority_r(int *a,int N,int *occ) //-1 = no majority
{
    int maja, majb;
    int occa, occb;

    if (N == 1)
    {
        *occ=1;
        return a[0];
    }
    maja = majority_r(a, N/2,&occa);
    if (N % 2 == 0)
        majb = majority_r(a+N/2, N/2,&occb);
    else
        majb = majority_r(a+N/2, N/2+1,&occb);

    if(maja!=-1)
    {
        for(int i=N/2;i<N;i++)
            if(a[i]==maja)
                occa++;
        if(occa>=N/2+1)
        {
            *occ=occa;
            return maja;
        }
    }
    if(majb!=-1)
    {
        for(int i=0;i<N/2;i++)
            if(a[i]==majb)
                occb++;
        if(occb>=N/2+1)
        {
            *occ=occb;
            return majb;
        }
    }
    return -1;
}
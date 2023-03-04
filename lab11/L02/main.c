#include <stdio.h>
#include <stdlib.h>

#include "support.h"
#include "diag.h"

#define MAX_FILENAME 50
#define PROG_S 3
#define DIAG_S 5

typedef enum {none,adouble,aback}state; //dai priorità a double, dai priorità a back

/*
approccio usato:
bound=0 cerca migliore tramite valore specifico + alto 
bound=1 cerca migliore tramite difficoltà + piccola (e ottimalità)

uso bound=0 per garantire soluzione, se ho spazio disponibile oppure ho finito le richieste del programma, bound=1

enum state mi indica quale delle richieste per avere una sol.
*/

elem** generateProgGreedy(elem* elems,int nElems,int DD,int DP);
float getVal(elem elem,state state,int bound,int pos);
elem getBest(elem* elems,int nElems,state state,dir lastDir,int bound,int pos);
elem **mallocProg(int rows,int cols);
void freeProg(elem** prog,int rows);
int elemIsVoid();
elem elemGetVoid();
void printProg();

int main(){
    int nElems, DD, DP;
    char fileName[MAX_FILENAME]="elementi.txt";
    FILE *fp;

    elem *elems;
    elem **prog;

    openFile(&fp,fileName,"r");
    readIndex(fp,&nElems);
        
    elems=malloc(nElems*sizeof(elem));
    if(elems==NULL) exit(-5);
    readElems(fp,elems,nElems);

	printf("Inserire DD DP: ");
	if(scanf("%d %d",&DD,&DP)!=2)
        	return 1;
	prog = generateProgGreedy(elems,nElems,DD,DP);

    if(prog==NULL)
        printf("Nessun programma trovato");
    else
        printProg(stdout,prog);

    freeProg(prog,PROG_S);
    free(elems);
    return 0;
}

elem** generateProgGreedy(elem* elems,int nElems,int DD,int DP){
 elem** prog = mallocProg(PROG_S,DIAG_S);

 int tmpDP=0,tmpDD;
 state state = adouble;
 int bounds=1;
 int acrobi=0;
 int acrostreak=0;
 int acrodouble=0;

 dir lastDir;
 elem tmpElem;
 type type;

 
 for(int y=0;y<PROG_S;y++){
 	tmpDD=0;
    acrostreak=0;
 	lastDir=dirf;

 	for(int x=0;x<DIAG_S;x++){
 		 tmpElem=getBest(elems,nElems,state,lastDir,bounds,x);

 		 if(tmpElem.dif+tmpDD<=DD && tmpDP+tmpDD+tmpElem.dif<=DP)
		 {
		     prog[y][x]=tmpElem;
		     tmpDD+=tmpElem.dif;
		 	
 	             type=tmpElem.type;

	             if(type==acrob) acrobi++;
	             	
	             if(type==acrob||type==acrof)
	                 acrostreak++;
        	     else
        	         acrostreak=0;
	
        	     if(acrostreak==2)
        	         acrodouble++;
        	     
        	     if(state==adouble && acrodouble>0)
        	     {
        		      if(DP/DD>=2)
                      {
        		          bounds=0;
        		          state=none;
                      }else{
                          state=aback;
                          //bounds rimane 1
        		      }
        	     }
        	     if(state==aback && acrobi>0)
        	     {
	        	      state=none;
        		      bounds=0;
        	     }
        	     lastDir=tmpElem.dirOut;

		 }
		 else
		 {
		 	for(x;x<DIAG_S;x++)
		 		prog[y][x]=elemGetVoid();	
			break;
		 }
 	}/*non va bene quando DP=DC come mai non cerca alti*/
 	if(state==none && acrobi==0)
	{
		state=aback;
		bounds=1;
	}
    tmpDP+=tmpDD;
 }
 
 if(state!=none)
	 return NULL;
 return prog;
}

float getVal(elem elem,state state,int bound,int pos){
	if(pos==0 && elem.isSecond==strue) //siamo a start, serve sfalse
    {
	    return -1;
    }

    float val;
	val=1/(float)elem.dif;
	if(bound==0)
		val*=elem.val; //ordino per valore/difficoltà
    else{
        val+=(float)elem.val/100; //per far elem. diff. uguali vinca quello con val >
    }
	if(state==adouble)
	{
		if(elem.type==acrob||elem.type==acrof)
		    val+=100;
	}
	else if(state==aback){
		if(elem.type==acrob)
			val+=100;
		else if(elem.type==transition && elem.dirIn!=elem.dirOut) //voglio che cambia dir per poi mettere un elem. acrobatico indietro
			val+=50;
	}

	return val;
}

elem getBest(elem* elems,int nElems,state state,dir lastDir,int bound,int pos){
	int ibest;
	float valbest;
	float tmpVal;
	ibest=-1;
	valbest=-1;
	for(int i=0;i<nElems;i++){
		if(elems[i].dirIn==lastDir &&(tmpVal=getVal(elems[i],state,bound,pos))>valbest)
		{
			ibest=i;
			valbest=tmpVal;
		}
	}
	return elems[ibest];
}

void printProg(FILE *fp, elem** prog){
    float val=0;
    int DP=0;
    for(int y=0;y<PROG_S;y++)
    {
        for(int x=0;x<DIAG_S;x++)
        {
            if(!elemIsVoid(prog[y][x]))
            {
                fprintf(fp,"%s ",prog[y][x].nome);
                val+=prog[y][x].val;
                DP+=prog[y][x].dif;
            }
        }
        fprintf(fp,"\n");
    }

    printf("Valore: %2f DP: %d ",val,DP);
}

int elemIsVoid(elem elem){
    if(elem.dif==-1)
        return 1;
    return 0;
}

elem elemGetVoid(){
    elem elem;
    elem.dif=-1;
    return elem;
}

elem **mallocProg(int rows,int cols){
	elem **prog=(elem**)malloc(rows*sizeof(*prog));
	if(prog==NULL)
	{
		printf("Errore nell'allocazione di memoria");
		exit(1);
	}
	for(int j=0;j<rows;j++){
		prog[j]=malloc(cols*sizeof(**prog));
		if(prog[j]==NULL)
		{
			printf("Errore nell'allocazione di memoria");
			exit(1);
		}		
	}
	return prog;
}

void freeProg(elem** prog,int rows){

    if(prog!=NULL)
        for(int i=0;i<rows;i++){
            free(prog[i]);
        }
	free(prog);
}

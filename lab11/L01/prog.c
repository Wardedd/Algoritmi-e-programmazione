//
// Created by carmine on 16/12/20.
//
#include "prog.h"
#include "diag.h"

static int checkIteration(pprog prog,pdiag newDiag,int DP);
static int checkResult(pprog prog);
static void combRip(pdiagArr diags,pprog bestProg,pprog tmpProg,int start, int k,int DP);

pprog initProg(){
    pprog prog = malloc(sizeof(sprog));
    if(prog==NULL)
        exit(-6);
    prog->DP=0;
    prog->pos=0;
    prog->val=0;
    return prog;
}

int progIsNull(pprog prog){
    if(prog->DP==0)
        return 1;
    return 0;
}

void freeProg(pprog prog){
    free(prog);
}

pprog generateProg(pdiagArr diags,int DP){
    pprog bestProg = initProg();
    pprog tmpProg = initProg();
    combRip(diags,bestProg,tmpProg,0,PROG_S,DP);

    freeProg(tmpProg);
	return bestProg;
}

void progCpy(pprog dest, pprog source){
    for(int i=0;i<PROG_S;i++){
        dest->diagSet[i]=source->diagSet[i];
    }
    
    dest->val=source->val;
    dest->DP=source->DP;
    dest->pos=source->pos; //3
}
/*ordinamento non conta, k=3 | Pos attuale dentro tmpProg
combinazioni ripetute fino a k-1esima scelta
poichè ultima diagonale può avere elemento finale, si devono guardare tutte le diagonali (esco da combinazioni ripetute)
*/
void combRip(pdiagArr diags,pprog bestProg,pprog tmpProg,int start, int k,int DP){
    if(tmpProg->pos==k)
    {
        double tmpVal=0;
        elem* lastElem = tmpProg->diagSet[PROG_S-1]->arr[tmpProg->diagSet[PROG_S-1]->size-1];
        if(lastElem->dif>=8)
            tmpVal=tmpProg->diagSet[PROG_S-1]->val/2;

        tmpProg->val+=tmpVal;

        if(tmpProg->val> bestProg->val && checkResult(tmpProg)){
            progCpy(bestProg,tmpProg);
        }

        tmpProg->val-=tmpVal;
        return;
    }
    
    if(tmpProg->pos==k-1){ //ULTIMA DIAGONALE
        for(int i=0;i<diags->pos;i++){
            if (checkIteration(tmpProg,diags->arr[i],DP)){
            tmpProg->diagSet[tmpProg->pos]=diags->arr[i];
            tmpProg->DP+=diags->arr[i]->DD;
            tmpProg->val+=diags->arr[i]->val;
            tmpProg->pos++;

            combRip(diags,bestProg,tmpProg,i,k,DP);

            tmpProg->pos--;
            tmpProg->DP-=diags->arr[i]->DD;
            tmpProg->val-=diags->arr[i]->val;
            }
        }
    }
    else{
        for(int i=start;i<diags->pos;i++){
            if (checkIteration(tmpProg,diags->arr[i],DP)){
            tmpProg->diagSet[tmpProg->pos]=diags->arr[i];
            tmpProg->DP+=diags->arr[i]->DD;
            tmpProg->val+=diags->arr[i]->val;
            tmpProg->pos++;

            combRip(diags,bestProg,tmpProg,i,k,DP);

            tmpProg->pos--;
            tmpProg->DP-=diags->arr[i]->DD;
            tmpProg->val-=diags->arr[i]->val;
            }
        }
    }
    
}

static int checkIteration(pprog prog,pdiag newDiag,int DP){
    if(prog->DP+newDiag->DD>DP)
        return 0;
    return 1;
}

static int checkResult(pprog prog){
    //DP visto in checkIteration
    int iacrof=0,iacrob=0;
    int acrostreak=0,acrodouble=0;
    type type;
    pdiag diag;
    
    for(int i=0;i<PROG_S;i++){
        diag = prog->diagSet[i];
         acrostreak=0;

        for(int j=0;j<diag->size;j++){
             type=diag->arr[j]->type;

             if(type==acrob) iacrob++;
             else if(type==acrof) iacrof++;

             if(type==acrob||type==acrof)
                 acrostreak++;
             else
                 acrostreak=0;

             if(acrostreak==2)
                 acrodouble++;
        }
    }

    if(iacrob>0&&iacrof>0&&acrodouble>0)
        return 1;
    return 0;
}

void printProg(FILE *fp,pprog prog){
    if(progIsNull(prog)){
        printf("Programma non esistente\n");
    }
    else{
        fprintf(fp,"Val: %lf DP: %d\n",prog->val,prog->DP);
        for(int i=0;i<PROG_S;i++)
        {
            fprintf(fp,"diag#%d val:%lf",i+1,prog->diagSet[i]->val);
            for(int j=0;j<prog->diagSet[i]->size;j++)
                fprintf(fp,"%s ",prog->diagSet[i]->arr[j]->nome);
            fprintf(fp,"\n");
        }
    }
}

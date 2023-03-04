//
#include "st.h"

typedef struct symboltable{ //ordine ascendente
 Vert *list;
 int num,currnum;
}*ST;

ST STnew(){
    return malloc(sizeof(struct symboltable));
}

void STinit(ST st, int num){

	st->list = malloc(num*sizeof(Vert));
	st->currnum=0;
	st->num=num;
}

void STfree(ST st){
	free(st->list);
	free(st);
}

Vert* STgetByIndex(ST st, int id){
    return &st->list[id];
}

void STinsert(ST st, Vert v){
	if(st->currnum>=st->num)
		return;	

	VERTset(&st->list[st->currnum],v.netId,v.name);
	st->currnum++;
}
int STsearchByName(ST st, char* name){
	for(int i=0;i<st->currnum;i++){

	    if(strcmp(VERTgetName(&(st->list[i])),name)==0)
	    {
		    return i;
	    }
	}
	return -1;
}

int *STorderByName(ST st){ //ascendente
	int min;
	int tmp;
	int* index = malloc(st->currnum*sizeof(int));
	for(int i=0;i<st->currnum;i++)
	    index[i]=i;

	for(int i=0;i<st->currnum-1;i++){
		min=i;
		for(int j=i+1;j<st->currnum;j++){
			if(VERTcmp(st->list[index[min]],st->list[index[j]])>0)
				min=j;
		}
		tmp=index[i];
        index[i]=index[min];
        index[min]=tmp;
	}

	return index;
}

int STcount(ST st){
    return st->currnum;
}

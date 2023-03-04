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

Vert* STselect(ST st, int i){
    return &st->list[i];
}

void STinsert(ST st, Vert v){
	if(st->currnum>=st->num)
		return;	

	VERTset(&st->list[st->currnum],v.name);
	st->currnum++;
}
int STsearch(ST st, char* name){
	for(int i=0;i<st->currnum;i++){

	    if(strcmp(VERTgetName(&(st->list[i])),name)==0)
		    return i;
	}
	return -1;
}

Vert STsearchByName(ST st, char *name){

	for(int i=0;i<st->currnum;i++){

	    if(strcmp(VERTgetName(&(st->list[i])),name)==0)
	    {
		    return st->list[i];
            }
	}
	return VERTgetNull();
}

void STorderByName(ST st){ //ascendente
	int max=0;
	Vert tmp;	
	for(int i=0;i<st->currnum-1;i++){
		max=i;
		for(int j=i+1;j<st->currnum;j++){
			if(VERTcmp(st->list[max],st->list[j])>0)
				max=j;
		}
		tmp = st->list[i];
		st->list[i]=st->list[max];
		st->list[max]=tmp;
	}
}

void STcopy(ST dst,ST src){
    if(dst==NULL)
    {
        printf("ST nuova non allocata");
        exit(4);
    }
    if(dst->list!=NULL)
        free(dst->list);
    STinit(dst,src->num);
    dst->currnum=src->currnum;
    for(int i=0;i<dst->currnum;i++){
        dst->list[i]=src->list[i];
    }
}

int STcount(ST st){
    return st->currnum;
}


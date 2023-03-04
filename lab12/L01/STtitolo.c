#include "STtitolo.h"

typedef struct node *link;

/*STtitle*/
struct STtitolo{
    link head;
    int currnum;
};

struct node{
    Title title;
    link next;
};

STtitle STinit(){
	STtitle list = malloc(sizeof(*list));
	list->currnum=0;
	list->head=NULL;
	return list;
}

link NEWnode(Title title,link next){
    link x = malloc(sizeof(*x));
    x->title=title;
    x->next=next;
    return x;
}

void FREEnode(link h){
    if(h==NULL)
        return;
    FREEnode(h->next);
    TITLEfree(h->title);
    free(h);
}

void STfree(STtitle list){
	FREEnode(list->head);
	free(list);
}

void STinsert(STtitle list,Title title){ //0 se non viene inserita(esiste), 1 se viene inserita

    link h = list->head;
	link p;
	if(h==NULL)
    {
	    list->head=NEWnode(title,NULL);
        return;
    }

	while(h!=NULL && T_KEYcmp(T_KEYget(title),T_KEYget(h->title))<0){
		p=h;
		h=h->next;
	}
	if(p==NULL||T_KEYcmp(T_KEYget(title),T_KEYget(h->title))>0) //head + piccolo della nuova
    {
	    h->next=NEWnode(title,h->next);
	    return;
    }
	p->next=NEWnode(title,h);
}

Title STsearch(STtitle list, char *name){
	link h = list->head;
	while(h!=NULL){
		if(T_KEYcmp(name,T_KEYget(h->title))==0)
			return h->title;
		h=h->next;
	}
	
	return TITLEgetNull();
}
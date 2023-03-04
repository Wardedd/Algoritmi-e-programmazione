#include "pgList.h"

typedef struct node{
    pg_t pg;
    struct node *next;
}*n_link; //node link

struct pgList_s{
    n_link head;
    n_link tail;
};

void node_free(n_link node){
    pg_clean(&(node->pg));
    free(node);
}

n_link node_init(n_link next,pg_t pg){
    n_link newNode=(n_link)malloc(sizeof(struct node));
    newNode->next = next;
    newNode->pg = pg;
    return newNode;
}

/* creatore e distruttore */
pgList_t pgList_init(){
    pgList_t pgList = malloc(sizeof(struct pgList_s));
    pgList->head=NULL;
    pgList->tail=NULL;
    return pgList;
}

void pgList_free(pgList_t pgList){
    n_link x,p;
    x=pgList->head;
    p=x;
    for(;x!=NULL;p=x){
        x=x->next;
        node_free(p);
    }
    free(pgList);
}

/* lettura e scrittura su file */
void pgList_read(FILE *fp, pgList_t pgList){
    if(pgList==NULL){
        fprintf(fp,"Lista dei personaggi non inizializzata\n");
        return;
    }

    pg_t tmpPg;
    int fileLines = getFileLines(fp,"%s %s %s %d %d %d %d %d %d");

    for(int i=0;i<fileLines;i++){
        pg_read(fp,&tmpPg);
        pgList_insert(pgList,tmpPg);
    }
}
void pgList_print(FILE *fp, pgList_t pgList, int extensive,invArray_t invArray){ //mod
    if(pgList==NULL||pgList->head==NULL){
        fprintf(fp,"Lista dei personaggi non inizializzata\n");
        return;
    }

    for(n_link x=pgList->head;x!=NULL;x=x->next){
        pg_print(fp,&x->pg,extensive,invArray);
        fprintf(fp,"\n");
    }
}

/* inserimento di un nuovo personaggio */
void pgList_insert(pgList_t pgList, pg_t pg){
    n_link node = node_init(NULL,pg);
    if(pgList->head==NULL){
        pgList->head=node;
        pgList->tail=node;
    }
    else{
        pgList->tail->next=node;
        pgList->tail=node;
    }
}

/* cancellazione con rimozione */
void pgList_remove(pgList_t pgList, char* cod){
    n_link x,p;
    x = pgList->head;
    p=NULL;
    for(;x!=NULL;p=x,x=x->next)
        if(strcmp(x->pg.cod,cod)==0)
        {
            if(pgList->head==x)
                pgList->head = pgList->head->next;
            else
                p->next=x->next;
            if(pgList->tail==x)
            {
                pgList->tail=p;
            }
            free(x);
        }
}

/* ricerca per codice, ritornando il puntatore */
link_pg pgList_searchByCode(pgList_t pgList, char* cod){
    n_link x = pgList->head;
    for(;x!=NULL;x=x->next)
        if(strcmp(x->pg.cod,cod)==0)
        {
            return &x->pg;
        }
    return NULL;
}

/*aggiunte*/
int pgList_isEmpty(pgList_t pgList){
    if(pgList->head==NULL)
        return 1;
    return 0;
}
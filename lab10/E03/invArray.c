#include "invArray.h"

/* ADT di prima classe collezione di oggetti di inventario */
struct invArray_s{
    inv_t *vettInv;
    int nInv;
};

/* creatore e disruttore */

invArray_t invArray_init()
{
    invArray_t inv = (invArray_t)malloc(sizeof(struct invArray_s));
    if(inv==NULL)
        exit(1);

    inv->nInv=0;
    inv->vettInv=NULL;
    return inv;
}

void invArray_free(invArray_t invArray)
{
    free(invArray->vettInv);
    free(invArray);
}

int invIndexSafe(invArray_t invArray, int index){
    if(index>=0&&index<invArray->nInv)
        return 1;
    return 0;
}

/* lettura e scrittura su file */
void invArray_read(FILE *fp, invArray_t invArray){
    readNum(fp, &invArray->nInv);
    invArray->vettInv=malloc(invArray->nInv*sizeof(inv_t));

    for(int i=0;i<invArray->nInv;i++){
        inv_read(fp,&invArray->vettInv[i]);
    }
}

void invArray_print(FILE *fp, invArray_t invArray){
    for(int i=0;i<invArray->nInv;i++){
        fprintf(fp,"%d)",i);
        inv_print(fp,&invArray->vettInv[i]);
        fprintf(fp,"\n");
    }
}

/* stampa un unico oggetto selezionato da indice (nel vettore) */
void invArray_printByIndex(FILE *fp, invArray_t invArray, int index){
    inv_print(fp,&invArray->vettInv[index]);
}

/* ritorna puntatore a oggetto selezionato da indice (nel vettore) */

inv_t *invArray_getByIndex(invArray_t invArray, int index){
    if(index<0||index>=invArray->nInv)
        return NULL;
    return &invArray->vettInv[index];
}
/* ritorna indice (nel vettore) a oggetto selezionato da nome */
int invArray_searchByName(invArray_t invArray,char *name){
    for (int i=0;i<invArray->nInv;i++) {
        if(strcmp(name,invArray->vettInv[i].nome)==0){
            return i;
        }
    }
    return -1;
}

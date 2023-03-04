#include "equipArray.h"

struct equipArray_s{
    int arr[EQUIP_SLOT];
    int inUso;
};

/* creatore e disruttore */
equipArray_t equipArray_init(){
    equipArray_t equipArray= malloc(sizeof(struct equipArray_s));
    for(int i=0;i<EQUIP_SLOT;i++){
        equipArray->arr[i]=-1;
    }
    equipArray->inUso=0;
    return equipArray;
}

void equipArray_free(equipArray_t equipArray){
    free(equipArray);
}

/* quanti equipaggiamenti sono in uso */
int equipArray_inUse(equipArray_t equipArray){
    return equipArray->inUso;
}

/* scrittura su file */
void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray){
    for(int i=0;i<EQUIP_SLOT;i++){
        if(equipArray->arr[i]!=-1)
        {
//            printf("%d)",i+1);
            invArray_printByIndex(fp,invArray,equipArray->arr[i]);
-           printf("\n");
        }
    }
}

/* modifica equipaggiamento scegliendo un oggetto da inventario */
void equipArray_update(equipArray_t equipArray, invArray_t invArray,int index,int toRemove){
    //toRemove = 1 -> index indica l'indice in equip da rimuovere
    //toRemove = 0 -> index indica l'indice in invarr da inserire

    if(toRemove){
        if(equipArray->inUso==0)
        {
            printf("Personaggio già equip vuoto");
            return;
        }
        equipArray->inUso--;
        equipArray->arr[index]=-1;
    }
    else{
        if(equipArray->inUso==EQUIP_SLOT)
        {
            printf("Personaggio ha equip. pieno");
            return;
        }
        for(int i=0;i<EQUIP_SLOT;i++){
            if(equipArray->arr[i]==-1)
            {
                equipArray->inUso++;
                equipArray->arr[i]=index;
                break;
            }
        }
    }
}
/* torna indice (nel vettore inventario) dell'oggetto in posizione index (0..EQUIP_SLOT-1) di equipArray */
int equipArray_getEquipByIndex(equipArray_t equipArray, int index){
    if(index<0||index>=equipArray->inUso)
        return -1;
    return equipArray->arr[index];
}

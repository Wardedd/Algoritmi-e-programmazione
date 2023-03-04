#include <limits.h>
#include "pg.h"

/* lettura e scrittura su file */
int pg_read(FILE *fp, link_pg pg){
    if(fp==NULL)
        exit(5);

    if(fscanf(fp,"%s %s %s",pg->cod,pg->nome,pg->classe)!=3)
    {
        printf("Errore nella letture del personaggio");
        exit(1);
    }
    stat_read(fp,&pg->b_stat);
    pg->eq_stat=pg->b_stat;
    pg->equip=equipArray_init();
}

/* non essendo struct dinamica, pulisce chiamando il distruttire di equipArray */
void pg_clean(link_pg pg){
    equipArray_free(pg->equip);
}

void pg_print(FILE *fp, link_pg pg,int extensive, invArray_t invArray){ //extensive=1 => printa anche inventario
        if(fp==NULL)
            exit(5);

        int eInUse = equipArray_inUse(pg->equip);

        fprintf(fp,"%s) %s %s equip:%d\n",pg->cod,pg->nome,pg->classe,eInUse);
        if(eInUse>0) {
            fprintf(fp, "Stat personaggio: ");
            stat_print(fp, &pg->eq_stat, 1);
            fprintf(fp,"\n");
        }

        fprintf(fp,"Stat base: ");
        stat_print(fp,&pg->b_stat,INT_MIN);
        fprintf(fp,"\n");

        if(equipArray_inUse(pg->equip)>0 && extensive) {
            fprintf(fp,"Inventario:\n");
            equipArray_print(fp,pg->equip,invArray);
        }
}

/* modifica personaggio aggiungendo/togliendo un equipaggiamento selezionato da inventario:
di fatto e' sufficiente chiamare l'opportuna funzione dal modulo equipArray */
void pg_updateEquip(link_pg pg, invArray_t invArray, int index,int toRemove){
    inv_t* tmpInv;

    if(toRemove)
        index=equipArray_getEquipByIndex(pg->equip,index);
        if(index==-1)
        {
            printf("Oggetto non trovato");
            return;
        }

    tmpInv = invArray_getByIndex(invArray, index);
    if (tmpInv != NULL){
        if(toRemove)
            pg->eq_stat = stat_diff(pg->eq_stat, inv_getStat(tmpInv));
        else
            pg->eq_stat = stat_sum(pg->eq_stat, inv_getStat(tmpInv));

        equipArray_update(pg->equip,invArray,index,toRemove);
    }
    else{
        printf("Oggetto non trovato");
    }
}
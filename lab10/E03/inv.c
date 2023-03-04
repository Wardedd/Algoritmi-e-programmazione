#include <limits.h>
#include "inv.h"

/* funzioni di input/output delle statistiche */
void stat_print(FILE *fp, stat_t *statp, int soglia){
    stat_t totstat = *statp;
    totstat.spr = statp->spr<soglia? MIN_STAT:statp->spr;
    totstat.mag = statp->mag<soglia? MIN_STAT:statp->mag;
    totstat.atk = statp->atk<soglia? MIN_STAT:statp->atk;
    totstat.mag = statp->mag<soglia? MIN_STAT:statp->mag;
    totstat.def = statp->def<soglia? MIN_STAT:statp->def;
    totstat.mp = statp->mp<soglia? MIN_STAT:statp->mp;
    fprintf(fp,"%d %d %d %d %d %d",totstat.hp,totstat.mp,totstat.atk,totstat.def,totstat.mag,totstat.spr);
}

void stat_read(FILE *fp,stat_t *stat){
    if(fscanf(fp,"%d %d %d %d %d %d",&stat->hp,&stat->mp,&stat->atk,&stat->def,&stat->mag,&stat->spr)!=6){
        printf("Problemi nella lettura delle statistiche");
        exit(1);
    }
}

/* funzioni di input/output di un oggetto dell'inventario */
void inv_read(FILE *fp, inv_t *invp){
    fscanf(fp,"%s %s",invp->nome,invp->tipo);
    stat_read(fp,&invp->stat);
}

void inv_print(FILE *fp, inv_t *invp){
    fprintf(fp,"%s %s ",invp->nome,invp->tipo);
        stat_print(fp,&invp->stat,INT_MIN);
}

/* ritorna il campo stat di un oggetto dell'inventario */
stat_t inv_getStat(inv_t *invp){
    return invp->stat;
}

/*aggiunte*/
stat_t stat_sum(stat_t a,stat_t b){
    stat_t new;
    new.mp=a.mp+b.mp;
    new.def=a.def+b.def;
    new.mag=a.mag+b.mag;
    new.spr=a.spr+b.spr;
    new.hp=a.hp+b.hp;
    new.atk=a.atk+b.atk;
    return new;
}

stat_t stat_diff(stat_t a,stat_t b){
    stat_t new;
    new.mp=a.mp-b.mp;
    new.def=a.def-b.def;
    new.mag=a.mag-b.mag;
    new.spr=a.spr-b.spr;
    new.hp=a.hp-b.hp;
    new.atk=a.atk-b.atk;
    return new;
}
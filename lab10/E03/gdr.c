#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pgList.h"
#include "invArray.h"
#include "pg.h"

#define N_SCELTE 7
#define DBG 0

enum { falso, vero };
typedef int bool;

void updateEquip(pg_t* pgp,invArray_t invArray); //mod

void stampaMenu(char *scelte[], int *selezione){
  int i=0;
  printf("\nMENU'\n");
  for(i=0;i<N_SCELTE;i++)
    printf("%2d > %s\n",i,scelte[i]);
  scanf(" %d",selezione);
}

int main(int argc, char **argv) {
  char *scelte[] = {
    "Uscita",
    "Stampa personaggi",
    "Stampa inventario",
    "Cerca personaggio",
    "Aggiungi personaggio",
    "Elimina personaggio",
    "Modifica equip"
  };

  char codiceRicerca[LEN];
  int selezione;
  FILE *fin;
  bool fineProgramma;

  pgList_t pgList = pgList_init();
  invArray_t invArray = invArray_init();
  pg_t *pgp, pg;

  fin = fopen("pg.txt","r");
  pgList_read(fin, pgList);
  fclose(fin);
#if DBG
  pgList_print(stdout, pgList);
#endif /* DBG */

  fin = fopen("inventario.txt","r");
  invArray_read(fin, invArray);
  fclose(fin);
#if DBG
  invArray_print(stdout, invArray);
#endif /* DBG */

  fineProgramma = falso;

  do {
    stampaMenu(scelte, &selezione);
    switch(selezione){

    case 0: {
      fineProgramma = vero;
    } break;

    case 1: {
      pgList_print(stdout, pgList,0,invArray);
    } break;

    case 2: {
      invArray_print(stdout, invArray);
    } break;

    case 3: {
      printf("Inserire codice personaggio: ");
      scanf("%s", codiceRicerca);
      pgp = pgList_searchByCode(pgList, codiceRicerca);
      if (pgp!=NULL) {
        pg_print(stdout, pgp, 1,invArray); //modificato
      }else{
          printf("Nessun personaggio trovato\n");
      }
    } break;

    case 4: {
      printf("Cod Nome Classe HP MP ATK DEF MAG SPR: ");
      if (pg_read(stdin, &pg) != 0) {
        pgList_insert(pgList, pg);
      }
    } break;

    case 5: {
      printf("Inserire codice personaggio: ");
      scanf("%s", codiceRicerca);
      pgList_remove(pgList, codiceRicerca);
    } break;

    case 6: {
      printf("Inserire codice personaggio: ");
      scanf("%s", codiceRicerca);
      pgp = pgList_searchByCode(pgList, codiceRicerca);
      if (pgp!=NULL) {
        updateEquip(pgp,invArray);
      }else{
          printf("Nessun personaggio trovato\n");
      }
    } break;

    default:{
      printf("Scelta non valida\n");
    } break;
    }
  } while(!fineProgramma);

  pgList_free(pgList);

  return 0;
}

void updateEquip(pg_t* pgp,invArray_t invArray){
    int toRemove,index;
    printf("Scegli:\n0)Aggiungi in inventario\n1)Rimuovi da inventario\n");
    readNum(stdin, &toRemove);

    if(toRemove) {
        if (equipArray_inUse(pgp->equip) == 0) {
            printf("Personaggio con equip vuoto");
            return;
        }
        equipArray_print(stdout,pgp->equip,invArray);
        readNum(stdin,&index);

        pg_updateEquip(pgp,invArray,index,1);
    }
    else{
        if(equipArray_inUse(pgp->equip)==EQUIP_SLOT)
        {
            printf("Personaggio ha equip. pieno");
            return;
        }
        invArray_print(stdout,invArray);
        readNum(stdin,&index);

        pg_updateEquip(pgp,invArray,index,0);
    }
}

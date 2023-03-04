#ifndef QUOT_H
#define QUOT_H

#include <stdio.h>
#include <stdlib.h>
#include "time.h"

typedef struct{
 float val;
 int num;
 Time time;
}Quot;

void QUOTinit(Quot *quot,Time time);

void QUOTupdate(FILE *fp,Quot *quot); //Funziona come read, però aggiunge le trasazioni nuove alla quot. esistente
void QUOTprint(FILE *fp, Quot quot);

Quot QUOTgetNull();
int QUOTisNull(Quot quot);

Time Q_KEYget(Quot quot);
int Q_KEYcmp(Time time1,Time time2);
float QUOTvalCmp(float key1,float key2);
float QUOTgetVal(Quot quot);
#endif

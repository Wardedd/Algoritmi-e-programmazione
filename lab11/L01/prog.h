//
// Created by carmine on 16/12/20.
//

#ifndef PROJECT_PROG_H
#define PROJECT_PROG_H

#include "diag.h"

#define PROG_S 3

typedef struct{
    pdiag diagSet[PROG_S];
    int DP;
    int pos;
    double val;
}*pprog,sprog;

pprog generateProg(pdiagArr diags,int DP);
void printProg(FILE *fp,pprog prog);
int progIsNull(pprog prog);

#endif //PROJECT_PROG_H

#ifndef PROJECT_SUPPORT_H
#define PROJECT_SUPPORT_H

#include <stdio.h>
#include <stdlib.h>

void openFile(FILE **fp,char *fileName,char *modes);
int getFileLines(FILE *fp,char *pattern); //tutte le righe nel file, incluse quelle vuote
void readNum(FILE *fp, int *p_num);

#endif
#ifndef PROJECT_SUPPORT_H
#define PROJECT_SUPPORT_H

#include <stdio.h>
#include <stdlib.h>

void openFile(FILE **fp,char *fileName,char *modes);
int getFileLines(FILE *fp);
void readIndex(FILE *fp,int *p_num);

void malloc2Dp(int*** mat, int nr,int nc);
void free2Dp(int** mat, int nr);

#endif

#ifndef TITOLO_H
#define TITOLO_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "BSTquot.h"

#define T_NAME_SIZE 20

typedef struct title *Title;

int T_KEYcmp(char* key1,char *key2);
char* T_KEYget(Title title);
BST TITLEgetBST(Title title);

Title TITLEinit(char *name);
void TITLEfree(Title title);

int TITLEisNull(Title title);
Title TITLEgetNull();
void TITLEprint(FILE *fp,Title title);

#endif

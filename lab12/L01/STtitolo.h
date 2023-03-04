#ifndef STTITOLO_H
#define STTITOLO_H

#include <stdio.h>
#include "titolo.h"

#define T_NAME_SIZE 20

typedef struct STtitolo *STtitle;

STtitle STinit();
void STfree(STtitle list);
Title STsearch(STtitle list, char *name);
void STinsert(STtitle list,Title title);
//STdelete
#endif

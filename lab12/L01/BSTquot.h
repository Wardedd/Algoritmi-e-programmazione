#ifndef BSTQUOT_H
#define BSTQUOT_H

#include <stdio.h>
#include "quot.h"
#include "time.h"

typedef struct BSTquot *BST;

BST BSTinit(); //mod ord 
void BSTfree(BST bst); 
int BSTcount(BST bst);
Quot BSTmin(BST bst); 
Quot BSTmax(BST bst);

Quot BSTsearch(BST bst,Time time); 
void BSTinsert(BST bst,Quot x);

void BSTupdatequot(FILE *fp,BST bst,Time time);

float BSTsearchLocalMax(BST bst,Time l,Time r);
float BSTsearchAbsoluteMax(BST bst);

//static link rotR(link h);
//static link rotL(link h);
//static link partR(link h, int r);
void BSTbalance(BST bst);

#endif

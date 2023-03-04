#include "BSTquot.h"

typedef struct BSTquot *BST;
typedef struct quotNode *link;

struct BSTquot{
 link root;
 link z;
};

struct quotNode{
 Quot item; //Item
 link l,r;
 link p;
 int N;
};

static link NEW(Quot item, link p, link l, link r, int N){
	link x = malloc(sizeof *x);
	x->item = item;
	x->p = p;
	x->l = l;
	x->r = r;
	x->N = N;
	return x;
}

BST BSTinit(){
    BST bst = malloc(sizeof(*bst));
    bst->root=(bst->z=(NEW(QUOTgetNull(),NULL,NULL,NULL,0)));
    return bst;
}


void freeNode(link h,link z){
    if(h==z)
           return;
    freeNode(h->r,z);
    freeNode(h->l,z);

    free(h);
}

void BSTfree(BST bst){
    freeNode(bst->root,bst->z);
    free(bst->z);
    free(bst);
}

/*         BSTcount         */
int countR(link h,link z){
    if(h==z)
        return 0;
    return 1+countR(h->l,z)+countR(h->r,z);
}

int BSTcount(BST bst){
    return countR(bst->root,bst->z);
}
/*         BSTmin ricorsivo e max iterativo        */
Quot minR(link h, link z) {
    if (h == z)
        return QUOTgetNull();
    if (h->l == z)
        return (h->item);
    return minR(h->l, z);
}

Quot BSTmin(BST bst) {
    return minR(bst->root, bst->z);
}

Quot BSTmax(BST bst){
    link h=bst->root;
    while(h->r!=bst->z){
        h=h->r;
    }
    return h->item;
}

/*         BSTsearch         */
Quot searchR(link h,Time k,link z){
    int cmp;
    if(h==z)
        return QUOTgetNull();
    cmp = Q_KEYcmp(k,Q_KEYget(h->item));
    if(cmp==0)
        return h->item;
    if(cmp<0)
        return searchR(h->l,k,z);
    return searchR(h->r,k,z);
}

Quot BSTsearch(BST bst,Time k){
    return searchR(bst->root,k,bst->z);
}

/*         BSTinsert_leaf(R: ricorsivo, I:iterativo)         */
static link insertR(link h,Quot x,link z)
{
    if (h==z)
        return NEW(x, z, z, z, 1);
    if (Q_KEYcmp(Q_KEYget(x), Q_KEYget(h->item))==-1){
        h->l=insertR(h->l, x, z);
        h->l->p=h;
    }
    else{
        h->r=insertR(h->r, x, z);
        h->r->p=h;
    }
    (h->N)++;
    return h;
}

void BSTinsert(BST bst,Quot x){
    bst->root=insertR(bst->root,x,bst->z);
}

static link rotR(link h){
    link x = h->l;
    h->l=x->r;
    x->l->p=h;
    x->r=h;
    x->p=h->p;
    x->p=x;

    x->N = h->N;
    h->N = 1;
    h->N += (h->l) ? h->l->N : 0;
    h->N += (h->r) ? h->r->N : 0;
    return x;
}
static link rotL(link h){
    link x = h->r;
    h->r=x->l;
    x->l->p=h;
    x->l=h;
    x->p=h->p;
    h->p=x;
    x->N=h->N;
    h->N=1;
    h->N+=(h->l)?h->l->N:0;
    h->N+=(h->r)?h->r->N:0;
    return x;
}

link partR(link h, int r) {
    int t = h->l->N;
    if (t > r) {
        h->l = partR(h->l, r);
        h = rotR(h);
    }
    if (t < r) {
        h->r = partR(h->r, r-t-1);
        h = rotL(h);
    }
    return h;
}

static link balanceR(link h, link z) {
    if (h == z)
        return z;

    int r = (h->N+1)/2-1;
    h = partR(h, r);
    h->l = balanceR(h->l, z);
    h->r = balanceR(h->r, z);
    return h;
}

void BSTbalance(BST bst) {
    bst->root = balanceR(bst->root, bst->z);
}

/*AGGIUNTE
 * Non volevo usare/modificare le funzioni originali, e non ho potuto
 * chiamare una search poichè essa doveva ritornare il tipo Quot* al posto di Quot
 */

void BSTuqR(FILE *fp,link h, Time time,link z){
    int cmp;
    if(h==z)
        return;
    cmp = Q_KEYcmp(time,Q_KEYget(h->item));
    if(cmp==0)
    {
        QUOTupdate(fp,&h->item);
        return;
    }
    if(cmp<0)
         BSTuqR(fp,h->l,time,z);
     BSTuqR(fp,h->r,time,z);
}

void BSTupdatequot(FILE *fp,BST bst,Time time){
    BSTuqR(fp,bst->root,time,bst->z);
}

//ricerca tra 2 date
static float sMaxR(link h,Time l, Time r,link z);
static int isValid(Time x, Time l, Time r);

float BSTsearchLocalMax(BST bst,Time l,Time r){ //vista in order = subleft root subright
    return sMaxR(bst->root,l,r,bst->z);
}

float BSTsearchAbsoluteMax(BST bst){ //vista in order = subleft root subright
    Time l,r;
    l=Q_KEYget(BSTmin(bst));
    r=Q_KEYget(BSTmax(bst));
    return BSTsearchLocalMax(bst,l,r);
}

static int isValid(Time x, Time l, Time r){
    int time=TIMEget(x);
    if(time<TIMEget(l)||time>TIMEget(r))
        return 0;
    return 1;
}
static float sMaxR(link h,Time l, Time r,link z){ //searchMaxR
    float max,tmp;
    max=-1;

    if(h==z)
        return -1;

    tmp=sMaxR(h->l,l,r,z);
    if(tmp>max) max=tmp;

    if((tmp=QUOTgetVal(h->item))>max && isValid(Q_KEYget(h->item),l,r)) max=tmp;

    tmp=sMaxR(h->r,l,r,z);
    if(tmp>max) max=tmp;

    return max;
}


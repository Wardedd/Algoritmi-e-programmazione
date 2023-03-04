#include "titolo.h"
typedef struct title *Title;

struct title{
	char name[T_NAME_SIZE];
	BST bst;
};

int T_KEYcmp(char* key1,char *key2){
	return strcmp(key1,key2);
}

char* T_KEYget(Title title){
	return title->name;
}

Title TITLEinit(char *name){
	Title p = malloc(sizeof(*p));
	strcpy(p->name,name);
	p->bst = BSTinit();
	return p;
}

void TITLEfree(Title title){
    BSTfree(title->bst);
	free(title);
}

BST TITLEgetBST(Title title){
    return title->bst;
}
Title TITLEgetNull(){
    return NULL;
}

void TITLEprint(FILE *fp,Title title){
	fprintf(fp,"Name: %s",title->name);
}

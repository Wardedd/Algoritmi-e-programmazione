#include <stdlib.h>
#include "vertex.h"

int VERTgetNetId(Vert *v){
	return v->netId;
}

char* VERTgetName(Vert *v){
	return v->name;
}

int VERTisNull(Vert v){
	if(v.netId==-1)
		return 1;
	return 0;
}

Vert VERTgetNull(){
	Vert v={netId:-1};
	return v;
}

int VERTcmp(Vert v1,Vert v2){
	return strcmp(v1.name,v2.name);
}

void VERTset(Vert *v,int netId,char *name){
    v->netId=netId;
    strcpy(v->name,name);
}

void VERTprint(FILE *fp,Vert v){
	fprintf(fp,"nome: %s netId:%d",v.name,v.netId);
}

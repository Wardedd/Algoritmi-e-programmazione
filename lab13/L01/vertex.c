#include "vertex.h"

char* VERTgetName(Vert* v){
	return v->name;
}

int VERTisNull(Vert v){
	if(!strcmp(v.name,""))
		return 1;
	return 0;
}

Vert VERTgetNull(){
	Vert v={v.name==""};
	return v;
}

int VERTcmp(Vert v1,Vert v2){
    if(v1.name==NULL||v2.name==NULL)
        return 0;

	return strcmp(v1.name,v2.name);
}

void VERTset(Vert *v,char *name){
    strcpy(v->name,name);
}

void VERTread(FILE *fp,Vert* v){
    if(fscanf(fp,"%s ",v->name)!=1)
    {
        printf("Problema nella lettura dei vertici");
        exit(1);
    }

}
void VERTprint(FILE *fp,Vert v){
	fprintf(fp,"nome: %s",v.name);
}

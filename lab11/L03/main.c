#include <stdio.h>
#include <stdlib.h>

#include "support.h"
#include "vertex.h"
#include "graph.h"

#define FILE_NAME_SIZE 30
#define MENU_SIZE 4
#define STR_QUERY 30

typedef enum{
printOrder,printListAdj,checkComplSub,fine
} menu;

const char *str_menu[]={"printOrder","printListAdj","checkComplSub","fine"};
const char *str_menu_desc[]={"Elenca ordine alfabetico i vertici e archi","Verifica che 3 vertici del grafo formano un sottografo completo (no cappi)","Stampa la lista di adiacenza","fine"};

int toMenu(char* queryStr){
	for(int i=0;i<MENU_SIZE;i++)
		if(strcmp(queryStr,str_menu[i])==0)
			return i;
}

void printMenu(){
	printf("Selezione l'azione scrivendo quello che è scritto nelle (parentesi):");
	for(int i=0;i<MENU_SIZE;i++){
		printf("\n%s | (%s)", str_menu_desc[i],str_menu[i]);
	}
}

int main(int argc, char **argv){
	if(argc<2 || argv[1]==NULL){
		printf("Inserire il nome del file da linea di comando");
		exit(3);
	}

	menu query;
	char fileName[FILE_NAME_SIZE];
	char strQuery[STR_QUERY];
	Graph graph;
    FILE *fp;
    	strcpy(fileName,argv[1]);
	
    openFile(&fp,fileName,"r");

	graph=GRAPHload(fp);

	printf("Grafo non ordinato pesato\n");
		
	do{
		printMenu();
		printf("\n");
		scanf("%s",strQuery);
		query = toMenu(strQuery);
		
		switch(query){
			case printOrder:
				GRAPHprintOrder(stdout,graph);
			break;
			
			case printListAdj:
				GRAPHprintListAdj(stdout,graph);
			break;
			
			case checkComplSub:
			{
				char a[V_NAME_SIZE],b[V_NAME_SIZE],c[V_NAME_SIZE];
				printf("Inserisci 3 nomi di 3 vertici differenti: \n");
				scanf("%s %s %s",a,b,c);
				if(GRAPHSubThree(graph,a,b,c))
					printf("I vertici formano un sottografo completo");
				else{
					printf("I vertici NON formano un sottografo completo");
				}
			}
			break;
		}		
		printf("\n\n");
		
	}while(query!=fine);
	
	GRAPHfree(graph);
	return 0;
}


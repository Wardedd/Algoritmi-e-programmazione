#include <stdio.h>
#include <stdlib.h>

#include "support.h"
#include "STtitolo.h"
#include "BSTquot.h"
#include "titolo.h"
#include "time.h"
#include "support.h"

#define MENU_SIZE 7
#define STR_QUERY_SIZE 20
#define FILE_NAME_SIZE 20

typedef enum{
leggiFile,cercaT,cercaQ,cercaQMaxLocale,cercaQMax,bilanciaTBST,fine
} menu;

const char *str_menu[]={"leggiFile","cercaT","cercaQ","cercaQMaxLocale","cercaQMax","bilanciaTBST","fine"};
const char *str_menu_desc[]={
"Leggi file",
"Cerca titolo",
"Cerca quotazione in data",
"Cerca quotazione max di un titolo in un periodo",
"Cerca quotazione max di un titolo",
"Bilancia il BST del titolo",
"Fine"};

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

void fileRead(STtitle list, FILE *fp);

int main(){

	menu query;
	char strQuery[STR_QUERY_SIZE];
	STtitle titleList;
    FILE *fp;

	titleList = STinit();
		
	do{
		printMenu();
		printf("\n");
		scanf("%s",strQuery);
		query = toMenu(strQuery);
		
		switch(query){
			case leggiFile:
			{
			char strName[FILE_NAME_SIZE];
			printf("Inserisci il nome del file:\n");
			scanf("%s",strName);
			openFile(&fp,strName,"r");

            fileRead(titleList, fp);
			}
			break;
			case cercaT:
			{
				char str[T_NAME_SIZE];
                Title title;

                printf("Inserisci il nome del titolo:\n");
				scanf("%s",str);
		        title = STsearch(titleList,str);

                if(title!=NULL)
                    TITLEprint(stdout,title);
                else
                    printf("\nTitolo non esistente");
			}
			break;
			case cercaQ:
			{
				BST bst;
                Title title;
				Time time;
                char str[T_NAME_SIZE];
                printf("Inserisci il nome del titolo:\n");
				scanf("%s",str);
                title = STsearch(titleList,str);
                if(title!=NULL)
                {
                    printf("Inserisci il la data e l'ora del titolo:\n"
                           "Formato: aaaa/mm/gg\n");
                    TIMEreadsimple(stdin, &time);

                    bst=TITLEgetBST(title);
                    printf("Quot: ");
                    QUOTprint(stdout, BSTsearch(bst, time));
                }
                else
                    printf("\nTitolo non esistente");
			}
			break;
			case cercaQMaxLocale:
			{
                char str[T_NAME_SIZE];
                Time timel,timer;
                Title title;
                float val;

                printf("Inserisci il nome del titolo:\n");
                scanf("%s",str);
                printf("Inserisci il la data e l'ora iniziale:\n"
                       "Formato: aaaa/mm/gg\n");
                TIMEreadsimple(stdin,&timel);
                printf("Inserisci il la data e l'ora finale:\n"
                       "Formato: aaaa/mm/gg\n");
                TIMEreadsimple(stdin,&timer);

                title=STsearch(titleList,str);
                if(title!=NULL){

                    val=BSTsearchLocalMax(TITLEgetBST(title), timel, timer);
                    TITLEprint(stdout, title);
                    printf("\n Valore massimo locale è : %f", val);
                }
                else{
                    printf("\nTitolo non esistente");
                }
            }
			break;
			case cercaQMax:
            {
                char str[T_NAME_SIZE];
                Title title;
                float val;

                printf("Inserisci il nome del titolo:\n");
                scanf("%s",str);

                title=STsearch(titleList,str);
                if(title!=NULL){

                    val=BSTsearchAbsoluteMax(TITLEgetBST(title));
                    TITLEprint(stdout, title);
                    printf("\nValore massimo: %f", val);
                }
                else{
                    printf("\nTitolo non esistente");
                }
            }
            break;
			case bilanciaTBST:
			{
				BST bst;
                Title title;
				char str[T_NAME_SIZE];

				printf("Inserisci il nome del titolo:\n");
                scanf("%s",str);
                title=STsearch(titleList, str);

                if(title!=NULL){
                    bst=TITLEgetBST(title);
                    BSTbalance(bst);
                    printf("\nBilanciamento effettuato");
                }
                else{
                    printf("\nTitolo non esistente");
                }
			}
			break;
			
		}		
		printf("\n\n");
		
	}while(query!=fine);
	
	STfree(titleList);
	return 0;
}

void fileRead(STtitle list, FILE *fp){
    char titleName[T_NAME_SIZE];
    int nQuots,nTitles;
    BST bst;
    Quot quot;
    Time time;
    Title title;
    fscanf(fp,"%d ",&nTitles);
    for(int j=0;j<nTitles;j++){
        fscanf(fp,"%s %d ",titleName,&nQuots);

        if((title=STsearch(list,titleName))==TITLEgetNull()){
            title=TITLEinit(titleName);
            STinsert(list,title);
        }
        bst=TITLEgetBST(title);
        for(int i=0;i<nQuots;i++){
            TIMEread(fp,&time);
            if(QUOTisNull(quot=(BSTsearch(bst,time)))){
                QUOTinit(&quot,time);
                BSTinsert(bst,quot);
            }
            BSTupdatequot(fp,bst,time);
        }
    }
}
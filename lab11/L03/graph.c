#include "graph.h"

static void setMatAdj(Graph g,Edge *eList);
static void initMat(Graph g,int val);
typedef struct ladjNode ladjNode,*ladjLink;

typedef struct G {
	ST vertList;
	int numV,numE;
	int **matAdj;
}*Graph;

struct ladjNode{
	int id;
	int weight;
	ladjLink next;
};

static ladjLink NEW(int id,int weight,ladjLink next){
	ladjLink h = malloc(sizeof(ladjNode));
	h->next=next;
	h->weight=weight;
	h->id=id;
	return h;
}
static void FREEnodeR(ladjLink h){
	if(h==NULL)return;
	FREEnodeR(h->next);
	free(h);
}

Graph GRAPHinit(){
	Graph g = malloc(sizeof(struct G));
	g->numV=0;g->numE=0;
	g->matAdj=NULL;
	g->vertList = STnew();
	return g;
}

void GRAPHfree(Graph g){
	STfree(g->vertList);
    free(g);
}


Graph GRAPHload(FILE *fp){
    	
    int numE;
	char vName[V_NAME_SIZE],wName[V_NAME_SIZE];
	int v,w,netv,netw,weight;
	Graph g = GRAPHinit();
	
	numE=getFileLines(fp);
	rewind(fp);

	STinit(g->vertList,numE*2);
	Edge *edges=malloc(numE*sizeof(Edge));
	
	Edge tmpEdge;
	Vert tmpV1,tmpV2;
	
	for(int i=0;i<numE;i++)
	{
		fscanf(fp,"%s Net%d %s Net%d %d ",vName,&netv,wName,&netw,&weight); 

		VERTset(&tmpV1,netv,vName);
		VERTset(&tmpV2,netw,wName);

		if((v=STsearchByName(g->vertList, vName))==-1){
		    STinsert(g->vertList,tmpV1);
            	    v=g->numV++;
		}
		if((w=STsearchByName(g->vertList, wName))==-1){
		    STinsert(g->vertList,tmpV2);
            	    w=g->numV++;
		}

	        EDGEset(&tmpEdge,v,w,weight);
	        edges[i]=tmpEdge;
	}
	
	g->numE=numE;
	setMatAdj(g,edges);
	free(edges);
	
	return g;
}

static void setMatAdj(Graph g,Edge *eList){

	initMat(g,0);

	for(int i=0;i<g->numE;i++)
	{
		g->matAdj[eList[i].v][eList[i].w]=eList[i].weight;
		g->matAdj[eList[i].w][eList[i].v]=eList[i].weight;
	}
}

static void initMat(Graph g,int val){
	if(g->matAdj!=NULL)
		free(g->matAdj);	
	malloc2Dp(&(g->matAdj),g->numV,g->numV);

	if(g->matAdj==NULL)
	    exit(2);

	for(int i=0;i<g->numV;i++)
	{
		for(int j=0;j<g->numV;j++)
		{

			g->matAdj[i][j]=val;
		}
	}
}

int GRAPHSubThree(Graph g,char *a,char *b,char *c){
    int *vects;
    int i,j;
    vects=malloc(3*sizeof(Vert));

    vects[0]=STsearchByName(g->vertList, a);
    vects[1]=STsearchByName(g->vertList, b);
    vects[2]=STsearchByName(g->vertList, c);

	for(int i=0;i<3;i++)
		if(vects[i]==-1)
			return 0;
		
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			if(j!=i)
			{
				if(g->matAdj[vects[i]][vects[j]]==0)
					return 0;
			}
		}
	}
	return 1;
}

static ladjLink* GRAPHgetLAdj(Graph g){
	ladjLink* ladj = malloc(g->numV*sizeof(ladjLink));
	ladjLink h;

    for(int i=0;i<g->numV;i++)
        ladj[i]=NEW(-1,0,NULL);

	for(int i=0;i<g->numV;i++){
		for(int j=0;j<g->numV;j++)
			if(g->matAdj[i][j]!=0){
			    if(ladj[i]->next==NULL){
                    h=NEW(j,g->matAdj[i][j],NULL);
                    ladj[i]->next=h;
			    }
			    else
                {
                    h->next=NEW(j, g->matAdj[i][j], NULL);
                    h=h->next;
                }
			}
	}
	return ladj;
}

static void FREEladj(ladjLink* ladj,int numV){
	for(int i=0;i<numV;i++)
        FREEnodeR(ladj[i]);
	free(ladj);
}

void GRAPHprintListAdj(FILE *fp, Graph g){

	if(g==NULL)
	{
		printf("Grafo non inizilizzato");
		return;
	}
	
	ladjLink* ladj = GRAPHgetLAdj(g);
	ladjLink h;
	for(int i=0;i<g->numV;i++){		
		fprintf(fp,"%d) %10s: ",i,VERTgetName(STgetByIndex(g->vertList, i)));
		h=ladj[i]->next;
		while(h!=NULL){
			fprintf(fp,"%d/%d ",h->id,h->weight);
			h=h->next;
		}
		fprintf(fp,"\n");
	}
	FREEladj(ladj,g->numV);	
}

void GRAPHprintOrder(FILE *fp,Graph g){
	int i, j;
	int *ordV; //non fixa matrice, quindi uso realI quando per cercare gli archi dentro la matrice
    int *ordAdj;
    ordV=STorderByName(g->vertList);
    ST tmpSt; //dati gli archi (v,w) questa st contiene i vertici w adiacenti al vertice v

	for(i=0;i<g->numV;i++){
        tmpSt=STnew();
        STinit(tmpSt,g->numV);

		VERTprint(fp,*STgetByIndex(g->vertList,ordV[i]));
		printf("\n");

		for(j=0;j<g->numV;j++){
			if(g->matAdj[ordV[i]][j]!=0)
				STinsert(tmpSt,*STgetByIndex(g->vertList, j));
		}
		
		ordAdj=STorderByName(tmpSt);
		
		for(j=0;j<STcount(tmpSt);j++){
			printf("\t");
			VERTprint(fp,*STgetByIndex(tmpSt, ordAdj[j]));
            printf(" weight: %d",g->matAdj[ordV[i]][STsearchByName(g->vertList,VERTgetName(STgetByIndex(tmpSt,ordAdj[j])))]);
			printf("\n");	
		}
		free(ordAdj);
		STfree(tmpSt);
	}
	free(ordV);
}

//debug
static void printMatrix(int **mat,int row,int col){
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            printf("%d ",mat[i][j]);
        }
        printf("\n");
    }
}


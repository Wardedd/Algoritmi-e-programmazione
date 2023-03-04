#include "graph.h"

typedef struct edgeinfo einfo;

static void setMatAdj(Graph g,Edge *eList);
static void initMat(Graph g,int val);

static void malloc2Dp(einfo*** mat, int nr,int nc);
static void free2Dp(einfo** mat, int nr);

struct edgeinfo{
    int weight;
    int used;
};

typedef struct G {
    ST vertList;
    int numV,numE;
    einfo **matAdj;
}*Graph;

Graph GRAPHinit(){
	Graph g = malloc(sizeof(struct G));
	g->numV=0;g->numE=0;
	g->matAdj=NULL;
	g->vertList = STnew();
	return g;
}


Graph GRAPHload(FILE *fp){
    	
    int numE,numV;
	char vName[V_NAME_SIZE],wName[V_NAME_SIZE];
	int v,w,weight,i;
    Vert tmpV1,tmpV2;
    Graph g = GRAPHinit();

    readIndex(fp,&numV);
    g->numV=numV;
    STinit(g->vertList,numV);

    for(i=0;i<numV;i++){
        VERTread(fp,&tmpV1);
        STinsert(g->vertList,tmpV1);
    }
    numE=numV*numV; //alternativa x=righe file; numE=x-numV-1
	Edge *edges=malloc(numE*sizeof(Edge));
	Edge tmpEdge;

    i=0;
    while(fscanf(fp,"%s %s %d ",vName,wName,&weight)==3 && i<numE){

		VERTset(&tmpV1,vName);
		v=STsearch(g->vertList,vName);
		VERTset(&tmpV2,wName);
        w=STsearch(g->vertList,wName);

        EDGEset(&tmpEdge,v,w,weight);
        edges[i++]=tmpEdge;
	}
    g->numE=i;

    setMatAdj(g,edges);
    free(edges);
	
    return g;
}

static void setMatAdj(Graph g,Edge *eList){

	initMat(g,0);

	for(int i=0;i<g->numE;i++)
	{
		g->matAdj[eList[i].v][eList[i].w].weight=eList[i].weight;
        	g->matAdj[eList[i].v][eList[i].w].used=1;
	}
}

static void initMat(Graph g,int val){
	if(g->matAdj!=NULL)
		free(g->matAdj);	
	malloc2Dp(&(g->matAdj),g->numV,g->numV);

	if(g->matAdj==NULL)
	    exit(2);

    einfo x;
    for(int i=0;i<g->numV;i++)
	{
        for(int j=0;j<g->numV;j++)
		{
            x.weight=val;
            x.used=0;
        
   	g->matAdj[i][j]=x;
		}
	}
}

void GRAPHprintListAdj(FILE *fp, Graph g){

	if(g==NULL)
	{
		printf("Grafo non inizializzato\n");
		return;
	}
    if(g->matAdj==NULL)
    {
        printf("Grafo vuoto\n");
        return;
    }
	for(int i=0;i<g->numV;i++){
		fprintf(fp,"%d) %s:",i,VERTgetName(STselect(g->vertList,i)));
		for(int j=0;j<g->numV;j++)
			if(g->matAdj[i][j].used==1)
				fprintf(fp," %d",j);
		fprintf(fp,"\n");
	}	
}

void GRAPHfree(Graph g){
	STfree(g->vertList);
	free2Dp(g->matAdj,g->numV);
    free(g);
}

static void GRAPHcopy(Graph dst,Graph src){
    if(src==NULL||dst==NULL){
        printf("Grafo non inizializzato");
        exit(3);
    }
    dst->numV=src->numV;
    dst->numE=src->numE;
    dst->vertList=STnew();
    STcopy(dst->vertList,src->vertList);

    initMat(dst,0);
    for(int i=0;i<src->numV;i++){
        for(int j=0;j<src->numV;j++){
            dst->matAdj[i][j]=src->matAdj[i][j];
        }
    }
}

/*            checkDagR             */
static int checkDagR(einfo** mat,int *v,int numV,int i);
int GRAPHdagCheck(Graph g){
	int v[g->numV];
	for(int i=0;i<g->numV;i++)
		v[i]=0;
		
	return checkDagR(g->matAdj,v,g->numV,0);	
}

static int checkDagR(einfo** mat,int *v,int numV,int i){
	int tmp;
	v[i]++;
	if(v[i]==2)
		return 0;

	for(int j=0;j<numV;j++){
		if(mat[i][j].used==1)
		{
			tmp = checkDagR(mat,v,numV,j);
			if(tmp==0)
				return 0;
		}
	}
	v[i]--;
	return 1;
}

/*           GRAPHgetMinDag         */
static void minDagR(Graph g,int i,int j, int tmpval,int tmpi,Graph bestg,int *bestval, int k);
Graph GRAPHgetMinDag(Graph g){

    if(GRAPHdagCheck(g)){
        return g;
    }
    Graph bestg=GRAPHinit();
    int bestval=-1;

    for(int k=1;k<g->numV && bestval==-1;k++){
        minDagR(g,0,0,bestval,0,bestg,&bestval,k);
    }
    return bestg;
}

static void minDagR(Graph g,int i,int j, int tmpval,int tmpi,Graph bestg,int *bestval, int k){ //conviene passare vedge se siamo in un grafo sparso
    if(tmpi==k){

        if(tmpval>*bestval && GRAPHdagCheck(g))
        {

            GRAPHcopy(bestg,g);
            *bestval=tmpval;
        }

        return;
    }

    int newi,newj;
    for(i;i<g->numV;i++){

        newi=i;
        for(j;j<g->numV;j++){
            if(g->matAdj[i][j].used==1){

                g->matAdj[i][j].used=0;
                newj=j+1;
                if(newj==g->numV) {newi++;newj=0;}

                minDagR(g,newi,newj,tmpval+g->matAdj[i][j].weight,tmpi+1,bestg,bestval,k);
                g->matAdj[i][j].used=1;
            }
        }
        j=0;
    }
}

static void TSdfsR(einfo** mat,int *ts,int *pos,int *time,int numV,int i){
    pos[i]=1;
    for(int j=0;j<numV;j++)
        if(mat[i][j].used==1 && pos[j]==-1){
            TSdfsR(mat,ts,pos,time,numV,j);
        }
    ts[(*time)++]=i;
}
/*        GRAPHdagPrintMaxPaths (vectFind)       */

static int vectFind(int *vect,int maxN,int num);
static void arrInverse(int *v,int size);

void GRAPHdagPrintMaxPaths(Graph g){
    if(!GRAPHdagCheck(g))
    {
        printf("Grafo non è un dag");
        return;
    }
    printf("Cammini massimi da ogni vertice SORGENTE(out-degree>0) ai vertici del DAG:\n");
    int *wt = malloc(g->numV*sizeof(int));
    int *ts = malloc(g->numV*sizeof(int));
    int *pos = malloc(g->numV*sizeof(int));

    //get parent vector
    //ord top. inverso giro=>ord top | non uso questa tecnica
    int *time=malloc(sizeof(int));
    int edgenum;

    for(int i=0;i<g->numV;i++){
        edgenum=0;
        for(int j=0;j<g->numV && edgenum==0;j++)
        {
            if (g->matAdj[i][j].used==1) //allora i è nodo sorgente
            {
                edgenum++;
                *time=0;

                for (int k=0; k<g->numV; k++)
                {
                    wt[k]=MIN_WEIGHT;
                    ts[k]=-1;
                    pos[k]=-1;
                }
                wt[i]=0;
                TSdfsR(g->matAdj, ts, pos, time, g->numV, i);

                //ts ordine topologico inverso, voglio ord top, quindi inverto ts
                arrInverse(ts,*time);

                //percorro i vertici secondo l'ordine topologico indicato da ts
                //applico la relaxation inversa ad essi
                for (int t=0; t<*time; t++)
                {
                    for (int w=0; w<g->numV; w++)
                    {
                        if (g->matAdj[ts[t]][w].used==1)
                            if (wt[w]<wt[ts[t]]+g->matAdj[ts[t]][w].weight)
                                wt[w]=wt[ts[t]]+g->matAdj[ts[t]][w].weight;
                    }
                }

                VERTprint(stdout, *STselect(g->vertList, i));
                int tmpi;
                for (int i=0; i<g->numV; i++)
                {
                    printf("\n\t%s:", VERTgetName(STselect(g->vertList, i)));
                    //se vett contiene i, la f. ritorna la sua posizione in ts
                    //guardare wt non bastava in caso di esistenza di archi con peso 0
                    if((tmpi=vectFind(ts,*time,i))!=-1)
                        printf(" %d", wt[ts[tmpi]]);
                    else{
                        printf(" non raggiungibile");
                    }
                }
                printf("\n");
            }
        }
    }
    free(time);
    free(wt);
    free(ts);
    free(pos);
}

static void arrInverse(int *v,int size){
    int *tmp = malloc((size/2)*sizeof(int));
    int i;
    for(i=0;i<size/2;i++)
        tmp[i]=v[i];
    for(i=0;i<size/2;i++)
        v[i]=v[size-i-1];
    for(i=0;i<size/2;i++)
        v[size-1-i]=tmp[i];
    free(tmp);
}

static int vectFind(int *vect,int maxN,int num){
    for(int i=0;i<maxN;i++)
        if(vect[i]==num)
            return i;
    return -1;
}
/*         malloc matrix       */
static void malloc2Dp(einfo*** mat, int nr,int nc){
    *mat = (einfo**)malloc(nr*sizeof(**mat));
    if(*mat==NULL)
    {
        printf("Errore nell'allocazione della memoria");
        exit(2);
    }
    else{
        for(int i=0;i<nr;i++)
        {
            (*mat)[i] = (einfo*)malloc(nc*sizeof(***mat));
            if((*mat)[i]==NULL)
            {
                printf("Errore nell'allocazione della memoria");
                exit(2);
            }
        }
    }
}

static void free2Dp(einfo** mat, int nr){
    for(int i=0;i<nr;i++){
        free(mat[i]);
    }
    free(mat);
}

//per debug
static void printMatrix(einfo **mat,int row,int col){
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            printf("%d ",mat[i][j].used);
        }
        printf("\n");
    }
}

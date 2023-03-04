#include <stdlib.h>
#include <stdio.h>

void powerset_r(int pos,int *sol,int k,int start,int *nodes,int **edges,int n_nodes,int n_edges);
void checkVect(int k,int *comb,int **edges,int n_edges);

int *getNodes(int **edges,int size,int *n_nodes);
int vContains(int *v,int v_size,int num);

int** getEdges(FILE *fp,int size);
void printAllVertex(int **edges,int *nodes,int n_edges,int n_nodes);
void printVertex(int comb[],int k);
void openFile(FILE **fp,char *fileName,char *mode);
int getFileLines(FILE *fp);
void checkPointer(void *p,char *s);

void malloc2Dp(int ***mat,int n_edges);
void free2Dp(int ***edges,int n_edges);
void freeAll(int ***edges,int **nodes, int size);

//grafo semplice - tolgo i cappi manualmente
//leggo file 2 volte, getFile lines legge il numero di righe
//no doppi archi, essendo un grafo monodirezionale

int main() {
    FILE *fp;
    int **edges;
    int *nodes;
    int n_edges,n_nodes;
    openFile(&fp,"grafo.txt","r");
    n_edges=getFileLines(fp);
    rewind(fp);

    edges=getEdges(fp,n_edges);
    nodes=getNodes(edges,n_edges,&n_nodes); //n_nodes settato da questa funzione

    printAllVertex(edges,nodes,n_edges,n_nodes);

    freeAll(&edges,&nodes,n_edges);
    return 0;
}

void powerset_r(int pos,int sol[],int k,int start,int *nodes,int **edges,int n_nodes,int n_edges){
    int i;
    if (pos >= k) {
        checkVect(k, sol, edges,n_edges);
        return;
    }
    for (i = start; i < n_nodes; i++) {
        sol[pos] = nodes[i];
        powerset_r(pos+1, sol, k, i+1,nodes,edges,n_nodes,n_edges);
    }
}

void checkVect(int k,int *comb,int **edges,int n_edges){
    int tmpedges=0;
    int found;

   //itera per ogni edge, controllando se l'insieme di vertici è un vertex cover del grafo
    //ovvero if finale, se archi trovati(con l'insieme) == numero archi reali

    /*
     * dato insieme di nodi (comb)
     * conta il numero di archi che questi toccano
     * nodo 3, {1,3}, {3,5} archi trovati:2 (if, else if sotto serve a questo)
     * se il numero di archi trovato = num archi grafo allora abbiamo un vertex cover, printalo :D
     */
    for (int j = 0; j < n_edges; j++)
    {
        found=0;
        for(int i=0;i<k&&!found;i++)
        {
            if (edges[j][0] == comb[i])
            {
                tmpedges++;
                found=1;
            }
            else if (edges[j][1] == comb[i])
            {
                tmpedges++;
                found=1;
            }
        }
    }
    if(tmpedges==n_edges)
        printVertex(comb,k);
}

int vContains(int *v,int v_size,int num){
    for(int i=0;i<v_size;i++){
        if(v[i]==num)
            return 1;
    }
    return 0;
}

int *getNodes(int **edges,int size,int *n_nodes) {
    int inode=0;
    int *nodes = (int*)malloc(size*2*sizeof(int));

    for (int i = 0; i < size; i++) {
        if (!vContains(nodes, inode,edges[i][0]))
            nodes[inode++] = edges[i][0];
        if (!vContains(nodes, inode, edges[i][1]))
            nodes[inode++] = edges[i][1];
    }
    *n_nodes=inode;
    return nodes;
}

//io stuff

int** getEdges(FILE *fp,int size) {
    int **edges;
    int tmpa, tmpb;

    malloc2Dp(&edges,size);

    for(int i=0;i<size;i++)
    {
        fscanf(fp,"%d %d",&tmpa,&tmpb);

        if(tmpa!=tmpb)
        {
            edges[i][0]=tmpa;
            edges[i][1]=tmpb;
        }
        else i--;
    }
    return edges;
}

void printVertex(int comb[],int k){
    printf("Vertice di grandezza %d: \n",k);
    for(int i=0;i<k;i++)
        printf("%d ",comb[i]);
    printf("\n");

}

void printAllVertex(int **edges,int *nodes,int n_edges,int n_nodes){
    if(n_edges==0){
        printf("Insieme vuoto");
    }else{
        int sol[n_nodes];
        for(int k=1;k<=n_nodes;k++) //usando il
            powerset_r(0,sol,k,0,nodes,edges,n_nodes,n_edges);
    }
}

int getFileLines(FILE *fp) {
    int i = 0;
    int a,b;
    while (fscanf(fp, "%d %d",&a,&b) == 2)
        if(a!=b)
                i++;
    return i;
}

void openFile(FILE **fp,char *fileName,char *mode){
    *fp=fopen(fileName,mode);
    checkPointer(fp,"Errore nell'apertura del file");
}

void checkPointer(void *p,char *s){
    if(p==NULL)
    {
        printf("%s",s);
        exit(-1);
    }
}
//dyn mem
void malloc2Dp(int ***mat,int size){
    *mat = (int**) malloc(size * sizeof(**mat));

    if (*mat == NULL)
    {
        printf("Problema nell'allocazione dinamica della matrice");
        exit(1);
    }
    for(int i=0;i<size;i++) {
        (*mat)[i] = (int *) malloc(2 * sizeof(***mat));
        if((*mat)[i]==NULL){
            printf("Problema nell'allocazione dinamica della matrice");
            exit(2);
        }
    }

}

void free2Dp(int ***edges,int size){
    for(int i=0;i<size;i++){
        free((*edges)[i]);
    }
    free(*edges);
}

void freeAll(int ***edges,int **nodes, int size){
    free2Dp(edges,size);
    free(*nodes);
    *edges=NULL;
    *nodes=NULL;
}
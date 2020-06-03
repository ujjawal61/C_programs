
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include<stdbool.h>
#define MAX_BUFFER 256
#define MAX 8000

struct AdjListNode
{
    
    int dest;
    int weight;
    struct AdjListNode* next;
};
struct AdjList
{
    struct AdjListNode *head;
};
struct Graph
{
    int V;
    struct AdjList* array;
};
struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
    int i;
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    
    return graph;
}
void addEdge(struct Graph* graph,int src, int dest, int weight)
{
    
    if(dest>=graph->V)
    {
        return;
    }
    else if(src>=graph->V)
    {
        return;
    }
    else{
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
    }
     
}

struct verticenode{
    int sid;
    char name[MAX_BUFFER];
    double longitutde;
    double latittude;
}vn[MAX];

struct edgenode{
    int v1;
    int v2;
    int weight;
}en;

int next_field( FILE *f, char *buf, int max ) {
    int i=0, end=0, quoted=0;
    for(;;) {
        buf[i] = fgetc(f);
        if(buf[i]=='"') { quoted=!quoted; buf[i] = fgetc(f); }
        if(buf[i]==',' && !quoted) { break; }
        if(feof(f) || buf[i]=='\n') { end=1; break; }
        if( i<max-1 ) { ++i; }
    }
    buf[i] = 0;
    return end;
}

void fetch_vertices( FILE *csv ) {
    
    char buf[MAX_BUFFER];
    next_field( csv, buf, MAX_BUFFER );
    int id;
    id = atoi(buf);
    vn[id].sid=id;
    
    next_field( csv, buf, MAX_BUFFER );
    strcpy(vn[id].name,buf);
    
    next_field( csv, buf, MAX_BUFFER );
    vn[id].latittude = atof(buf);
    
    next_field( csv, buf, MAX_BUFFER );
    vn[id].longitutde = atof(buf);
    
    
}
void fetch_edges( FILE *csv) {
    
    char buf[MAX_BUFFER];
    next_field( csv, buf, MAX_BUFFER );
    en.v1 = atoi(buf);
    next_field( csv, buf, MAX_BUFFER );
    en.v2 = atoi(buf);
    next_field( csv, buf, MAX_BUFFER );
    en.weight = atoi(buf);
}



struct pathlist{
    int s;
    int c;
}path[MAX];

struct MinHeapNode
{
    int  v;
    int dist;
};

struct MinHeap
{
    int size;
    int capacity;
    int *pos;
    struct MinHeapNode **array;
};

struct MinHeapNode* newMinHeapNode(int v, int dist)
{
    struct MinHeapNode* minHeapNode = (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

struct MinHeap* createMinHeap(int capacity)
{
    struct MinHeap* minHeap = (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =  (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
    
    if (left < minHeap->size &&
        minHeap->array[left]->dist < minHeap->array[smallest]->dist )
        smallest = left;
    
    if (right < minHeap->size &&
        minHeap->array[right]->dist < minHeap->array[smallest]->dist )
        smallest = right;
    
    if (smallest != idx)
    {
        
        struct MinHeapNode *smallestNode = minHeap->array[smallest];
        struct MinHeapNode *idxNode = minHeap->array[idx];
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isEmpty(struct MinHeap* minHeap)
{
    return minHeap->size == 0;
}
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;
    
    struct MinHeapNode* root = minHeap->array[0];
    
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
    
    --minHeap->size;
    minHeapify(minHeap, 0);
    
    return root;
}

void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
    int i = minHeap->pos[v];
    
    minHeap->array[i]->dist = dist;
    
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
        
        i = (i - 1) / 2;
    }
    
    
    
}

bool isInMinHeap(struct MinHeap *minHeap, int v)
{
    if (minHeap->pos[v] < minHeap->size)
    {
        return true;}
    return false;
}

void pathinit(int v)
{
    int k=0;
    for (k=0; k<v; k++) {
        path[k].c=k;
        path[k].s=-1;
    }
}
void printPath( int j)
{
    if (path[j].s == -1)
    {
        printf("\n%d\t\t\t%s\t\t%lf\t\t\t%lf",vn[j].sid,vn[j].name,vn[j].latittude,vn[j].longitutde);
        return;
    }
    printf("\n%d\t\t\t%s\t\t\t%lf\t\t\t%lf",vn[j].sid,vn[j].name,vn[j].latittude,vn[j].longitutde);
    printPath( path[j].s);
    
}
void printArr(int dist[MAX], int n,int s,int d)
{
    
    printf("Path");
    
    printf("\n%d\t\t\t%s\t\t\t%lf\t\t\t%lf",vn[d].sid,vn[d].name,vn[d].latittude,vn[d].longitutde);
    printPath(path[d].s);
    printf("\n");
    
    
}

void dijkstra(struct Graph* graph, int src,int t)
{
    
    int V = graph->V;
    int dist[V];
    pathinit(V);
    struct MinHeap* minHeap = createMinHeap(V);
    int v;
    for (v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }
    
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src]   = src;
    dist[src] = 0;
    
    decreaseKey(minHeap, src, dist[src]);
    
    minHeap->size = V;
    
    while (!isEmpty(minHeap))
    {
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;
        
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;
            
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
                pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;
                path[v].s=u;
                decreaseKey(minHeap, v, dist[v]);
            }
            
            pCrawl = pCrawl->next;
        }
        
    }
    
    printArr(dist, V,src,t);
}

int main () {
    FILE *f1;
    FILE *f2;
    
    f1 = fopen("vertices.csv", "r");
    if(!f1) {
        printf("unable to open %s\n", "vertices.csv");
        return EXIT_FAILURE;
    }
    int countvertices=1;
    fetch_vertices( f1);
    while(!feof(f1)) {
        fetch_vertices( f1);
        countvertices++;
    }
    printf("\nLoaded %d vertices",countvertices);
    f2 = fopen("edges.csv", "r");
    if(!f2) {
        printf("unable to open %s\n", "edges.csv");
        return EXIT_FAILURE;
    }
    int countedges=0;
    struct Graph* graph = createGraph(countvertices);
    
     do{
        fetch_edges( f2 );
        countedges++;
        addEdge(graph, en.v1, en.v2,en.weight);
     }while(!feof(f2));
    printf("\nLoaded %d edges",countedges);
    printf("\n");
    int startid;
    int destid;
    printf("\nEnter start stop: ");
    scanf("%d",&startid);
    printf("\nEnter end stop: ");
    scanf("%d",&destid);
    printf("\n");
    dijkstra(graph,destid, startid);
    fclose(f1);
    fclose(f2);
    return EXIT_SUCCESS;
}



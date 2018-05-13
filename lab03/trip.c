#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define BSIZE 1<<15  
  
/*********************Global Variables**************************/  
char buffer[BSIZE];  
long long int bpos = 0L, bsize = 0L;  
  
/***************************************************************/  
  
/********************* Structs & Types**************************/

struct node{
	long long int dest;
	long long int weight;
	struct node* next;
};

struct AdjList{
	struct node* head;
};

typedef struct AdjList AdjListT;

struct Graph{
	long long int V;
	struct AdjList* array;
};

typedef struct Graph GraphT;

struct heapNode{
	long long int n;
	long long int key;
};

typedef struct heapNode heapNodeT;

struct heap{
	long long int size;
	long long int capacity;
	long long int* position;
	heapNodeT **array;
};

typedef struct heap heapT;

struct List{
	long long int number;
	long long int maxWeight;
	struct List *next;
};

typedef struct List ListT;

/***************************************************************/   
  
  
/********************Functions**********************************/  
long long int readLong();  
GraphT * createGraph(long long int V);
struct node* newAdjListNode(long long int dest, long long weight);
void addEdge(GraphT* graph, long long int source, long long int destination, long long int weight);
void primMST(GraphT* graph, GraphT* MST);
void printGraph(GraphT* graph); 
heapT * createHeap(long long int N);
heapNodeT * newHeapNode(long long int N, long long int key);
heapNodeT * findMinNode(heapT* myHeap);
void minHeapify(heapT* myHeap, long long int idx);
void swapHeapNodes(heapNodeT** a, heapNodeT** b);
void decreaseKey(heapT* myHeap, long long int x, long long int key);
void printArr(GraphT * graph, long long int * array, long long int * weight, long long int N);
void BFS(GraphT * graph, long long int N, long long int src, long long int ** space);
void solve(GraphT * graph, long long int N, long long int Q, long long ** path);
/***************************************************************/

int main(int argc, char **argv){
	
	long long int i;
	
	long long int N = readLong();
	long long int M = readLong();
	
	GraphT* myGraph = createGraph(N);
	
	long long int x,y,d;
	
	for (i=0; i<M; i++){
		x = readLong() - 1;
		y = readLong() - 1;
		d = readLong();
		
		addEdge(myGraph, x, y, d);
	}
	
	GraphT* MSTGraph = createGraph(N);
	
	primMST(myGraph, MSTGraph);
	
	long long int Q = readLong();

	long long int** path = (long long int **)malloc(Q * sizeof(long long int *));	
	for(i=0; i<Q; i++){
		path[i] = (long long int *)malloc(2 * sizeof(long long int));
	}
	
	for(i=0; i<Q; i++)
		for(int j=0; j<2; j++) path[i][j] = readLong() - 1;
	
	solve(MSTGraph, N, Q, path);
	
	return 0;
	
}

/********************************************************************/  
/*                        readLong                                  */  
/* The readLong is a function which read from std input integers    */  
/* and convert them to long long integers.                          */  
/* This function is given from:                                     */  
/* http://www.corelab.ntua.gr/courses/algorithms/problemsets/read.c */  
/********************************************************************/  
  
  
long long int readLong() {  
    long long int d = 0L, x = 0L;  
    char c;  
    int sign_flag=1;  
  
    while (1)  {  
        if (bpos >= bsize) {  
            bpos = 0;  
            if (feof(stdin)) return x;  
            bsize = fread(buffer, 1, BSIZE, stdin);  
        }  
        c = buffer[bpos++];  
        if (c == '-') sign_flag= -1; // a small modification for negative integers   
        if (c >= '0' && c <= '9') { x = (x*10 + (c-'0')); d = 1; }  
        else if (d == 1) return sign_flag * x;  
    }  
    return 0;  
}  

GraphT* createGraph(long long int V){
	
	GraphT* graph = (GraphT *)malloc(sizeof(GraphT));
	
	graph->V = V;
	graph->array = (AdjListT *)malloc(V * sizeof(AdjListT));
	
	for(long long int i=0; i<V; i++) graph->array[i].head = NULL;
	
	return graph;

}

struct node * newAdjListNode(long long int dest, long long weight){
	
	struct node* newNode = (struct node *)malloc(sizeof(struct node));
	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = NULL;
	
	return newNode;
	
}

void addEdge(GraphT* graph, long long int source, long long int destination, long long int weight){
	
	struct node* newNode = newAdjListNode(destination, weight);
	newNode->next = graph->array[source].head;
	graph->array[source].head = newNode;
	
	newNode = newAdjListNode(source, weight);
	newNode->next = graph->array[destination].head;
	graph->array[destination].head = newNode;
}

void printGraph(GraphT* graph){
    long long int v;
    for (v = 0; v < graph->V; ++v){
        struct node* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

void primMST(GraphT* graph, GraphT* MST){
	
	long long int verticesNumber = graph->V;
	long long int* parent = malloc(verticesNumber * sizeof(long long int));
	long long int* key = malloc(verticesNumber * sizeof(long long int));
	
	heapT* minHeap = createHeap(verticesNumber);
	
	for(long long int i=1; i<verticesNumber; i++){
		parent[i] = -1;
		key[i] = LLONG_MAX;
		minHeap->array[i] = newHeapNode(i, key[i]);
		minHeap->position[i] = i;
	}
		
		key[0] = 0;
		minHeap->array[0] = newHeapNode(0, key[0]);
		minHeap->position[0] = 0;
		minHeap->size = verticesNumber;
		
		while(minHeap->size != 0){
			
			heapNodeT* minHeapNode = findMinNode(minHeap);
			long long int u = minHeapNode->n;
			
			struct node* pCrawl = graph->array[u].head;
			while(pCrawl != NULL){
				int i = pCrawl->dest;
				
				if((minHeap->position[i] < minHeap->size) && (pCrawl->weight < key[i])){
					key[i] = pCrawl->weight;
					parent[i] = u;
					decreaseKey(minHeap, i, key[i]);
					
				}
				pCrawl = pCrawl->next;
			}
	}
	printArr(MST, parent, key, verticesNumber);
}

heapT * createHeap(long long int N){
	
	heapT* myHeap =(heapT*)malloc(sizeof(heapT));
	myHeap->position = (long long int *)malloc(N * sizeof(long long int));
	myHeap->size = 0;
	myHeap->capacity = N;
	myHeap->array = (heapNodeT **)malloc(N * sizeof(heapNodeT*));
	
	return myHeap;
}

heapNodeT* newHeapNode(long long int N, long long int key){
	
	heapNodeT* myHeapNode = (heapNodeT *)malloc(sizeof(heapNodeT));
	myHeapNode->n = N;
	myHeapNode->key = key;
	
	return myHeapNode;
}

heapNodeT * findMinNode(heapT* myHeap){
	
	if(myHeap->size == 0) return NULL;
	
	heapNodeT* root = myHeap->array[0];
	heapNodeT* lastNode = myHeap->array[myHeap->size - 1];
	myHeap->array[0] =lastNode;
	
	myHeap->position[root->n] = myHeap->size-1;
	myHeap->position[lastNode->n] = 0;
	
	myHeap->size--;
	minHeapify(myHeap,0);
	
	return root;
}

void minHeapify(heapT* myHeap, long long int idx){
	
	long long int minimum, left, right;
	
	minimum = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;
	
	if((left < myHeap->size) && (myHeap->array[left]->key < myHeap->array[minimum]->key)) minimum = left;
	if((right < myHeap->size) && (myHeap->array[right]->key < myHeap->array[minimum]->key)) minimum = right;
	
	if(minimum != idx){
		
		heapNodeT* minNode = myHeap->array[minimum];
		heapNodeT* idxNode = myHeap->array[idx];
		
		myHeap->position[minNode->n] = idx;
		myHeap->position[idxNode->n] = minimum;
		
		swapHeapNodes(&myHeap->array[minimum], &myHeap->array[idx]);
		
		minHeapify(myHeap, minimum);
	}
	
}

void swapHeapNodes(heapNodeT** a, heapNodeT** b){
	
	heapNodeT* tmp = *a;
	*a = *b;
	*b = tmp;
	
}

void decreaseKey(heapT* myHeap, long long int x, long long int key){
	
	int N = myHeap->position[x];
	myHeap->array[N]->key = key;
	
	while(N && myHeap->array[N]->key < myHeap->array[(N-1)/2]->key){
		myHeap->position[myHeap->array[N]->n] = (N-1)/2;
		myHeap->position[myHeap->array[(N-1)/2]->n] = N;
		swapHeapNodes(&myHeap->array[(N-1)/2], &myHeap->array[N]);
		
		N = (N-1)/2;
	}
	
}

void printArr(GraphT * graph, long long int * array, long long int * weight, long long int N){
	for(int i=1; i<N; i++) addEdge(graph, array[i], i, weight[i]);
}

void solve(GraphT * graph, long long int N, long long int Q, long long ** path){
	
	long long int src=-1;
	long long int dst=-1;
	long long int ** space = (long long int **)malloc(N * sizeof(long long int *));
	for(long long int i=0; i<N; i++) space[i] = (long long int *)malloc(N * sizeof(long long int));
	
	for(long long int i=0; i<N; i++) BFS(graph, N, i, space);
	
	for(long long int i=0; i<Q; i++){
		src = path[i][0];
		dst = path[i][1];
		printf("%lld\n", space[src][dst]);
	}
}

void BFS(GraphT * graph, long long int N, long long int src, long long int ** space){
	
	int* visited = (int *)malloc(N * sizeof(int));
	for(long long i=0; i<N; i++) visited[i]=0;
	
	ListT* queueHead = (ListT *)malloc(sizeof(ListT));
	ListT* queueLast = queueHead;
	
	visited[src] = 1;
	
	queueHead->number = src;
	queueHead->next = NULL;
	
	long long int s = src;
	
	long long int* weight = (long long int *)malloc(N * sizeof(long long int));
	for(long long i=0; i<N; i++) weight[i]=0;
	
	long long int i=0;
	long long int j=0;
	
	while(queueHead != NULL){
		
		s = queueHead->number;
		queueHead = queueHead->next;
		struct node *head = graph->array[s].head;
		
		while(head != NULL){
			
			if(!visited[head->dest]){
				
				visited[head->dest] = 1;
				
				ListT* temp = (ListT *)malloc(sizeof(ListT));
				temp->number = head->dest;
				temp->next = NULL;
				
				if(queueHead == NULL){
					if(weight[j] > head->weight) temp->maxWeight = weight[j];
					else temp->maxWeight = head->weight;
					queueHead = temp;
					queueLast = temp;
				}
				else{
					if(weight[j] > head->weight) temp->maxWeight = weight[j];
					else temp->maxWeight = head->weight;
					queueLast->next = temp;
					queueLast = queueLast->next;
					
				}
				weight[i+1] = queueLast->maxWeight;
				space[src][head->dest] = queueLast->maxWeight;
				
				i++;	
				
			}
			
			head = head->next;
			
		}
		
		j++;
		
	}
	
	
}

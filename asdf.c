
typedef struct node{
    char visited;
    char reachable;
    char queued;
	int id_Node;
    int distance;
    int posHeap;
}t_node;

t_node *dictGraph;

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
int actualMinHeapSize=0;
int minHeapSize=0;
void swap(t_node *a, t_node *b){
    t_node temp=*b;
    *b=*a;
    *a=temp;
}
void maxHeapify(int i){
    if (actualMinHeapSize > 1){
        int largest=i, l=2*i + 1, r=l+1;
        if (l<actualMinHeapSize && dictGraph[l].distance > dictGraph[largest].distance) largest=l;
        if (r<actualMinHeapSize && dictGraph[r].distance > dictGraph[largest].distance) largest=r;
        
        if (largest != i){
            swap(&dictGraph[i], &dictGraph[largest]);
            maxHeapify(largest);
        }
    }
}

char visited;
char reachable;
char queued;
int id_Node;
int distance;
int posHeap;
void maxInsert(int idNewGraph, int sumMinPaths){
	int i;
    // printf("new Insert %d %d",idNewGraph,sumMinPaths);
    if (actualMinHeapSize==0){
        dictGraph[0].id_Node = idNewGraph;
        dictGraph[0].distance = sumMinPaths;
        actualMinHeapSize += 1;
    }else {
        dictGraph[actualMinHeapSize].id_Node = idNewGraph;
        dictGraph[actualMinHeapSize].distance = sumMinPaths;
        actualMinHeapSize += 1;
        for (i=actualMinHeapSize/2-1; i>=0; i--) maxHeapify(i);
    }
}
void maxDeleteRoot(int num){
    int i;
    for (i=0; i<actualMinHeapSize; i++) if (num==dictGraph[i].id_Node) break;                 // O(n) to find
    swap(&dictGraph[0], &dictGraph[actualMinHeapSize-1]);                                     // replace with last 
    actualMinHeapSize--;                                                            // decrease to "delete"
    for (i=actualMinHeapSize/2-1; i>=0; i--) maxHeapify(i);      // O(n log(n))
}
void displayMaxHeapWithDetails(int actualMinHeapSize){
	int i;
    for (i=0; i<actualMinHeapSize; i++) printf("%d(%d) ", dictGraph[i].id_Node, dictGraph[i].distance);
    printf("\n");
}
void displayMaxHeap(int actualMinHeapSize){
	int i;
    if (actualMinHeapSize>0){
        printf("%d", dictGraph[0].id_Node);
        for (i=1; i<actualMinHeapSize; i++) printf(" %d", dictGraph[i].id_Node);
    }
    printf("\n");
}
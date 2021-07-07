#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

char c;
int **graph, sizeGraph;
typedef struct graph{
    int idGraph; //id of the graph to print
    int cost;    //sum of the minimal paths to keep updated the topk
} t_graph;
t_graph *output;
int sizeOutput, currSizeOutput;

int *heapQ,*costs,*color,*trackerQ;
int sizeHeapQ;

// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________

// Max heap for topk

//  log(topK)
void maxHeapifyBotTop_NoRecursion(int i){
    int parent = i>>1;
    t_graph temp;
    while (output[parent].cost < output[i].cost){
        temp = output[parent];
        output[parent] = output[i];
        output[i] = temp;
        i = parent;
        parent = i>>1;
    }
}

//  log(topK)
void maxHeapify_NoRecursion(int i){
    if (currSizeOutput > 1){
        int largest=i, l = (i << 1) + 1, r = l + 1;
        t_graph temp;
        if (l<currSizeOutput && output[l].cost > output[largest].cost) largest=l;
        if (r<currSizeOutput && output[r].cost > output[largest].cost) largest=r;
        while (largest != i){
            temp = output[largest];
            output[largest] = output[i];
            output[i] = temp;
            i = largest;
            l = (i << 1) + 1;
            r = l + 1;
            if (l<currSizeOutput && output[l].cost > output[largest].cost) largest=l;
            if (r<currSizeOutput && output[r].cost > output[largest].cost) largest=r;
        }
    }
}

//  log(topK)
void maxInsert(int k, int sum){
    if(currSizeOutput<sizeOutput){
        output[currSizeOutput].cost = sum;
        output[currSizeOutput].idGraph = k;
        maxHeapifyBotTop_NoRecursion(currSizeOutput);
        currSizeOutput ++;
    }else if(sum<output[0].cost){
        output[0].idGraph = k;
        output[0].cost = sum;
        maxHeapify_NoRecursion(0);
    }
}



// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________

// improvement of disjktra priority queue with minHeap

void swapNode(int *a, int *b){
    int temp=*b;
    *b=*a;
    *a=temp;
}

void minHeapifyBotTop(int i){
    int parent = i>>1;
    if(costs[heapQ[parent]] > costs[heapQ[i]]){
        trackerQ[heapQ[parent]] = i;
        trackerQ[heapQ[i]] = parent;
        swapNode(&heapQ[i], &heapQ[parent]);
        minHeapifyBotTop(parent);
    }
}

void minHeapify(int i){ //log(n)
    if (sizeHeapQ > 0){
        int smallest=i, l=(i<<1) + 1, r=l+1;
        if (l<sizeHeapQ && costs[heapQ[l]] < costs[heapQ[smallest]]) smallest=l;
        if (r<sizeHeapQ && costs[heapQ[r]] < costs[heapQ[smallest]]) smallest=r;
        if (smallest != i){
            trackerQ[heapQ[smallest]] = i;
            trackerQ[heapQ[i]] = smallest;
            swapNode(&heapQ[i], &heapQ[smallest]);
            minHeapify(smallest);
        }
    }
}
void priorityInsert(int node){
    trackerQ[node] = sizeHeapQ;
    heapQ[sizeHeapQ] = node;
    if (sizeHeapQ>0) minHeapifyBotTop(sizeHeapQ);
    sizeHeapQ ++;
}

int popMin(){
    int temp = heapQ[0];
    sizeHeapQ --;
    heapQ[0] = heapQ[sizeHeapQ];
    trackerQ[heapQ[0]] = 0;
    if(sizeHeapQ>0) minHeapify(0);
    return temp;
}


// SPT_Dijkstra (G, r, P, d)
// still O(|nodes|^2), but queue with log(n)
int SPT_Dijkstra_Matrix_Heap(){
    
    int i, j;
    int selected, temp;

    for (i = 1; i < sizeGraph; i++){
        costs[i] = INT_MAX;
        color[i] = 0;
    }

    costs[0] = 0;
    color[0] = 1;
    sizeHeapQ=0;
    priorityInsert(0);
    while (sizeHeapQ>0){
        selected = popMin();
        for (j = 1; j < sizeGraph; j++){
            if (graph[selected][j] > 0 && color[j]<2){ // if connected
                temp = costs[selected] + graph[selected][j];
                if (temp < costs[j]){ // if lower costs
                    costs[j] = temp;
                    if (color[j] == 0){
                        color[j] = 1;
                        priorityInsert(j);
                    }else if (color[j] == 1) minHeapifyBotTop(trackerQ[j]);
                }
            }
        }
        color[selected] = 2;
    }
    temp = 0;
    for (i = 1; i < sizeGraph; i++) if (color[i]) temp += costs[i];
    return temp;
}


// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________

// read from stdin

void displayTopK_List_stdin(){
    int i;
    while (getchar() != '\n');
    if (currSizeOutput > 0){
        printf("%d", output[0].idGraph);
        for (i = 1; i < currSizeOutput; i++) printf(" %d", output[i].idGraph);
    }
    printf("\n");
}

void initNewMatrix_stdin(){
    int i;
    sizeGraph = 0;
    sizeOutput = 0;
    currSizeOutput = 0;
    // retrieve numbers
    while((c=getchar())!=' ') sizeGraph = sizeGraph * 10 + (c-'0');
    while((c=getchar())!='\n') sizeOutput = sizeOutput * 10 + (c-'0');

    // initialize globals
    graph = (int **)malloc(sizeGraph * sizeof(int *));
    for (i = 0; i < sizeGraph; i++) graph[i] = (int *)malloc(sizeGraph * sizeof(int));
    output = (t_graph *)malloc(sizeOutput * sizeof(t_graph));
    costs = (int *)malloc(sizeGraph * sizeof(int));
    color = (int *)malloc(sizeGraph * sizeof(int));
    heapQ = (int *)malloc(sizeGraph * sizeof(int));
    trackerQ = (int *)malloc(sizeGraph * sizeof(int));
}

void main_MaxHeap_MinHeap_stdin(){
    int i, j, k, sum;
    k = 0;
    sum = 0;
    initNewMatrix_stdin();
    while ((c = getchar()) != EOF){
        if (c == 'A'){
            while (getchar() != '\n');
            // read graph
            for (i = 0; i < sizeGraph; i++){
                while (getchar()!=',');
                for (j = 1; j < sizeGraph; j++){
                    if (j==i){
                        while ((c=getchar())!=',' && c!='\n');                        
                        graph[i][j] = 0;
                    }else if((c=getchar())=='0'){
                        c=getchar();
                        graph[i][j] = 0;
                    }else{
                        sum = c-'0';
                        while((c=getchar())!='\n' && c!=',') sum = sum * 10 + (c-'0');
                        graph[i][j] = sum;
                    }
                }
            }
            sum = SPT_Dijkstra_Matrix_Heap();
            maxInsert(k, sum);
            k++;
        }else displayTopK_List_stdin();
    }

}

int main(){
    main_MaxHeap_MinHeap_stdin();
    return 0;
}

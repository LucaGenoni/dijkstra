#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INPUTTXT "../tests/input/file/input_4" 
#define OUTPUTTXT "output"
FILE *standardin, *standardout;

void localSetUp(){
    standardin = fopen(INPUTTXT, "r");
    if (!standardin) standardin = stdin;
    standardout = fopen(OUTPUTTXT, "r");
    if (standardin){
        fclose(standardout);
        standardout = fopen(OUTPUTTXT, "w");
    }else standardout = stdout;
}

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

void init(){
    int i;
    if (fscanf(standardin, "%d %d", &sizeGraph, &sizeOutput) == EOF) return;
    c = getc(standardin);
    // depending on the sizeGraph prefer a dijkstra implementetion over another
    graph = (int **)malloc(sizeGraph * sizeof(int *));
    for (i = 0; i < sizeGraph; i++) graph[i] = (int *)malloc(sizeGraph * sizeof(int));
    costs = (int *)malloc(sizeGraph * sizeof(int));

    // depending on the sizeOutput prefer a topK implementetion over another
    output = (t_graph *)malloc(sizeOutput * sizeof(t_graph));
    currSizeOutput = 0;
}

void displayMatrix(FILE *std){
    int i, j;
    fprintf(std, "\nGraph:\n");
    for (i = 0; i < sizeGraph; i++){
        for (j = 0; j < sizeGraph; j++) fprintf(std, "%d\t", graph[i][j]);
        fprintf(std, "\n");
    }
}

void displayTopK_List(){
    int i;
    while (getc(standardin) != '\n');
    if (currSizeOutput > 0){
        printf("%d", output[0].idGraph);
        for (i = 1; i < currSizeOutput; i++) printf(" %d", output[i].idGraph);
    }
    printf("\n");
}

void displayTopK_List_Detailed(){
    int i;
    while (getc(standardin) != '\n');
    if (currSizeOutput > 0){
        printf("%d(%d)", output[0].idGraph,output[0].cost);
        for (i = 1; i < currSizeOutput; i++) printf(" %d(%d)", output[i].idGraph,output[i].cost);
    }
    printf("\n");
}

void displayArray(FILE *std,int *array,int size){
    int i;
    if (size > 0){
        fprintf(std,"'%d", array[0]);
        for (i = 1; i < size; i++) fprintf(std," %d", array[i]);
    }
    fprintf(std,"'\n");
}
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________
// __________________________________________________________________________________________________________

// basic solution

// O(|nodes|)
int minQueue_list(int *queue,int current, int sizeQueue, int *cost){
    int selected,temp,k;
    selected = current;
    k = current + 1;
    if (k < sizeQueue){
        for (; k < sizeQueue; k++) if (cost[queue[k]] < cost[queue[selected]]) selected = k;
        temp = queue[current];
        queue[current] = queue[selected];
        queue[selected] = temp;
    }
    return queue[current];
}


// O(topK)
void addTopK_list(int k, int sum){
    int i_max, i;
    if (currSizeOutput < sizeOutput){
        output[currSizeOutput].cost = sum;
        output[currSizeOutput].idGraph = k;
        currSizeOutput++;
    }else{
        i_max = 0;
        // select the minimum
        for (i = 1; i < sizeOutput; i++) if (output[i_max].cost < output[i].cost) i_max = i;
        if (sum < output[i_max].cost){
            output[i_max].cost = sum;
            output[i_max].idGraph = k;
        }
    }
}


// SPT_Dijkstra (G, r, P, d)
// O(|nodes|^2)
int SPT_Dijkstra_Matrix_List(){
    
    int i, j;
    int sizeQueue, selected, temp;
    int toQueue[sizeGraph], queue[sizeGraph];

    for (i = 1; i < sizeGraph; i++){
        costs[i] = INT_MAX;
        toQueue[i] = 1;
    }
    i = 0;
    costs[i] = 0;

    toQueue[i] = 0;
    queue[i] = 0;
    sizeQueue = 1;

    while (i < sizeQueue){
        selected = minQueue_list(queue,i,sizeQueue,costs);
        i++;
        for (j = 1; j < sizeGraph; j++){
            if (graph[selected][j] > 0){ // if connected
                temp = costs[selected] + graph[selected][j];
                if (temp < costs[j]){ // if lower costs
                    costs[j] = temp;
                    if (toQueue[j]){
                        toQueue[j] = 0;
                        queue[sizeQueue] = j;
                        sizeQueue++;
                    }
                }
            }
        }
    }
    if (sizeQueue == 1) return 0;
    temp = 0;
    for (i = 1; i < sizeGraph; i++) if (!toQueue[i]) temp += costs[i];
    return temp;
}

void mainEasy(){
    init();
    int i, j, k, sum, errorkeeper = 0;    
    k = errorkeeper;
    while ((c = getc(standardin)) != EOF){
        if (c == 'A'){
            while (getc(standardin) != '\n');
            for (i = 0; i < sizeGraph; i++)
                for (j = 0; j < sizeGraph; j++){
                    errorkeeper = fscanf(standardin, "%d", &sum);
                    c = getc(standardin);
                    if (j != i) graph[i][j] = sum;
                    else graph[i][j] = 0;
                }
            sum = SPT_Dijkstra_Matrix_List();
            addTopK_list(k, sum);
            k++;
        }else displayTopK_List();
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

// improvement of TOPK

void swapGraph(t_graph *a, t_graph *b){
    t_graph temp=*b;
    *b=*a;
    *a=temp;
}

// log(topK)
void maxHeapifyBotTop(int i){
    int parent = i>>1;
    if(output[parent].cost < output[i].cost){
        swapGraph(&output[i], &output[parent]);
        maxHeapifyBotTop(parent);
    }
}
// log(topK)
void maxHeapify(int i){ 
    if (currSizeOutput > 1){
        int largest=i, l=(i<<1) + 1, r=l+1;
        if (l<currSizeOutput && output[l].cost > output[largest].cost) largest=l;
        if (r<currSizeOutput && output[r].cost > output[largest].cost) largest=r;
        if (largest != i){
            swapGraph(&output[i], &output[largest]);
            maxHeapify(largest);
        }
    }
}

//  remove recursion 
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

//  remove recursion 
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

//  no recursion 
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


void topK(){
    init();
    int i, j, k, sum, errorkeeper = 0;    
    k = errorkeeper;
    while ((c = getc(standardin)) != EOF){
        if (c == 'A'){
            while (getc(standardin) != '\n');
            for (i = 0; i < sizeGraph; i++)
                for (j = 0; j < sizeGraph; j++){
                    errorkeeper = fscanf(standardin, "%d", &sum);
                    c = getc(standardin);
                    if (j != i) graph[i][j] = sum;
                    else graph[i][j] = 0;
                }
            sum = SPT_Dijkstra_Matrix_List();
            maxInsert(k, sum);
            k++;
        }else displayTopK_List();
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

// improvement of disjktra priority queue

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

void mainDijkstraHeap(){
    init();
    color = (int *)malloc(sizeGraph * sizeof(int));
    heapQ = (int *)malloc(sizeGraph * sizeof(int));
    trackerQ = (int *)malloc(sizeGraph * sizeof(int));

    int i, j, k, sum, errorkeeper = 0;    
    k = errorkeeper;
    while ((c = getc(standardin)) != EOF){
        if (c == 'A'){
            while (getc(standardin) != '\n');
            for (i = 0; i < sizeGraph; i++)
                for (j = 0; j < sizeGraph; j++){
                    errorkeeper = fscanf(standardin, "%d", &sum);
                    c = getc(standardin);
                    if (j != i) graph[i][j] = sum;
                    else graph[i][j] = 0;
                }
            sum = SPT_Dijkstra_Matrix_Heap();
            maxInsert(k, sum);
            k++;
        }else displayTopK_List();
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

// improvement of matrix that store the costs arcs

// new method to store the graph
t_graph **graph_OnlyArcs;
void initNewMatrix(){
    int i;
    if (fscanf(standardin, "%d %d", &sizeGraph, &sizeOutput) == EOF) return;
    c = getc(standardin);
    // depending on the sizeGraph prefer a dijkstra implementetion over another
    graph_OnlyArcs = (t_graph **)malloc(sizeGraph * sizeof(t_graph *));
    for (i = 0; i < sizeGraph; i++) graph_OnlyArcs[i] = (t_graph *)malloc(sizeGraph * sizeof(t_graph));
    costs = (int *)malloc(sizeGraph * sizeof(int));

    // depending on the sizeOutput prefer a topK implementetion over another
    output = (t_graph *)malloc(sizeOutput * sizeof(t_graph));
    currSizeOutput = 0;
}

void displayMatrix_OnlyArcs(FILE *std){
    int i, j;
    fprintf(std, "\nGraph:\n");
    for (i = 0; i < sizeGraph; i++){
        for (j = 0; graph_OnlyArcs[i][j].idGraph>0; j++) fprintf(std, "%d(%d)\t", graph_OnlyArcs[i][j].idGraph, graph_OnlyArcs[i][j].cost);
        fprintf(std, "\n");
    }
};
int SPT_Dijkstra_Matrix_OnlyArcs_List(){
    int i, j, k;
    int sizeQueue, selected, temp;
    int toQueue[sizeGraph], queue[sizeGraph];

    costs[0] = 0;
    toQueue[0] = 0;
    queue[0] = 0;
    sizeQueue = 1;

    for (i = 1; i < sizeGraph; i++){
        costs[i] = INT_MAX;
        toQueue[i] = 1;
    }
    i = 0;
    while (i < sizeQueue){
        selected = minQueue_list(queue,i,sizeQueue,costs);
        i++;
        for (j = 0; graph_OnlyArcs[selected][j].idGraph; j++){
            temp = costs[selected] + graph_OnlyArcs[selected][j].cost;
            k = graph_OnlyArcs[selected][j].idGraph;
            if (temp < costs[k]){ // if lower costs
                costs[k] = temp;
                if (toQueue[k]){
                    toQueue[k] = 0;
                    queue[sizeQueue] = k;
                    sizeQueue++;
                }
            }
        }
    }
    if (sizeQueue == 1) return 0;
    temp = 0;
    for (i = 1; i < sizeGraph; i++) if (!toQueue[i]) temp += costs[i];
    return temp;
}

void main_MaxList_MinList_OnlyArcs(){
    initNewMatrix();
    int i, j, k, z, sum, errorkeeper = 0;    
    k = errorkeeper;
    while ((c = getc(standardin)) != EOF){
        if (c == 'A'){
            while (getc(standardin) != '\n');
            for (i = 0; i < sizeGraph; i++){
                z = 0;
                while ((c=getc(standardin))!=',');
                for (j = 1; j < sizeGraph; j++){
                    errorkeeper = fscanf(standardin, "%d", &sum);
                    c = getc(standardin);
                    if (j != i && sum!=0 ) {
                        graph_OnlyArcs[i][z].idGraph = j;
                        graph_OnlyArcs[i][z].cost = sum;
                        z++;
                    }
                }
                graph_OnlyArcs[i][z].idGraph = 0;
            }
            sum = SPT_Dijkstra_Matrix_OnlyArcs_List();
            addTopK_list(k, sum);
            k++;
        }else displayTopK_List();
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

// Upgrades: topk, disjktra heap, only arcs, directly from stdin 
void displayTopK_List_stdin(){
    int i;
    while (getchar() != '\n');
    if (currSizeOutput > 0){
        printf("%d", output[0].idGraph);
        for (i = 1; i < currSizeOutput; i++) printf(" %d", output[i].idGraph);
    }
    printf("\n");
}
void initNewMatrix_onlyArcs_stdin(){
    int i;
    sizeGraph = 0;
    sizeOutput = 0;
    while((c=getchar())!=' ') sizeGraph = sizeGraph * 10 + (c-'0');
    while((c=getchar())!='\n') sizeOutput = sizeOutput * 10 + (c-'0');
    // depending on the sizeGraph prefer a dijkstra implementetion over another
    graph_OnlyArcs = (t_graph **)malloc(sizeGraph * sizeof(t_graph *));
    for (i = 0; i < sizeGraph; i++) graph_OnlyArcs[i] = (t_graph *)malloc(sizeGraph * sizeof(t_graph));
    costs = (int *)malloc(sizeGraph * sizeof(int));

    // depending on the sizeOutput prefer a topK implementetion over another
    output = (t_graph *)malloc(sizeOutput * sizeof(t_graph));
    currSizeOutput = 0;
}

void main_MaxHeap_List_OnlyArcs_stdin(){
    initNewMatrix_onlyArcs_stdin();
    int i, j, k, z, sum;
    char numkeeper[30];
    k = 0;
    sum = 0;
    while ((c = getchar()) != EOF){
        if (c == 'A'){
            while (getchar() != '\n');
            for (i = 0; i < sizeGraph; i++){
                z = 0;
                while ((c=getchar())!=',');
                for (j=1;c!='\n';j++){
                    if (j==i) while ((c=getchar())!=',' && c!='\n');
                    else {
                        sum = -1;
                        while((numkeeper[++sum]=getchar())!=',' && numkeeper[sum]!='\n');
                        if (numkeeper[0]!='0'){
                            graph_OnlyArcs[i][z].idGraph = j;
                            graph_OnlyArcs[i][z].cost = atoi(numkeeper);
                            z++;
                        }
                        c = numkeeper[sum];
                    }
                }
                graph_OnlyArcs[i][z].idGraph = 0;
            }
            sum = SPT_Dijkstra_Matrix_OnlyArcs_List();
            maxInsert(k, sum);
            k++;
        }else displayTopK_List_stdin();
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

// Upgrades: topk, disjktra heap, only arcs, directly from stdin 

// SPT_Dijkstra (G, r, P, d)
// still O(|nodes|^2), but queue with log(n)
int SPT_Dijkstra_Matrix_OnlyArcs_Heap(){
    int i, j, k;
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
        for (j = 0; graph_OnlyArcs[selected][j].idGraph; j++){
            temp = costs[selected] + graph_OnlyArcs[selected][j].cost;
            k = graph_OnlyArcs[selected][j].idGraph;
            if (temp < costs[k]){ // if lower costs
                costs[k] = temp;
                if (color[k] == 0){
                    color[k] = 1;
                    priorityInsert(k);
                }else if (color[k] == 1) minHeapifyBotTop(trackerQ[k]);
            }
        }
        color[selected] = 2;
    }
    temp = 0;
    for (i = 1; i < sizeGraph; i++) if (color[i]) temp += costs[i];
    return temp;
}

void main_MaxHeap_MinHeap_OnlyArcs_stdin(){
    initNewMatrix_onlyArcs_stdin();
    color = (int *)malloc(sizeGraph * sizeof(int));
    heapQ = (int *)malloc(sizeGraph * sizeof(int));
    trackerQ = (int *)malloc(sizeGraph * sizeof(int));

    int i, j, k, z, sum;
    char numkeeper[30];
    k = 0;
    sum = 0;
    while ((c = getchar()) != EOF){
        if (c == 'A'){
            while (getchar() != '\n');
            for (i = 0; i < sizeGraph; i++){
                z = 0;
                while ((c=getchar())!=',');
                for (j=1;c!='\n';j++){
                    if (j==i) while ((c=getchar())!=',' && c!='\n');
                    else {
                        sum = -1;
                        while((numkeeper[++sum]=getchar())!=',' && numkeeper[sum]!='\n');
                        if (numkeeper[0]!='0'){
                            graph_OnlyArcs[i][z].idGraph = j;
                            graph_OnlyArcs[i][z].cost = atoi(numkeeper);
                            z++;
                        }
                        c = numkeeper[sum];
                    }
                }
                graph_OnlyArcs[i][z].idGraph = 0;
            }
            sum = SPT_Dijkstra_Matrix_OnlyArcs_Heap();
            maxInsert(k, sum);
            k++;
        }else displayTopK_List_stdin();
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


void initNewMatrix_stdin(){
    int i;
    sizeGraph = 0;
    sizeOutput = 0;
    currSizeOutput = 0;

    while((c=getchar())!=' ') sizeGraph = sizeGraph * 10 + (c-'0');
    while((c=getchar())!='\n') sizeOutput = sizeOutput * 10 + (c-'0');

    graph = (int **)malloc(sizeGraph * sizeof(int *));
    for (i = 0; i < sizeGraph; i++) graph[i] = (int *)malloc(sizeGraph * sizeof(int));

    output = (t_graph *)malloc(sizeOutput * sizeof(t_graph));

    costs = (int *)malloc(sizeGraph * sizeof(int));
}

void main_MaxHeap_MinHeap_stdin(){
    initNewMatrix_stdin();
    color = (int *)malloc(sizeGraph * sizeof(int));
    heapQ = (int *)malloc(sizeGraph * sizeof(int));
    trackerQ = (int *)malloc(sizeGraph * sizeof(int));

    int i, j, k, sum;
    k = 0;
    sum = 0;
    while ((c = getchar()) != EOF){
        if (c == 'A'){
            while (getchar() != '\n');
            
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
    // standardin = stdin;
    // standardout = stdout;
    // localSetUp();

    main_MaxHeap_MinHeap_stdin();
    return 0;
}

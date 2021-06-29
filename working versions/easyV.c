#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define SIZEBUFFER 2000
#define INPUTTXT "../tests/input/file/input_5"
#define OUTPUTTXT ""
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

char bInput[SIZEBUFFER], c;
int **graph, sizeGraph;
typedef struct graph{
    int idGraph; //id of the graph to print
    int cost;    //sum of the minimal paths to keep updated the topk
} t_graph;
t_graph *output;
int sizeOutput, currSizeOutput;

void init(){
    int i;
    if (fscanf(standardin, "%d %d", &sizeGraph, &sizeOutput) == EOF) return;
    c = getc(standardin);
    // depending on the sizeGraph prefer a dijkstra implementetion over another
    graph = (int **)malloc(sizeGraph * sizeof(int *));
    for (i = 0; i < sizeGraph; i++) graph[i] = (int *)malloc(sizeGraph * sizeof(int));

    // depending on the sizeOutput prefer a topK implementetion over another
    output = (t_graph *)malloc(sizeOutput * sizeof(t_graph));
    currSizeOutput = 0;

}

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


// SPT_Dijkstra (G, r, P, d)
// O(|nodes|^2)
int SPT_Dijkstra_Matrix_List(){
    int i, j, k;
    int sizeQueue, selected, temp;
    int cost[sizeGraph], toQueue[sizeGraph], queue[sizeGraph];

    for (i = 1; i < sizeGraph; i++){
        cost[i] = INT_MAX;
        toQueue[i] = 1;
    }
    i = 0;
    cost[i] = 0;
    toQueue[i] = 0;
    queue[i] = 0;
    sizeQueue = 1;

    while (i < sizeQueue){
        selected = minQueue_list(queue,i,sizeQueue,cost);
        i++;
        for (j = 1; j < sizeGraph; j++){
            if (graph[selected][j] > 0){ // if connected
                temp = cost[selected] + graph[selected][j];
                if (temp < cost[j]){ // if lower cost
                    cost[j] = temp;
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
    for (i = 1; i < sizeGraph; i++) if (!toQueue[i]) temp += cost[i];
    return temp;
}

// O(topK)
void displayTopK_List(){
    int i;
    while (getc(standardin) != '\n');
    if (currSizeOutput > 0){
        printf("%d", output[0].idGraph);
        for (i = 1; i < currSizeOutput; i++) printf(" %d", output[i].idGraph);
    }
    printf("\n");
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

void displayMatrix(FILE *std){
    int i, j;
    fprintf(std, "\nGraph:\n");
    for (i = 0; i < sizeGraph; i++){
        for (j = 0; j < sizeGraph; j++) fprintf(std, "%d\t", graph[i][j]);
        fprintf(std, "\n");
    }
};

int main(){
    int i, j, k, sum, errorkeeper = 0;
    standardin = stdin;
    standardout = stdout;
    localSetUp();
    init();

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
    return 0;
}

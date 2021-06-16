
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define SIZEBUFFER 10
#define SIZEBUFFER_1 SIZEBUFFER-1

#define INPUTTXT "tests/input/file/input_2"
#define OUTPUTTXT "tests/output2"
FILE *standardin,*standardout;
// ////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct line{
    char isEnd;
    char buffer[SIZEBUFFER];
    struct line *next;
}t_line;

typedef struct node{
    char visited;
    char reachable;
    char queued;
	int id_Node;
    int distance;
    int posHeap;
    t_line *arcs;
}t_node;

typedef struct graph{
    int id_graph;
    int cost;
}t_graph;
// _______________________________________________________________________________________________________________
char buffer[SIZEBUFFER], *trash;
// variable for dijsktra
int sizeGraph;
t_node *dictGraph;
char computeDijkstra;
int *queue;

// variables for TopK
int sizeOut, maxSizeOut;
t_graph *finalOut;
char c;

// ---------------------------------------------------------------------------------------------------------------
void display(t_node node){
    t_line *temp;
    printf("%10d-%10d(%10d) ",node.id_Node,node.distance,node.posHeap);
    printf("[v:%1c,r:%1c,q:%1c] ",node.visited,node.reachable,node.queued);
    if (node.visited=='f') 
        for(temp=node.arcs; temp!= NULL;temp=temp->next){
            printf(">%s",temp->buffer);
            if(temp->isEnd=='t') break;
        }
    printf("\n");
}
void displayArc(t_line line);
void displayGraph(){
    int i;
    printf("\nGraph: ");
    for(i=0;i<sizeGraph;i++) display(dictGraph[i]);
}
void dimensions(){
    printf("\nSize node: %d",sizeof(t_node));
    printf("\nSize line: %d",sizeof(t_line));
    printf("\n");
}

void initParsing();
void storeLine(int);


void parsingFirstLine(){
    int i,distrance,i_node;
    c=getc(standardin);
    i_node=1;
    while ((c=getc(standardin))!=',' && c!='\n'); // skip reading node 0
    while(c!='\n'){ //if space continue reading
        c=getc(standardin); //read first most significant cifra
        if(c=='0'){    
            dictGraph[i_node].distance=0;
            dictGraph[i_node].reachable='f';
            dictGraph[i_node].queued='f';
            dictGraph[i_node].visited='f';
            dictGraph[i_node].id_Node=i_node;
            c=getc(standardin); // read space or newline
        }else{
            buffer[0]=c;
            for(i=1;(c=getc(standardin))!=',' && c!='\n';i++) buffer[i]=c;
            buffer[i]='\0';
            dictGraph[i_node].distance=atoi(buffer);
            dictGraph[i_node].reachable='t';
            dictGraph[i_node].queued='t';
            dictGraph[i_node].visited='f';
            dictGraph[i_node].id_Node=i_node;
            // TODO : add to heap queue
        }
        i_node++;
    }
};

// ************************************************************************************************************

// Max-Heap data structure in C

int actualMaxHeapSize=0;
int maxHeapSize=0;
void swap(t_graph *a, t_graph *b){
    t_graph temp=*b;
    *b=*a;
    *a=temp;
}
void maxHeapify(int i){
    if (actualMaxHeapSize > 1){
        int largest=i, l=2*i + 1, r=l+1;
        if (l<actualMaxHeapSize && finalOut[l].cost > finalOut[largest].cost) largest=l;
        if (r<actualMaxHeapSize && finalOut[r].cost > finalOut[largest].cost) largest=r;
        
        if (largest != i){
            swap(&finalOut[i], &finalOut[largest]);
            maxHeapify(largest);
        }
    }
}
void maxInsert(int idNewGraph, int sumMinPaths){
	int i;
    // printf("new Insert %d %d",idNewGraph,sumMinPaths);
    if (actualMaxHeapSize==0){
        finalOut[0].id_graph = idNewGraph;
        finalOut[0].cost = sumMinPaths;
        actualMaxHeapSize += 1;
    }else if(actualMaxHeapSize<maxHeapSize){
        finalOut[actualMaxHeapSize].id_graph = idNewGraph;
        finalOut[actualMaxHeapSize].cost = sumMinPaths;
        actualMaxHeapSize += 1;
        for (i=actualMaxHeapSize/2-1; i>=0; i--) maxHeapify(i);
    }else if(sumMinPaths<finalOut[0].cost){
        maxDeleteRoot();                                            // n log(n) too long...
        finalOut[actualMaxHeapSize].id_graph = idNewGraph;
        finalOut[actualMaxHeapSize].cost = sumMinPaths;
        actualMaxHeapSize += 1;
        for (i=actualMaxHeapSize/2-1; i>=0; i--) maxHeapify(i);
    }
}
void maxDeleteRoot(){
    int i;
    // for (i=0; i<actualMaxHeapSize; i++) if (num==finalOut[i]) break;                 // O(n) to find
    swap(&finalOut[0], &finalOut[actualMaxHeapSize-1]);                                     // replace with last 
    actualMaxHeapSize--;                                                            // decrease to "delete"
    for (i=actualMaxHeapSize/2-1; i>=0; i--) maxHeapify(i);      // O(n log(n))
}
void displayMaxHeapWithDetails(int actualMaxHeapSize){
	int i;
    for (i=0; i<actualMaxHeapSize; i++) printf("%d(%d) ", finalOut[i].id_graph, finalOut[i].cost);
    printf("\n");
}
void displayMaxHeap(int actualMaxHeapSize){
	int i;
    if (actualMaxHeapSize>0){
        printf("%d", finalOut[0].id_graph);
        for (i=1; i<actualMaxHeapSize; i++) printf(" %d", finalOut[i].id_graph);
    }
    printf("\n");
}
int main(){
    finalOut=malloc (sizeof(t_graph)*4);
    maxHeapSize= 4;
    maxInsert(1,3);
    maxInsert(2,4);
    maxInsert(3,9);
    maxInsert(4,5);
    maxInsert(5,2);

    printf("Max-Heap heap: ");
    displayMaxHeapWithDetails(actualMaxHeapSize);

    maxDeleteRoot(9);

    printf("After deleting an element: ");

    displayMaxHeapWithDetails(actualMaxHeapSize);
    displayMaxHeap(actualMaxHeapSize);
}














// ************************************************************************************************************
// inspired by a state machine
// 
// initial phase 0-1
// 0 -'\d+'->1 (read a number)
// 1 -' '-> 0 (read another number)
// 
// go to final state 2
// 1 -'\n'-> 2
// 2 <-"TopK\n"-> 2 
// 2 -"AggiungiGrafo\n"-> 3
// 2 -'\d+'-> 4 (read cost arcs of another node, increase id_node)
// parsing of arc costs 
// 3 -'\d+'-> 4 (read a number)
// 4 -' '-> 3 (read another number)
// 4 -'\n'-> 2 (line complete try to go on final state)

void localSetUp(){    
    standardin=fopen(INPUTTXT,"r");
    if(!standardin) standardin= stdin;
    standardout=fopen(OUTPUTTXT,"r");
    if(standardin) {
        fclose(standardout);
        standardout=fopen(OUTPUTTXT,"w");
    }else standardout=stdout;
}
void dijkstra(){
    if (computeDijkstra==1){
        // final computation of dijkstra
        // final sum of dijkstra
        // displayGraph();
        // append infos on topKList
        if (sizeOut<maxSizeOut){
            // simple maxInsert
        }else{
            // check if smaller, if so pop and maxInsert
        }
    }
}
// int main(){
//     int arc_node=0;
//     int number_graph=-1;
//     standardin=stdin;
//     standardout=stdout;
//     computeDijkstra=0;
//     // TODO remove before update
//     localSetUp();
//     dimensions();
//     // states 0,1
// 	initParsing();
//     //check if final state
//     while ( (c=getc(standardin)) != EOF){ 
//         if (c=='A'){
//             dijkstra();
            
//             // prepare to read a new graph
//             number_graph++;
//             arc_node=0;
//             computeDijkstra=1;
//             while (getc(standardin)!='\n'); //clearline
//             printf("\nRead graph %d:\n",number_graph);
//             parsingFirstLine();
            
//             // trash=fgets(buffer, SIZEBUFFER, standardin);
//             // parsing the node 0
//             // store 
//             // read and process disjktra
//         }else if (c=='T'){            
//             if (computeDijkstra==1){
//                 // final sum of dijkstra
//                 // displayGraph();
//                 // append infos on topKList
//                 computeDijkstra=0;
//             }
//             while ((getc(standardin))!='\n');
//             printf("\ntopk\n");
//             // print topKList

//         }else{
//             // if node is the minimum run one iteration of dijkstra
//             // else store in the information of the node in a buffer to parse if needed.
//             arc_node++;
//             if (queue[0]==arc_node){
//                 printf("\nDirect expansion of node %d: ",arc_node);
//             }else{
//                 printf("\nStore node %d: ",arc_node);
//                 storeLine(arc_node);
//             }

//         };
//     }
//     printf("End of file reached");
// }

void storeLine(int arc_node){
    int i;
    t_line *temp;
    if((dictGraph[arc_node].arcs)==NULL){
        dictGraph[arc_node].arcs=malloc(sizeof(t_line));
        dictGraph[arc_node].arcs->next=NULL;
    }
    for (i=0;i<SIZEBUFFER_1 && c!='\n';i++){
        dictGraph[arc_node].arcs->buffer[i]=c;
        c=getc(standardin);
    }
    dictGraph[arc_node].arcs->buffer[i] ='\0';
    temp=dictGraph[arc_node].arcs;
    while (c!='\n'){
        if (temp->next==NULL){
            temp->next=malloc(sizeof(t_line));
            temp->next->next=NULL;    
        }
        temp->isEnd='f';
        temp=temp->next;
        for (i=0;i<SIZEBUFFER_1 && c!='\n';i++){
            temp->buffer[i]=c;
            c=getc(standardin);
        }
        temp->buffer[i] ='\0';
    }
    temp->isEnd='t';
};
void initParsing(){
    int i;
    
    trash=fgets(buffer, SIZEBUFFER, standardin);    
    for (i=-1;*(buffer+(++i))!=' ';);
    buffer[i]='\0';
    sizeGraph=atoi(buffer);
    maxSizeOut=atoi(buffer+(++i));   
    sizeOut=0; 
    printf("%d, %d\n",sizeGraph,maxSizeOut);
    dictGraph=malloc (sizeGraph*sizeof(t_node));
    queue=malloc (sizeGraph*sizeof(int));
    for (i=0;i<sizeGraph;i++) dictGraph[i].arcs=NULL;
    finalOut=malloc (maxSizeOut*sizeof(t_graph));
}

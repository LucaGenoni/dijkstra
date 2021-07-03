
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define SIZEBUFFER 2000
#define SIZEBUFFER_1 SIZEBUFFER-1

#define INPUTTXT "tests/input/file/input_2"
#define OUTPUTTXT "tests/output2"
FILE *standardin,*standardout;
// ////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct line{
    char isEnd;                 // boolean 't' if it's the last line of the arcs of the headNode of the current graph
    char buffer[SIZEBUFFER];    // buffer to store the line with an '\0' to end the buffer
    struct line *next;          // pointer to the next lines in case isEnd='f'
}t_line;

typedef struct node{
	int label,cost;
	struct node *next;
}t_node;

typedef struct headNode{
    char isVisited;     //boolean 't' if already expanded by dijkstra, otherwise 'f'
    char isReachable;   //boolean 't' if the headNode is reachable from 0, otherwise 'f'
	int idNode;         //store the id of the headNode for print purposes
    int distance;       //store the distance from 0 to this headNode
    int posHeap;        //store the position on the heap for direct access
    int size;
	t_node *first,*last;
    t_line *arcs;       //store the line of the arcs
}t_headNode;



typedef struct graph{
    int idGraph;       //id of the graph to print
    int cost;           //sum of the minimal paths to keep updated the topk
}t_graph;
// _______________________________________________________________________________________________________________
// _______________________________________________________________________________________________________________
// _______________________________________________________________________________________________________________
// _______________________________________________________________________________________________________________
// _______________________________________________________________________________________________________________

char buffer[SIZEBUFFER], *trash;
// variable for dijsktra
int sizeGraph;
t_headNode *dictGraph;
int sizeGraphQueue;
int *queue;
char addGraphToTopK;

// variables for TopK
int sizeOut, maxSizeOut;
t_graph *finalOut;
char c;

// modified push, allow overwrite to skip repeated malloc 
// space worst case... worse than an adjacent matrix
// time worst case, should be better than adjacent matrix
void pushArc(int src, int dest,int cost){
	if (dictGraph[src].last == NULL){ 
		//first element of the list
		dictGraph[src].last = dictGraph[src].first;
	}else if (dictGraph[src].last->next == NULL){
		//new element need to be allocated to be added.
		dictGraph[src].last->next = malloc (sizeof(t_node));
		dictGraph[src].last = dictGraph[src].last->next;
		dictGraph[src].last->next = NULL;
	}else dictGraph[src].last = dictGraph[src].last->next;
	// overwrite the new values
	dictGraph[src].last->label = dest;
	dictGraph[src].last->cost = cost;
	dictGraph[src].size++;

	return;
}
// ---------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------

void displayArc(t_node *this){
	// printf("%3d(%5d)  <%6x-%6x>, ",this->label,this->cost,this,(this->next));
	fprintf(standardout,"%3d(%5d), ",this->label,this->cost);
	return;
}
void display(FILE *sdt,t_headNode headNode){
//    t_line *temp;
    t_node *temp;
    char asd;
    int i;
    fprintf(sdt,"%5d(%10d)[%2d] ",headNode.idNode,headNode.distance,headNode.posHeap);
    fprintf(sdt,"[v:%1c,r:%1c] ",headNode.isVisited,headNode.isReachable);
    if (headNode.last!=NULL){
        for(temp = headNode.first; temp != NULL && headNode.last!=temp; temp = temp->next) displayArc(temp);
		displayArc(temp); // last element
    }
        // for(temp=headNode.arcs; temp!= NULL;temp=temp->next){
        //     ;
        //     for(i=0;(asd = temp->buffer[i])!='\n' && asd!='\0';i++) fprintf(sdt,"%c",asd);
        //     if(temp->isEnd=='t') break;
        // }
    fprintf(sdt,"\n");
}
void displayGraph(FILE *sdt){
    int i;
    fprintf(sdt,"\n");
    for(i=0;i<sizeGraph;i++) display(sdt,dictGraph[i]);
}
void dimensions(){
    printf("\nSize headNode: %d",sizeof(t_headNode));
    printf("\nSize line: %d",sizeof(t_line));
    printf("\n");
}

void initParsing();
void storeLine(int);
void parsingFirstLine();
void storeLineOnIncidence(int);

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// Max-Heap data structure in C

int sizeOut=0;
int maxSizeOut=0;
void swapGraph(t_graph *a, t_graph *b){
    t_graph temp=*b;
    *b=*a;
    *a=temp;
}

void maxHeapifyBotTop(int i){ // log(n)
    int parent = i/2;
    if(finalOut[parent].cost < finalOut[i].cost){
        swapGraph(&finalOut[i], &finalOut[parent]);
        maxHeapifyBotTop(parent);
    }
}

void maxHeapify(int i){ // log(n)
    if (sizeOut > 1){
        int largest=i, l=2*i + 1, r=l+1;
        if (l<sizeOut && finalOut[l].cost > finalOut[largest].cost) largest=l;
        if (r<sizeOut && finalOut[r].cost > finalOut[largest].cost) largest=r;
        if (largest != i){
            swapGraph(&finalOut[i], &finalOut[largest]);
            maxHeapify(largest);
        }
    }
}

t_graph maxPop(){ // log(n)
    sizeOut--;
    swapGraph(&finalOut[0], &finalOut[sizeOut]);
    if (sizeOut>0) maxHeapify(0);   // log(n)
    return finalOut[sizeOut];
}

int maxPopInsert(int idNewGraph, int sumMinPaths){ //log(n)
    finalOut[0].cost = sumMinPaths;
    finalOut[0].idGraph = idNewGraph;
    maxHeapify(0);  // log(n)
}

void maxInsert(int idNewGraph, int sumMinPaths){
	int i;
    // printf("new Insert %d %d",idNewGraph,sumMinPaths);
    if (sizeOut==0){
        finalOut[0].idGraph = idNewGraph;
        finalOut[0].cost = sumMinPaths;
        sizeOut += 1;
    }else if(sizeOut<maxSizeOut){
        finalOut[sizeOut].idGraph = idNewGraph;
        finalOut[sizeOut].cost = sumMinPaths;
        sizeOut += 1;
        maxHeapifyBotTop(sizeOut);
    }else if(sumMinPaths<finalOut[0].cost){
        finalOut[0].cost = sumMinPaths;
        finalOut[0].idGraph = idNewGraph;
        maxHeapify(0);
    }
}
void displayMaxHeapWithDetails(){
	int i;
    for (i=0; i<sizeOut; i++) printf("%d(%d) ", finalOut[i].idGraph, finalOut[i].cost);
    printf("\n");
}
void displayMaxHeap(){
	int i;
    if (sizeOut>0){
        printf("%d", finalOut[0].idGraph);
        for (i=1; i<sizeOut; i++) printf(" %d", finalOut[i].idGraph);
    }
    printf("\n");
}

void appendLastDijkstra(int idGraph){
    int i,sum= 0;
    if (addGraphToTopK==1){
        for (i = 0; i < sizeGraph; i++){
            sum += dictGraph[i].distance;
        }
        maxInsert(idGraph,sum);
        // fprintf("\nFinal graph:");
        fprintf(stdin,"%d - cost: %d\n",idGraph, sum);
        displayGraph(standardout);
    }
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void swapNode(int *a, int *b){
    int temp=*b;
    *b=*a;
    *a=temp;
}
void minHeapifyBotTop(int i){
    int parent = i/2;
    if(dictGraph[queue[parent]].distance > dictGraph[queue[i]].distance){
        dictGraph[queue[i]].posHeap = parent;
        dictGraph[queue[parent]].posHeap = i;
        swapNode(&queue[i], &queue[parent]);
        minHeapifyBotTop(parent);
    }
}

void minHeapify(int i){ //log(n)
    if (sizeGraphQueue > 1){
        int smallest=i, l=2*i + 1, r=l+1;
        if (l<sizeGraphQueue && dictGraph[queue[l]].distance < dictGraph[queue[smallest]].distance) smallest=l;
        if (r<sizeGraphQueue && dictGraph[queue[r]].distance < dictGraph[queue[smallest]].distance) smallest=r;
        if (smallest != i){
            dictGraph[queue[i]].posHeap = smallest;
            dictGraph[queue[smallest]].posHeap = i;
            swapNode(&queue[i], &queue[smallest]);
            minHeapify(smallest);
        }
    }
}
int minPop(){
    int i;
    sizeGraphQueue--;
    dictGraph[queue[sizeGraphQueue]].posHeap = 0;
    swapNode(&queue[0], &queue[sizeGraphQueue]);
    if(sizeGraphQueue>1) minHeapify(0);
    return queue[sizeGraphQueue];
}
void minDeleteNode(int pos){
    int i;
    sizeGraphQueue--;  
    dictGraph[queue[pos]].posHeap = sizeGraphQueue;
    dictGraph[queue[sizeGraphQueue]].posHeap = pos;
    swapNode(&queue[pos], &queue[sizeGraphQueue]);
    for (i=sizeGraphQueue/2-1; i>=0; i--) minHeapify(i);
}

void displayMinHeapWithDetails(){
	int i;
    for (i=0; i<sizeGraphQueue; i++) printf("%d(%d) ", queue[i], dictGraph[queue[i]].distance);
    printf("\n");
}
void minInsert(int iNode){
	int i;
    if (sizeGraphQueue==0){
        queue[0] = iNode;
        dictGraph[iNode].posHeap = 0;
        sizeGraphQueue += 1;
    }else {
        queue[sizeGraphQueue] = iNode;
        dictGraph[iNode].posHeap = sizeGraphQueue;
        minHeapifyBotTop(sizeGraphQueue);
        sizeGraphQueue += 1;
    }
}


// ************************************************************************************************************
// ************************************************************************************************************
// ************************************************************************************************************
// ************************************************************************************************************
// ************************************************************************************************************
// ************************************************************************************************************


void localSetUp(){ 
    standardin=stdin;
    standardout=stdout;   
    standardin=fopen(INPUTTXT,"r");
    if(!standardin) standardin= stdin;
    standardout=fopen(OUTPUTTXT,"r");
    if(standardin) {
        fclose(standardout);
        standardout=fopen(OUTPUTTXT,"w");
    }else standardout=stdout;
}

typedef struct scanner{
    int current;
    t_line *input;
}t_scanner;
void displayScanner(t_scanner *scan){
    if(scan->input!=NULL) printf("%c(%d)",scan->input[scan->current],scan->current);
}
void skipInt(t_scanner *scanner){
    int i;
    t_line *temp;
    // fprintf("skip: %c(%d) -> ",c,c);
    for(temp = scanner->input; c!=',' && c!='\n';){
        if(c=='\0'){
            scanner->current = 0;
            temp = temp->next;
            scanner->input = temp;
        }else scanner->current++;
        c=temp->buffer[scanner->current];
    };
    if(c!='\n'){
        scanner->current++;
        if(temp->buffer[scanner->current]=='\0'){
            scanner->current = 0;
            temp = temp->next;
            scanner->input = temp;
        }
        c=temp->buffer[scanner->current];  
        // fprintf("%c(%d)\n",c,c);  
    } // fprintf("a capo\n");
}
int nextInt(t_scanner *scanner){
    t_line *temp = scanner->input;
    int i;
    c = temp->buffer[scanner->current];
    // fprintf("nextInt: %c(%d) -> ",c,c);
    if(c=='\n') return -1;
    for(i=0;c!=',' && c!='\n';){
        if(c=='\0'){
            scanner->current = 0;
            temp = temp->next;
            scanner->input = temp;
        }else {
            buffer[i] = c;
            scanner->current++;
            i++;
        }
        c = temp->buffer[scanner->current];
    };
    buffer[i] = '\0';
    if(c!='\n'){
        scanner->current++;
        if(temp->buffer[scanner->current]=='\0'){
            scanner->current = 0;
            temp = temp->next;
            scanner->input = temp;
        }
        c=temp->buffer[scanner->current];  
        // fprintf("%c(%d)\n",c,c);  
    } // fprintf("a capo\n");
    return atoi(buffer);
}
void dijkstraStepOffline(){
    int i,iMin = minPop(),cost,iArc;
    // printf("\nOffline dijkstra %d, %d",iMin,sizeGraphQueue);
    t_scanner scan;
    scan.current=0;
    scan.input = dictGraph[iMin].arcs;
    // display(dictGraph[iMin]);
    // fprintf(standardout,"OFF %d | ",iMin);
    displayGraph(standardout);
    dictGraph[iMin].isVisited = 't';
    iArc = 0;
    c=scan.input->buffer[scan.current];

    while (iArc<sizeGraph){ //c !='\n'
        if (dictGraph[iArc].isVisited=='t') {
            // printf("Already visited ");
            skipInt(&scan);
        }else{
            if(c=='0') {
                // fprintf("No connection ");
                skipInt(&scan);
            }else{
                cost = nextInt(&scan);
                cost += dictGraph[iMin].distance;
                if (dictGraph[iArc].isReachable=='f'){
                    // fprintf(standardout,"%d -> %d unconnected ",iMin,iArc);
                    dictGraph[iArc].distance = cost;
                    dictGraph[iArc].isReachable = 't';
                    minInsert(iArc);
                    // fprintf(standardout,"cost %d",dictGraph[iArc].distance);
                }else if (cost <dictGraph[iArc].distance){
                    // fprintf(standardout,"%d -> %d (%d<?<%d) ",iMin,iArc,cost,dictGraph[iArc].distance);
                    dictGraph[iArc].distance = cost;
                    minHeapifyBotTop(dictGraph[iArc].posHeap); // log(d)
                    // fprintf(standardout,"cost %d",dictGraph[iArc].distance);
                }
            }
        } 
        iArc++;
    }
    // printf(" terminated.");
}
void dijkstraStepOnline(){
    int i, iMin = minPop(), cost, iArc;
    // fprintf(standardout,"ONLINE %d | ",iMin);
    dictGraph[iMin].isVisited = 't';
    iArc = 0;
    while(c!='\n'){ //if , continue reading the arcs
        // if self arc or already isVisited skip the arc
        if (dictGraph[iArc].isVisited=='t') while ((c=getc(standardin))!=',' && c!='\n');
        else{
            // fscanf(standardin,"%d",&cost);
            
            c=getc(standardin); //read first most significant cipher
            // fscanf(standardin,"%d",&dictGraph[iNode].distance);
            if(c=='0'){
                c=getc(standardin); // read , or newline
            }else{
                
                buffer[0]=c;
                for(i=1;(buffer[i]=getc(standardin))!=',' && buffer[i]!='\n';i++);
                c = buffer[i];
                buffer[i]='\0';
                // read int
                cost += atoi(buffer) + dictGraph[iMin].distance;
                if (dictGraph[iArc].isReachable=='f'){
                    // fprintf(standardout,"%d -> %d unconnected ",iMin,iArc);
                    dictGraph[iArc].distance = cost;
                    dictGraph[iArc].isReachable = 't';
                    minInsert(iArc);
                    // fprintf(standardout,"cost %d\n",dictGraph[iArc].distance);
                }else if (cost <dictGraph[iArc].distance){
                    // fprintf(standardout,"%d -> %d (%d<?<%d) ",iMin,iArc,cost,dictGraph[iArc].distance);
                    dictGraph[iArc].distance = cost;
                    minHeapifyBotTop(dictGraph[iArc].posHeap); // log(d)
                    // fprintf(standardout,"cost %d\n",dictGraph[iArc].distance);
                }
            }
        } 
        iArc++;
    }
}
void parsingFirstLine(){
    int i,iNode;
    c=getc(standardin);
    while ((c=getc(standardin))!=',' && c!='\n'); // skip reading headNode 0
    iNode=1;
    while(c!='\n'){ //if , continue reading
        c=getc(standardin); //read first most significant cipher
        // fscanf(standardin,"%d",&dictGraph[iNode].distance);
        dictGraph[iNode].idNode=iNode;
        dictGraph[iNode].isVisited='f';
        if(c=='0'){
            // no connection  
            dictGraph[iNode].distance = 0;
            dictGraph[iNode].isReachable='f';
            c=getc(standardin); // read , or newline
        }else{
            
            buffer[0]=c;
            for(i=1;(buffer[i]=getc(standardin))!=',' && buffer[i]!='\n';i++);
            c = buffer[i];
            buffer[i]='\0';
            dictGraph[iNode].distance=atoi(buffer);
            dictGraph[iNode].isReachable='t';
            minInsert(iNode);
        }
        iNode++;
    }
};

// (((((((((((((((((((((((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))))))))))))))))))))))
// (((((((((((((((((((((((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))))))))))))))))))))))
// (((((((((((((((((((((((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))))))))))))))))))))))
// (((((((((((((((((((((((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))))))))))))))))))))))
// (((((((((((((((((((((((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))))))))))))))))))))))
// (((((((((((((((((((((((((((((((((((((((((((((((((((((((())))))))))))))))))))))))))))))))))))))))))))))))))))))))

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
// 2 -'\d+'-> 4 (read cost arcs of another headNode, increase idNode)
// parsing of arc costs 
// 3 -'\d+'-> 4 (read a number)
// 4 -' '-> 3 (read another number)
// 4 -'\n'-> 2 (line complete try to go on final state)
int main(){
    int arcNode=0;
    int idGraph=-1;
    // TODO remove before update
    localSetUp();
    dimensions();

    // states 0,1
	initParsing();
    addGraphToTopK=0;
    c=getc(standardin);
    while (c!= EOF){
        if (c=='A'){            
            appendLastDijkstra(idGraph);
            
            // prepare to read a new graph
            idGraph++;
            arcNode = 0;
            sizeGraphQueue = 0;
            addGraphToTopK=1;
            while (getc(standardin)!='\n'); 

            // read the new graph
            printf("\nGraph %d",idGraph);
            parsingFirstLine();
            if (sizeGraphQueue>0) c=getc(standardin);
            else{
                // printf("No isReachable headNode");
                //skip the reading of the entire graph 
                while ((c=getc(standardin))!= EOF && c!='A' && c !='T') while (getc(standardin)!='\n');
                // immediately add the graph with cost 0
                addGraphToTopK = 0;
                maxInsert(idGraph,0);
                fprintf(stdin,"%d - cost: %d\n",idGraph, 0);
            } 
            for (arcNode=1;arcNode<sizeGraph;arcNode++){
                // if (queue[0]==arcNode){
                //     // process dijkstra while reading
                //     printf("on  |");
                //     // dijkstraStepOnline();
                //     // while(queue[0]<arcNode && sizeGraphQueue>0){
                //     //     // fprintf("off |");
                //     //     displayGraph(stdout);
                //     //     // dijkstraStepOffline();
                //     // }
                // }else 
                storeLineOnIncidence(arcNode);
            }
            displayGraph(stdout);
            
        }else if (c=='T'){
            appendLastDijkstra(idGraph);
            while ((getc(standardin))!='\n');

            // print the TopK
            printf("\nTopk: "); 
            displayMaxHeap();
            c=getc(standardin);

        }else{
            fgets(buffer,SIZEBUFFER,standardin);
            printf("\nSTRANGE\n%s",buffer);
            return 5;
        } 
    }
    printf("\nEOF");
    return 0;
}

// ************************************************************************************************************
// ************************************************************************************************************
// ************************************************************************************************************
// ************************************************************************************************************
// ************************************************************************************************************
// ************************************************************************************************************

void initParsing(){
    int i;
    // retrieve data
    trash = fgets(buffer, SIZEBUFFER, standardin);    
    for (i=-1;*(buffer+(++i))!=' ';);
    buffer[i]='\0';
    sizeGraph=atoi(buffer);
    maxSizeOut=atoi(buffer+(++i)); 
    dictGraph=malloc (sizeGraph*sizeof(t_headNode));
    queue=malloc (sizeGraph*sizeof(int));
    for(i=0;i<sizeGraph;i++){
        dictGraph[i].idNode = i;
        dictGraph[i].distance = 0;  
        dictGraph[i].isReachable='t';
        dictGraph[i].isVisited='t';
        dictGraph[i].posHeap=-1;
        dictGraph[i].first = (t_node*) malloc (sizeof(t_node));
        dictGraph[i].last = NULL;
        dictGraph[i].first->label = -1;
        dictGraph[i].first->cost = -1;
        dictGraph[i].first->next = NULL;
    }
    finalOut=malloc (maxSizeOut*sizeof(t_graph));
    sizeOut=0;

    // fprintf("Graph:%d, topK:%d init done\n",sizeGraph,maxSizeOut);  
}


void storeLine(int arcNode){
    int i;
    t_line *temp;
    for (i=0;i<SIZEBUFFER_1 && c!='\n';i++){
        dictGraph[arcNode].arcs->buffer[i]=c;
        c=getc(standardin);
    }
    dictGraph[arcNode].arcs->buffer[i] ='\0';
    temp=dictGraph[arcNode].arcs;
    while(c!='\n'){
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
    temp->buffer[i] = '\n';
    temp->isEnd='t';
};

void storeLineOnIncidence(int arcNode){
    int i,cost;
    printf("[%d",arcNode);
    while ((c=getc(standardin))!=',');
    dictGraph[arcNode].last = NULL;
    dictGraph[arcNode].size = 0;
    // push existing arcs
    for(i=1;i<sizeGraph;i++){
        if (i==arcNode) while (c=getc(standardin)!=44 && c!=10 && c!=0);
        else{
            if(fscanf(standardin,"%d",&cost)) pushArc(arcNode,i,cost);
            c=getc(standardin);
        }
    }
    printf("]");
};

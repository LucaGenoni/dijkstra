#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INPUTTXT "tests/input/file/input_2"
#define OUTPUTTXT "tests/output2"
FILE *standardin,*standardout;
typedef struct node{
	unsigned int label,cost;
	struct node *next;
}t_node;

typedef struct headNode{
	unsigned int id_Node,distance,size;
	char visited;
	t_node *first,*last;
}t_headNode;

void free_incidence_Matrix(t_headNode* ,unsigned int);
void minHeapifyTopBot(t_headNode *,unsigned int [], unsigned int );
unsigned int minPop (t_headNode *,unsigned int [],unsigned int);
void minHeapifyBottomUp(t_headNode *,unsigned int [], unsigned int);
void minInsertNode(t_headNode *,unsigned int [],unsigned int ,unsigned int );
int minFind(t_headNode *,unsigned int [], unsigned int  , unsigned int  );
void printArray(FILE *,int []);
void dijkstra(t_headNode *,unsigned int );
void displayArc(t_node *);
void displayAllNode(t_headNode );
void displayNode(t_headNode );
void displayIncidentMatrix(t_headNode *,unsigned int );
void pushArc(t_headNode *,unsigned int src, unsigned int dest,unsigned int cost);

/**
 * liberazione della memoria per la matrice di incidenza.
**/
void free_incidence_Matrix(t_headNode *sourceHead,unsigned int number_Nodes){
	t_node *toDelete;
	unsigned int i;
	// displayIncidentMatrix(sourceHead,number_Nodes);
	for (i=0;i<number_Nodes;i++){
		sourceHead[i].last = NULL;
		while (sourceHead[i].first != NULL){
			// displayArcDetails(toDelete);
			toDelete = sourceHead[i].first;
			sourceHead[i].first = toDelete->next;
			free(toDelete);
		}
		// displayIncidentMatrix(sourceHead,number_Nodes);
	}
}

/**
 * heap binario. 
 * in posizione 0 c'é il numero di elementi contenuti nella lista 
 * gli elementi 1,...,N contengono gli elementi dell'heap
**/
void minHeapifyTopBot(t_headNode *sourceHead,unsigned int heap[], unsigned int index){
    unsigned int left = 2 * index, right = 2 * index + 1, smallest = index;
    if (left <= heap[0] && sourceHead[heap[left]].distance < sourceHead[heap[smallest]].distance) smallest = left;
    if (right <= heap[0] && sourceHead[heap[right]].distance < sourceHead[heap[smallest]].distance) smallest = right;
    
    if (smallest != index){
		left = heap[index];
		heap[index] = heap[smallest];
		heap[smallest] = left;
        minHeapifyTopBot(sourceHead,heap,smallest);
    }
}
unsigned int minPop (t_headNode *sourceHead,unsigned int heap[],unsigned int maxSize){ //log(n)
	unsigned int max;
    if (heap[0] > 0){
        max = heap[1];
		heap[1] = heap[heap[0]];
        heap[0]--;
        minHeapifyTopBot(sourceHead,heap,1); //log(n)
		return max;
    }
	return UINT_MAX;
}

void minHeapifyBottomUp(t_headNode *sourceHead,unsigned int heap[], unsigned int index){
    unsigned int parent = index/2, temp;
    if (parent==0) return;
	if (sourceHead[heap[index]].distance < sourceHead[heap[parent]].distance) {
		temp = heap[index];
		heap[index] = heap[parent];
		heap[parent] = temp;
		minHeapifyBottomUp(sourceHead,heap, parent); //log(n)
	}
}

void minInsertNode(t_headNode *sourceHead,unsigned int heap[],unsigned int maxSize,unsigned int toAdd){ //log(n) or 2log(n)
	if (heap[0]==0){
        heap[0] = 1;
        heap[1] = toAdd;        
	}else if (heap[0] < maxSize){
        heap[0]++;
        heap[heap[0]] = toAdd;
        minHeapifyBottomUp(sourceHead, heap, heap[0]); //log(n)
    }else{
        minPop(sourceHead, heap, maxSize); //log(n)
        minInsertNode(sourceHead, heap, maxSize, toAdd); //log(n)
    }
    return ;
}

int minFind(t_headNode *sourceHead,unsigned int heap[], unsigned int  index, unsigned int  target){ //between log(n) and O(n) 
    int left=2*index,right=2*index+1;
    if (index>heap[0]) return -3;
    if (sourceHead[heap[index]].distance == target) return index;
    if (sourceHead[heap[index]].distance > target) return -2;
    left = minFind(sourceHead, heap, left, target);
    if (left>-1) return left;
    return minFind(sourceHead, heap, right, target);
}

void printArray(FILE *stand,int heap[]){
    int i;
    if (heap[0]>0){
        // printf("%d",heap[1]);
        for ( i = 1; i <= heap[0]; i++) fprintf(stand," %d",heap[i]);
    }
    fprintf(stand,"\n");
}
/**
 * algoritmo dijkstra
**/
void dijkstra(t_headNode *sourceHead,unsigned int number_Nodes){
	t_node *temp;
	unsigned int i,i_min,queue[number_Nodes+1],check[number_Nodes];
	unsigned int min,previous;
	char flag = 0;
	queue[0] = 0;
	check[0] = 1;
	
	for (i=1;i<number_Nodes;i++) {
		minInsertNode(sourceHead,queue,number_Nodes+1,i);
		check[i]=0;
	}
	check[0] = 1;
	while (queue[0]>0){
		i_min = minPop(sourceHead,queue,number_Nodes+1);
		if(sourceHead[i_min].distance==UINT_MAX) break;
		check[i_min] = 1;
		if (sourceHead[i_min].size>0){
			temp = sourceHead[i_min].first;
			while (1){
				min = sourceHead[i_min].distance + temp->cost;
				if (min < sourceHead[temp->label].distance){
					previous = minFind(sourceHead,queue,1,sourceHead[temp->label].distance);
					sourceHead[temp->label].visited = 1;
					sourceHead[temp->label].distance = min;
					// printf
					if(previous<UINT_MAX-4) minHeapifyBottomUp(sourceHead,queue,previous);
				}
				if (temp==sourceHead[i_min].last) break;
				temp = temp->next;
			}
		}
	} 
};

// dosplay a single arcs 
void displayArc(t_node *this){
	// printf("%3d(%5d)  <%6x-%6x>, ",this->label,this->cost,this,(this->next));
	fprintf(standardout,"%3d(%5d), ",this->label,this->cost);
	return;
}

// display all the allocated arcs to the node (previous garbage arcs not freed)
// example of loop to free the memory
void displayAllNode(t_headNode sourceHead){
	t_node *temp;
	fprintf(standardout,"\nNodo %3d - first: %6x - last: %6x \tArcs: ",sourceHead.id_Node,sourceHead.first,sourceHead.last);
	for(temp = sourceHead.first; temp != NULL; temp = temp->next) displayArc(temp);
	return;
}

// display only the arcs belonging to the node. (overwritten arcs)
// example of loop to use for dijkstra
void displayNode(t_headNode sourceHead){
	t_node *temp;
	fprintf(standardout,"\nN:%3d, V:%1d(%6d), S:%4d, (%6x->%6x) : ",sourceHead.id_Node,sourceHead.visited,sourceHead.distance,sourceHead.size,sourceHead.first,sourceHead.last);
	if (sourceHead.last!=NULL) {
		for(temp = sourceHead.first; temp != NULL && sourceHead.last!=temp; temp = temp->next) displayArc(temp);
		displayArc(temp); // last element
	}	
	return;
}

// display the entire graph
void displayIncidentMatrix(t_headNode *sourceHead,unsigned int number_Nodes){
	int i;
	for (i=0;i<number_Nodes;i++) displayNode(sourceHead[i]);
	fprintf(standardout,"\n");
	return;
}

// modified push, allow overwrite to skip repeated malloc 
// space worst case... worse than an adjacent matrix
// time worst case, should be better than adjacent matrix
void pushArc(t_headNode *sourceHead,unsigned int src, unsigned int dest,unsigned int cost){
	if (sourceHead[src].last == NULL){ 
		//first element of the list
		sourceHead[src].last = sourceHead[src].first;
	}else if (sourceHead[src].last->next == NULL){
		//new element need to be allocated to be added.
		sourceHead[src].last->next = malloc (sizeof(t_node));
		sourceHead[src].last = sourceHead[src].last->next;
		sourceHead[src].last->next = NULL;
	}else sourceHead[src].last = sourceHead[src].last->next;
	// overwrite the new values
	sourceHead[src].last->label = dest;
	sourceHead[src].last->cost = cost;
	sourceHead[src].size++;
	return;
}

int main(){
// definitions
	// constants
		unsigned int number_Nodes, top_K;
		t_headNode *incidence_Matrix;
	// variables
		unsigned int id_Graphs, src, dest, costNode, sum, max, i_max,size_topK;
		char input_Char;
		standardin =stdin;
         standardin=fopen(INPUTTXT,"r");
         if(!standardin) standardin= stdin;
         standardout=fopen(OUTPUTTXT,"r");
		 if(standardin) {
		 	fclose(standardout);
         	standardout=fopen(OUTPUTTXT,"w");
		 }
// Initializations
	// constants 
		if(fscanf(standardin,"%d %d",&number_Nodes,&top_K)==EOF) return 10;
		incidence_Matrix = (t_headNode*) malloc (number_Nodes*sizeof(t_headNode));
		unsigned int listTopK[top_K+1][2];
	// graph
		for ( src=0; src<number_Nodes;src++){
			incidence_Matrix[src].id_Node = src;
			incidence_Matrix[src].first = (t_node*) malloc (sizeof(t_node));
			incidence_Matrix[src].last = NULL;
			incidence_Matrix[src].first->label = -1;
			incidence_Matrix[src].first->cost = -1;
			incidence_Matrix[src].first->next = NULL;
		}
	// variables
		id_Graphs=0;
		size_topK = 0;
// main loop
	while ((input_Char=getc(standardin))!=EOF){

		if (input_Char=='A') {
			while (getc(standardin)!='\n');
			// update arcs of incidence_Matrix
            if(number_Nodes>0){
                incidence_Matrix[0].last = NULL;
				incidence_Matrix[0].size = 0;
				incidence_Matrix[0].distance = 0;
				incidence_Matrix[0].visited = 1;
                if(fscanf(standardin,"%d",&costNode)==EOF) return 20; // retrieve the weight
                if(getc(standardin)==EOF) return 30; // discard "," or "\n"
                for( dest=1; dest<number_Nodes;dest++){
					if(fscanf(standardin,"%d",&costNode)==EOF) return 20; // retrieve the weight
					if(getc(standardin)==EOF) return 30; // discard "," or "\n"
					// printf("%3d(%5d),",dest,costNode); // show matrix on screen  
                    incidence_Matrix[dest].visited = costNode!=0?1:0;
                    if (incidence_Matrix[dest].visited) incidence_Matrix[dest].distance = costNode;
                    else incidence_Matrix[dest].distance = UINT_MAX;
                     // optimization, no storing for non arcs and self arcs.
					
				}
            }
			for (src=1; src < number_Nodes; src++){
				// Initialization of the new last arc. (no counter, but pointer to pushing/overwrite at the end of the list)
				incidence_Matrix[src].last = NULL;
				// incidence_Matrix[src].distance = UINT_MAX;
				// incidence_Matrix[src].visited = 0;
				incidence_Matrix[src].size = 0;
				// push existing arcs
                if(fscanf(standardin,"%d",&costNode)==EOF) return 20; // retrieve the weight
                if(getc(standardin)==EOF) return 30; // discard "," or "\n"
				for( dest=1; dest<number_Nodes;dest++){
					if(fscanf(standardin,"%d",&costNode)==EOF) return 20; // retrieve the weight
					if(getc(standardin)==EOF) return 30; // discard "," or "\n"
					if (src==dest || costNode==0 ) continue; // optimization, no storing for non arcs and self arcs.
					pushArc(incidence_Matrix,src,dest,costNode); // store the arc
					
				}
			}
			// if (standardout) {
			// 	fprintf(standardout,"############################    %d\n",id_Graphs);
			// 	displayIncidentMatrix(incidence_Matrix,number_Nodes);
			// };
			dijkstra(incidence_Matrix,number_Nodes);
			// add all the distance of visited nodes
			sum = 0;
			for (src = 1; src < number_Nodes; src++){
				if (incidence_Matrix[src].visited !=0){
					sum += incidence_Matrix[src].distance;
				}
			}
			// update TOPK list
            // if (listTopK[0][0]>0){
            //     // if the max is bigger then add the new graph
            //     if (listTopK[1][0]>sum) maxInsertNode(listTopK,top_K+1,sum,id_Graphs);                
            // }else maxInsertNode(listTopK,top_K,sum,id_Graphs);
			if(id_Graphs<top_K){
				listTopK[id_Graphs][0] = id_Graphs;
				listTopK[id_Graphs][1] = sum;
			}else{
				max = listTopK[0][1];
				i_max = 0;
				for(src = 1; src < top_K; src++){
					if (max<=listTopK[src][1]){
						max = listTopK[src][1];
						i_max = src;
					}
				}
				if (sum<max) {
					listTopK[i_max][0] = id_Graphs;
					listTopK[i_max][1] = sum;
				}
			}
			
			if (standardout) {
				fprintf(standardout,">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>    %d - cost: %d\n",id_Graphs, sum);
				displayIncidentMatrix(incidence_Matrix,number_Nodes);
			};
			id_Graphs++;
		}else if (input_Char=='T'){
			while (getc(standardin)!='\n');
			if(id_Graphs < top_K) size_topK = id_Graphs;
			else size_topK = top_K;
			if (size_topK>0){
				size_topK--;
				for (src = 0; src < size_topK; src++) printf("%d ",listTopK[src][0]);
				printf("%d",listTopK[size_topK][0]);
			}
			printf("\n");
			// printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>    %d - cost: %d\n",id_Graphs, sum);
			// printArray(listTopK);
		}
	};

	return 0;
}

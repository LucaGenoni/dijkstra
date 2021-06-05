#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
// #define INPUTTXT "./-h/input/open/input.6"
#define INPUTTXT "tests/input/file/input_1"
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

// i can also don't free the memory
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

void dijkstra(t_headNode *sourceHead,unsigned int number_Nodes){
	t_node *temp;
	unsigned int i,j,queue[number_Nodes],i_min;
	unsigned int min;
	min = 0;
	i_min = 0;
	for (i=0;i<number_Nodes;i++) queue[i] = 0;
	sourceHead[0].distance = 0;
	// displayIncidentMatrix(sourceHead,number_Nodes);
	// printf("\n");

	for (i=0;i<number_Nodes;i++){
		// pick the node with minimum cost
		for (j=0;j<number_Nodes;j++) 
			if (queue[j]==0){
				min = sourceHead[j].distance;
				i_min = j;
				break;
			}
		for (j=0;j<number_Nodes;j++){
			if (queue[j]==0){
				if (min>sourceHead[j].distance){
					min = sourceHead[j].distance;
					i_min = j;
				}
			}
		}


		// on the next iteration queue[i] will not be considered
		queue[i_min] = 1;
		if(standardout) fprintf(standardout,"<");
		for(temp = sourceHead[i_min].first,j=0; j<sourceHead[i_min].size; temp = temp->next,j++){
			if(queue[temp->label]==0){
				min = sourceHead[i_min].distance + temp->cost;
				if(standardout) fprintf(standardout,"%d -> %d cost %d",sourceHead[i_min].id_Node,temp->label,min);
				if (min < sourceHead[temp->label].distance){
					sourceHead[temp->label].visited = 1;
					sourceHead[temp->label].distance = min;
				}
			}
		}
	}
	if(standardout) fprintf(standardout,"\n");
	return;
};

// dosplay a single arcs 
void displayArc(t_node *this){
	// printf("%3d(%5d)  <%6x-%6x>, ",this->label,this->cost,this,(this->next));
	if(standardout) fprintf(standardout,"%3d(%5d), ",this->label,this->cost,this,(this->next));
	return;
}

// display all the allocated arcs to the node (previous garbage arcs not freed)
// example of loop to free the memory
void displayAllNode(t_headNode sourceHead){
	t_node *temp;
	if(standardout) fprintf(standardout,"\nNodo %3d - first: %6x - last: %6x \tArcs: ",sourceHead.id_Node,sourceHead.first,sourceHead.last);
	for(temp = sourceHead.first; temp != NULL; temp = temp->next) displayArc(temp);
	return;
}

// display only the arcs belonging to the node. (overwritten arcs)
// example of loop to use for dijkstra
void displayNode(t_headNode sourceHead){
	t_node *temp;
	if(standardout) fprintf(standardout,"\nN:%3d, V:%1d(%6d), S:%4d, (%6x->%6x) : ",
		sourceHead.id_Node,sourceHead.visited,sourceHead.distance,sourceHead.size,sourceHead.first,sourceHead.last);
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
	if(standardout) fprintf(standardout,"\n");
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
		unsigned int listTopK[top_K][2];
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


















			for (src=0; src < number_Nodes; src++){
				// Initialization of the new last arc. (no counter, but pointer to pushing/overwrite at the end of the list)
				incidence_Matrix[src].last = NULL;
				incidence_Matrix[src].distance = UINT_MAX;
				incidence_Matrix[src].visited = 0;
				incidence_Matrix[src].size = 0;
				// push existing arcs
                if(fscanf(standardin,"%d",&costNode)==EOF) return 20; // retrieve the weight
                if(getc(standardin)==EOF) return 30; // discard "," or "\n"
				for( dest=1; dest<number_Nodes;dest++){
					if(fscanf(standardin,"%d",&costNode)==EOF) return 20; // retrieve the weight
					if(getc(standardin)==EOF) return 30; // discard "," or "\n"
					if (src==dest || costNode==0 || dest==0) continue; // optimization, no storing for non arcs and self arcs.
					pushArc(incidence_Matrix,src,dest,costNode); // store the arc
					
				}
			}
			dijkstra(incidence_Matrix,number_Nodes);
			// add all the distance of visited nodes
			sum = 0;
			for (src = 1; src < number_Nodes; src++){
				if (incidence_Matrix[src].visited !=0){
					sum += incidence_Matrix[src].distance;
				}
			}
			// update TOPK list
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
				fprintf(standardout,"############################    %d - cost: %d\n",id_Graphs, sum);
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
		}
	};

	return 0;
}

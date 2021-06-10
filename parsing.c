
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define SIZEBUFFER 50000
#define SIZEBUFFER_1 SIZEBUFFER-1

typedef struct line{
    char *positions;
    char *buffer;
    struct line *next;
}t_line;

typedef struct node{
    char visited;
    char reachable;
	int id_Node,distance;
    t_line arcs;
}t_node;

char buffer[SIZEBUFFER], *trash;
t_node *finalCosts;
char c;

void dimensions(){
    printf("\nSize node: %d",sizeof(t_node));
    printf("\nSize line: %d",sizeof(t_line));
    printf("\n");
}

void initParsing(int []);


void parsingFirstLine(FILE *input){
    int i,distrance,i_node;
    c=getchar();
    i_node=0;
    while ((c=getchar())!=' ' && c!='\n'); // skip reading node 0
    while(c!='\n'){
        if(c=='0'){    
            finalCosts[i_node].distance = 0;
            finalCosts[i_node].reachable = 0;
            finalCosts[i_node].visited = 0;
            finalCosts[i_node].id_Node = i_node;
        }else{
            // buffer[0] = c;
            for(i=0;(c=getchar())!=' ' && c!='\n';i++) buffer[i] = c;
            buffer[i] = '\0';
            finalCosts[i_node].distance = atoi(buffer);
            finalCosts[i_node].reachable = 1;
            finalCosts[i_node].visited = 0;
            finalCosts[i_node].id_Node = i_node;
        }
        i_node++;
    }
};
// void parsingLine(FILE *input){
//     int i;
//     i=0;
//     while ((c=getchar())!='\n'){
//         startingCosts
//         i++;
//         if(i==SIZEBUFFER_1){
//             startingCosts[i]='\0'

//         }
//     }
//     for(i=0;;i++) {
//         buffer[i] = c;
//     }
// };

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
// 2 -'\d+'-> 4 (read a cost of arc increase id_node)
// parsing of arc costs 
// 3 -'\d+'-> 4 (read a number)
// 4 -' '-> 3 (read another number)
// 4 -'\n'-> 2 (line complete try to go on final state)
int main(){
    int n=0,init[2];
    char computeDijkstra=0;
    dimensions();

    // states 0,1
	initParsing(init);
    printf("%d, %d",init[0],init[1]);
    finalCosts = malloc (init[0]*sizeof(t_node));
    int topKList[init[1]];
    //check if final state
    while ((c=getchar()) != EOF){ 
        if (c=='A'){
            if (computeDijkstra==1){
                
            }
            while (getchar()!='\n'); //clear
            parsingLine();

            
            trash = fgets(buffer, SIZEBUFFER, stdin);
            // parsing the node 0
            // store 
            // read and process disjktra
        }else if (c=='T'){            
            if (computeDijkstra==1){
                computeDijkstra=0;
            }      
        }else{
            while (getchar()!='\n');
            // print topk    

        };
    }
    printf("End of file reached");
}

void initParsing(int init[]){
    int i;
    trash = fgets(buffer, SIZEBUFFER, stdin);    
    for (i=-1;*(buffer+(++i))!=' ';);
    buffer[i] = '\0';
    init[0] = atoi(buffer);
    init[1] = atoi(buffer+(++i));
}
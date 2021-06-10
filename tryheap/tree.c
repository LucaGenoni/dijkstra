#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdbool.h>
// https://en.wikipedia.org/wiki/Binary_heap

typedef struct node{
    int key;
    int value;
}t_node;

static int pow2i(int a) {
    int r = 1;
    while (a--) {
        r *= 2;
    }
    return r;
}

static void display_tree(const int *arr, size_t arrlen) {
    assert(arrlen);
    size_t i;
    int longest_digits = 0;
    int tree_depth = 0;
    bool first;
    int pos = 0;
    int depth = 0;
    for (i = 0; i <arrlen; ++i) {
        if (arr[i] != -1) {
            const int len = snprintf(NULL, 0, "%d", arr[i]);
            if (longest_digits < len) {
                longest_digits = len;
            }
        }
        
        if (pos == 0) {
            tree_depth++;
            pos = pow2i(depth++);
        }
        pos--;      
    }

    //printf("%d %d\n", longest_digits, tree_depth);

    pos = 0;
    depth = 0;
    const int additional_offset = 3;
    int max_width = pow2i(tree_depth) * (longest_digits + additional_offset);
    for (i = 0; i < arrlen; ++i) {
        first = pos == 0;
        if (first) {
            pos = pow2i(depth);
            depth++;
        }
        const int count_elems = pow2i(depth);
        const int chunk = max_width / count_elems;
        const int width = chunk + (first ? -chunk/2 : 0);
        const int pre_spaces = width - longest_digits;

        printf("%*s", pre_spaces, "");
        if (arr[i] == -1) {
            printf("%*s", longest_digits, "-");
        } else {
            printf("%*d", longest_digits, arr[i]);
        }

        if (pos == 1) {
            printf("\n");
        }
        pos--;
    }

}

void maxFillVoid(int heap[], int maxSize, int index){
    int left = 2 * index, right = left+1;
    if (right<maxSize){
        if(heap[left] > heap[right]){
            heap[index] = heap[left];
            maxFillVoid(heap, maxSize, left);
        }else{
            heap[index] = heap[right];
            maxFillVoid(heap, maxSize, right);
        }
    }else if (left<maxSize){
        heap[index] = heap[left];
        maxFillVoid(heap, maxSize, left);
    }
    return;
}
void maxHeapify(int heap[], int index){
    int parent = index/2, temp;
    if (parent>0) {
        if (heap[index] > heap[parent]) {
            temp = heap[index];
            heap[index] = heap[parent];
            heap[parent] = temp;
            maxHeapify(heap, parent);
        }
    }
}

void maxPop (int heap[], int max[], int maxSize){ //log(n)
    if (heap[0] > 0){
        max[0] = 1;
        max[1] = heap[1];
        maxFillVoid(heap,maxSize,1); //log(n)
        heap[0]--;
    }else max[0] = 0;
}

void maxInsertNode(int heap[], int maxSize, int toAdd){ //log(n) or 2log(n)
    if (heap[0] < maxSize){
        // just insert
        heap[0]++;
        heap[heap[0]] = toAdd;
        maxHeapify(heap, heap[0]); //log(n)
    }else{
        int max[2];
        maxPop(heap, max, maxSize); //log(n)
        maxInsertNode(heap, maxSize, toAdd); //log(n)
    }
    return ;
}

int maxCheckProperty(int heap[], int index){
    int left=2*index,right=2*index+1;
    if (right<heap[0]){
        if (heap[index]<heap[right]) return right;
        right = maxCheckProperty(heap,right);
        if (right>-1) return right;
    } 
    if (left<heap[0]){
        if (heap[index]<heap[left]) return left;
        left = maxCheckProperty(heap,left);
        if (left>-1) return left;
    } 
    return -1;
}

void printArray(int heap[]){
    int i;
    if (heap[0]>0){
        printf("%d",heap[1]);
        for ( i = 1; i < heap[0]; i++) printf(" %d",heap[i]);
    }
    printf("\n");
}


void minFillVoid(unsigned int heap[],unsigned int maxSize,unsigned int index){
    int left = 2 * index, right = left+1;
    if (right < maxSize && heap[right] < heap[left]) left = right;
    else if (left>=maxSize) return;
	heap[index] = heap[left];
	minFillVoid(heap, maxSize, left);
}
void minHeapify(int heap[], int index){
    int parent = index/2, temp;
    if (parent>0) {
        if (heap[index] < heap[parent]) {
            temp = heap[index];
            heap[index] = heap[parent];
            heap[parent] = temp;
            minHeapify(heap, parent);
        }
    }
}

void minPop (int heap[], int max[], int maxSize){ //log(n)
    if (heap[0] > 0){
        max[0] = 1;
        max[1] = heap[1];
        minFillVoid(heap,maxSize,1); //log(n)
        heap[0]--;
    }else max[0] = 0;
}

void minInsertNode(int heap[], int maxSize, int toAdd){ //log(n) or 2log(n)
    if (heap[0] < maxSize){
        // just insert
        heap[0]++;
        heap[heap[0]] = toAdd;
        minHeapify(heap, heap[0]); //log(n)
    }else{
        int max[2];
        minPop(heap, max, maxSize); //log(n)
        minInsertNode(heap, maxSize, toAdd); //log(n)
    }
    return ;
}

int minCheckProperty(int heap[], int index){
    int left=2*index,right=2*index+1;
    if (right<heap[0]){
        if (heap[index]>heap[right]) return right;
        right = minCheckProperty(heap,right);
        if (right>-1) return right;
    } 
    if (left<heap[0]){
        if (heap[index]>heap[left]) return left;
        left = minCheckProperty(heap,left);
        if (left>-1) return left;
    } 
    return -1;
}

int maxFind(int heap[], int index, int target){ //between log(n) and O(n) 
    int left=2*index,right=2*index+1;
    if (heap[index] == target) return index;
    if (index>=heap[0]) return -5;
    if (heap[index] < target) return -4;
    left = maxFind(heap, left, target);
    if (left>-1) return left;
    return maxFind(heap, right, target);
}

int minFind(int heap[], int index, int target){ //between log(n) and O(n) 
    int left=2*index,right=2*index+1;
    if (heap[index] == target) return index;
    if (index>=heap[0]) return -3;
    if (heap[index] > target) return -2;
    left = minFind(heap, left, target);
    if (left>-1) return left;
    return minFind(heap, right, target);
}


int main(){
    int maxSize = 31;
    int arr[maxSize],tree[maxSize-1];
    int actual = 0;
    int max[2] = {0,0};
    int r,i,m;
    int counter=0;
    srand(time(NULL));   // Initialization, should only be called once.
    arr[0]=0;
    for (counter = 0; counter < 10000; counter++){
        printf("\rtest %d",counter);
        for (i=1;i<1000;i++){
            maxInsertNode(arr,maxSize,rand());
            r = maxCheckProperty(arr,1);
            if(r>-1){
                printf("violato i_c:%d(%d),i_p:%d(%d)\n",r/2,arr[r/2],r,arr[r]);
                return -1;
            }
        } 
        r = rand()%arr[0];
        m = maxFind(arr,1,arr[r]);
        if (m <0) {
            printf("\nCerco indice %d con %d, ma non l'ho trovato %d",r,arr[r],m);
            // print array            
            for (i=0;i<arr[0];i++) tree[i]=arr[i+1];
            display_tree(tree, arr[0]);
            return 1;
        }
        for (i=0;i<1000;i++){
            maxPop(arr, max, maxSize);
            if (max[0]!=0 && max[1]<arr[1]){
                printf("\nViolazione Max: %d, Next:%d\n",max[1],arr[1]);
                return -1;
            }
        }
        for (i=1;i<11111;i++){
            maxInsertNode(arr,maxSize,rand());
            r = maxCheckProperty(arr,1);
            if(r>-1){
                printf("violato i_c:%d(%d),i_p:%d(%d)\n",r/2,arr[r/2],r,arr[r]);
                return -1;
            }
        } 
        
        for (i=0;i<1000;i++){
            maxPop(arr, max, maxSize);
            if (max[0]!=0 && max[1]<arr[1]){
                printf("\nViolazione Max: %d, Next:%d\n",max[1],arr[1]);
                return -1;
            }
        }

        // printf("tutto ok per i max heap dimensione %d\n",arr[0]);
        for (i=0;i<11111;i++){
            minInsertNode(arr,maxSize,rand());
            r = minCheckProperty(arr,1);
            if(r>-1){
                // print array
                for (i=0;i<arr[0];i++) tree[i]=arr[i+1];
                display_tree(tree, arr[0]);
                printf("\nViolato i_c:%d(%d),i_p:%d(%d)\n",r/2,arr[r/2],r,arr[r]);
                return -1;
            }
        } 
        // if (minFind(arr,1,arr[rand()%arr[0]]) == -1) return -2;
        for (i=0;i<11111;i++){
            minInsertNode(arr,maxSize,rand());
            r = minCheckProperty(arr,1);
            if(r>-1){
                // print array            
                for (i=0;i<arr[0];i++) tree[i]=arr[i+1];
                display_tree(tree, arr[0]);
                printf("\nViolato INSERT_BEFORE i_c:%d(%d),i_p:%d(%d)\n",r/2,arr[r/2],r,arr[r]);
                return -1;
            }

            minPop(arr, max, maxSize);

            r = minCheckProperty(arr,1);
            if(r>-1){
                for (i=0;i<arr[0];i++) tree[i]=arr[i+1];
                display_tree(tree, arr[0]);
                // print array
                printf("\nViolato POP i_c:%d(%d),i_p:%d(%d)\n",r/2,arr[r/2],r,arr[r]);
                return -1;
            }

            minInsertNode(arr,maxSize,rand());

            r = minCheckProperty(arr,1);
            if(r>-1){
                for (i=0;i<arr[0];i++) tree[i]=arr[i+1];
                display_tree(tree, arr[0]);
                // print array
                printf("\nViolato INSERT_AFTER i_c:%d(%d),i_p:%d(%d)\n",r/2,arr[r/2],r,arr[r]);
                return -1;
            }
            r = (rand()%(arr[0]-1))+1;
            m = minFind(arr,1,arr[r]);
            if (m <0) {
                printf("\nCerco indice %d con %d, ma non l'ho trovato %d",r,arr[r],m);
                // print array            
                for (i=0;i<arr[0];i++) tree[i]=arr[i+1];
                display_tree(tree, arr[0]);
                return 1;
            }
        } 
        for (i=0;i<11111;i++) minPop(arr, max, maxSize);
    }
    return 0;
}

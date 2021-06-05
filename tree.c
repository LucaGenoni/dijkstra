#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdbool.h>
// https://en.wikipedia.org/wiki/Binary_heap
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
void printArray(int heap[], int actual){
    int i;
    if (actual>0){
        printf("%d",heap[0]);
        for ( i = 1; i < actual; ++i) printf(" %d",heap[i]);
        printf("\n");
    }
}

void maxHeapifyToDelete(int heap[], int actual, int parent){
    int left=2*parent, right=2*parent, largest = parent;
    if (left < actual && heap[left] > heap[largest]) largest = left;
    if (right < actual && heap[right] > heap[largest]) largest = right;
    if (largest != parent){
        left = heap[largest];
        heap[largest] = heap[parent];
        heap[parent] = left;
        maxHeapifyToDelete(heap,actual,largest);
    }
}

int getMax(int heap[], int *actual){
    int max=0;
    if ((*actual)>0){
        max = heap[0];
        (*actual)--;
        heap[0] = heap[(*actual)];
        heap[(*actual)+1] = 0;
        maxHeapifyToDelete(heap,(*actual),0);
    }
    return max;
}

void heapify(int heap[], int index){
    int parent = index/2, temp;
    if (heap[index] != UINT_MAX) {
        if (heap[index] > heap[parent]) {
            temp = heap[index];
            heap[index] = heap[parent];
            heap[parent] = temp;
            heapify(heap, parent);
        }
    }
}

void insertNode(int heap[],int size, int toAdd){
    if (heap[0]<size){

        heap[(*actual)] = toAdd;
    	(*actual)++;
        heapify(heap, (*actual)-1);
    }else{ 
        // need to implement pop push
    }
    return ;
}

void fillVoid(int heap[], int size, int index){
    int left = 2 * index, right = left+1;
    if (right<size){
        if(heap[left] > heap[right]){
            heap[index] = heap[left];
            fillVoid(heap, size, left);
        }else{
            heap[index] = heap[right];
            fillVoid(heap, size, right);
        }
    }else if (left<size){
        heap[index] = heap[left];
        fillVoid(heap, size, left);
    }
    return;
}
// position 0 used to store the real size
int* getMaxArray (int heap[], int size){
    int max[2]={0,0};
    if (heap[0] > 0){
        max[0] = 1;
        max[1] = heap[1];
        fillVoid(heap,size,1);
        heap[0]--;
    }
    return max;
}
int checkProperty(int heap[], int index){
    int left=2*index,right=2*index+1;
    if (right<heap[0]){
        if (heap[index]<heap[right]) return right;
        right = checkProperty(heap,right);
        if (right>-1) return right;
    } 
    if (left<heap[0]){
        if (heap[index]<heap[left]) return left;
        left = checkProperty(heap,left);
        if (left>-1) return left;
    } 
    return -1;
}
int main(){
    int arr[100];
    int actual = 0;
    int maxSize = 100;
    int r,i;
    srand(time(NULL));   // Initialization, should only be called once.
    arr[0]=0;
    for (i=1;i<20;i++){
        insertNode(arr,&actual,maxSize,rand());
        r = checkProperty(arr,1);
        if(r>-1){
            printf("violato i_c:%d(%d),i_p:%d(%d)\n",r/2,arr[r/2],r,arr[r]);
            display_tree(arr, actual);
            break;            
        }
    
    } 
    
    // for (i=0;i<20;i++){
        
    //     display_tree(arr, actual);
    //     r = getMax(arr, &actual);
    //     printf("\nMax: %d, Next:%d\n",r,arr[0]);
    //     if(r<arr[0]) return 1;
    // }
    // for (i=0;i<50;i++)insertNode(arr,&actual,maxSize,rand());    
    
    return 0;
}

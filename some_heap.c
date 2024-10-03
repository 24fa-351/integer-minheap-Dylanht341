
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "some_heap.h"

#define KEY_NOT_PRESENT -1

heap_t *heap_create(int capacity) {        // Allocates memory for the heap and data within the heap
    heap_t *heap = (heap_t*)malloc(sizeof(heap_t));
    heap->data = (heap_node_t*)malloc(sizeof(heap_node_t));
    heap->size = 0;
    heap->capacity = capacity;
}

void heap_free(heap_t *heap) {            // Frees memory allocated for heap and data within heap
    if(heap) {
        if (heap->data) {
            free(heap->data);
        }
        free(heap);
    }
}

unsigned int heap_size(heap_t *heap) { return heap->size; }

unsigned int heap_parent(unsigned int index) {
    if (index == 0) {
        return 0;
    }
    return (index - 1) / 2;
}

unsigned int heap_left_child(unsigned int index) {
    return 2 * index + 1;
}

unsigned int heap_right_child(unsigned int index) {
    return 2 * index + 2;
}

unsigned int heap_level(unsigned int index) {
    return(unsigned int)floor(log2(index + 1));
}

void heap_print(heap_t *heap) {
    for (int ix = 0; ix < heap_size(heap); ix++) {
        printf("%3d - %3d : " HEAP_KEY_FORMAT "\n", heap_level(ix), ix,
               heap->data[ix].key);
    }
    printf("\n");
}

void heap_swap(heap_t *heap, int index1, int index2) {
    heap_node_t temp_node = heap->data[index1];         // Stores one index while swap happens
    heap->data[index1]  = heap->data[index2];           // Swap indices
    heap->data[index2] = temp_node;                     // Update other index to stored index
}

void heap_bubble_up(heap_t *heap, int index) {
    while (index > 0) {
        unsigned int parent_index = heap_parent(index);
        // Checks if current index is less than parent index
        if (heap->data[index].key < heap->data[parent_index].key) {
            heap_swap(heap, index, parent_index);   // Swap current node and parent node
            index = parent_index;                   // Update current node index to parent index
        } else {
            break;                                  // If current node is in the right position, do not swap
        }
    }
}

void heap_bubble_down(heap_t *heap, int index) {
    unsigned int size = heap_size(heap);
    while (index < size) {
        unsigned int index_left_child = heap_left_child(index);
        unsigned int index_right_child = heap_right_child(index);
        unsigned int index_smallest = index;
        // Check for left child and compare values
        if (index_left_child < size && heap->data[index_left_child].key < heap->data[index_smallest].key) {
            index_smallest = index_left_child;      // If smaller, update the smallest index
        }
        // Check for right child and compare values
        if (index_right_child < size && heap->data[index_right_child].key < heap->data[index_smallest].key) {
            index_smallest = index_right_child;     // if smaller, update the smallest index
        }
        // If smallest index has not been updated, do not swap
        if (index_smallest == index) {
            break;
        }

        heap_swap(heap, index, index_smallest);     // Swap current node and smallest child node
        index = index_smallest;                     // Update current node index to smallest child node index
    }
}

void heap_insert(heap_t *heap, heap_key_t key, heap_value_t data) {
    if (heap_size(heap) == heap->capacity) {
        return;
    }

    heap->data[heap_size(heap)].key = key;
    heap->data[heap_size(heap)].value = data;
    heap->size++;

    heap_bubble_up(heap, heap_size(heap) - 1);
}


heap_value_t heap_remove_min(heap_t *heap) {
    if (heap_size(heap) == 0) {
        return (heap_value_t){.as_int = KEY_NOT_PRESENT};
    }

    heap_value_t min = heap->data[0].value;

    heap->size--;

    // move last element to the root
    heap->data[0] = heap->data[heap_size(heap)];

    // then bubble it down to its correct position
    heap_bubble_down(heap, 0);

    return min;
}

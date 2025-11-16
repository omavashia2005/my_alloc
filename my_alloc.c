#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define HEAP_CAP 640000
#define CHUNK_ALLOCED_CAP 1024
#define CHUNK_FREED_CAP 1024

typedef struct {
	char *start;
	size_t size;
} chunk;


typedef struct{
	size_t count;
	chunk chunks[CHUNK_ALLOCED_CAP];
} chunk_list;



void dump_chunk_list(const chunk_list *list){
	printf("ALLOCATED CHUNKS %zu:\n", list->count);
	for(size_t i = 0; i < list->count; ++i){
		printf(" start %p, size %zu \n", 
	 		list->chunks[i].start, 
	 		list->chunks[i].size);
	}
}


int chunk_start_compar(const void *a, const void *b){
	const chunk *a_chunk = a;
	const chunk *b_chunk = b;

	return a_chunk->start - b_chunk->start;
}

/*
 const pointer
	* You cannot modify what p points to
	* But you can move p to point somewhere else 
	* Allows you to walk the list but not edit it, required to FIND a pointer in the list of chunks
*/
int chunk_list_find(const chunk_list *list, void *ptr){

	chunk key = {
		.start = ptr
	};

	chunk *result = bsearch(&key, list->chunks, list->count, sizeof(*list->chunks), chunk_start_compar);

	if(result != 0){
		assert(list->chunks <= result);
		return (result - list->chunks) / (sizeof(*list->chunks));
	}
	
	return -1;
}

void chunk_list_insert(chunk_list *list, void *ptr, size_t size){

	assert(list->count < CHUNK_FREED_CAP);
	
	list->chunks[list->count].start = ptr;
	list->chunks[list->count].size = size;

	for(size_t i = list->count; 
		i > 0 && list->chunks[i].start < list->chunks[i-1].start; 
		--i){
		chunk t = list->chunks[i];
		list->chunks[i] = list->chunks[i - 1];
		list->chunks[i - 1] = t;
	}

	list->count += 1;
}

void chunk_list_remove(chunk_list *list, size_t index){

	assert(index <= list->count);
	
	for(size_t i = 0; i < list->count; ++i){

		list->chunks[i] = list->chunks[i + 1];
	}

	list->count --;
}



char heap[HEAP_CAP] = {0};

size_t heap_size = 0;

chunk_list alloced_chunks = {0};
chunk_list freed_chunks = {0};


size_t heap_alloced_size = 0;


void *heap_alloc(size_t size){
	if (size > 0){

		assert(heap_size + size <= HEAP_CAP);
		void *ptr = heap + heap_size;
		heap_size += size;
		chunk_list_insert(&alloced_chunks, ptr, size);
		return ptr;
	}

	return NULL;
}

void heap_free(void *ptr)
{
	if(ptr != NULL){
		const int index = chunk_list_find(&alloced_chunks, ptr);
		
		assert(index >= 0);
		
		chunk_list_insert(&freed_chunks, 					 // array of insertion 
				  alloced_chunks.chunks[index].start,		        // pointer to start of block of insertion
				  alloced_chunks.chunks[index].size); 		       // size of insertion

		chunk_list_remove(&alloced_chunks, (size_t) index);
	}
}

void heap_collect(){
	assert(false && "free NOT implemented");
}

int main(){

	for(int i = 0; i < 10; ++i){	
		void *p = heap_alloc(i);
		if (i % 2 == 0){
			heap_free(p);
		}
	}

 	dump_chunk_list(&alloced_chunks);

	return 0;
}

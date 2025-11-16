#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define HEAP_CAP 640000
#define CHUNK_ALLOCED_CAP 1024
#define CHUNK_FREED_CAP 1024

typedef struct {
	void *start;
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

/*
 const pointer
	* You cannot modify what p points to
	* But you can move p to point somewhere else 
	* Allows you to walk the list but not edit it, required to FIND a pointer in the list of chunks
*/
int chunk_list_find(const chunk_list *list, void *ptr){
	
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
	
	return ;
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
	assert(false && "free NOT implemented");
}

void heap_collect(){
	assert(false && "free NOT implemented");
}

int main(){

	for(int i = 0; i < 100; ++i){	
		void *p = heap_alloc(i);
		//if (i % 2 == 0){
			//heap_free(p);
		//}
	}

 	dump_chunk_list(&alloced_chunks);

	return 0;
}



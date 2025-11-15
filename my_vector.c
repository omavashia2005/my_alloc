#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct{
	int32_t *data;
	size_t length;
	size_t capacity;
} Vector;

/*
	 Vector *vector passes the Vector struct by reference
	 Any changes made to this parameter directly affect the object Vector 
*/
void resize(Vector *vector){

	if (vector->length < vector->capacity) return; // no need to resize

	if(vector->length > vector->capacity){
		fprintf(stderr, "Out of capacity\n");
		exit(EXIT_FAILURE);
	}

	size_t new_capacity = (vector->capacity == 0) 	? 2 : vector->capacity * 2;

	size_t new_bytes = new_capacity * sizeof(*vector->data);
		
//	printf("vector capacity before realloc: %zu\n", vector->capacity);
	
	int32_t *new_block = realloc(vector->data, new_bytes); // void *ptr, size_t new_size	

	// dealing with null ptr being returned by realloc
	// If there is not enough memory, the old memory block is not freed and null pointer is returned
	if(new_block == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(EXIT_FAILURE);
	}

	vector->data = new_block;
	vector->capacity = new_capacity;

}

void append(Vector *vector, int32_t value){

	resize(vector);	

	// printf("vector capacity after realloc: %zu\n", vector->capacity);
	
	vector->data[vector->length++] = value;
}


void pop(Vector *vector){
	assert(vector->length > 0 && "Length of vector must be > 0");
	vector->data[vector->length] = 0;
	vector->length--;

}


/*
Corrected here, but:
	The number of bytes to move in memmove should be vector->length - 1 instead of vector->capacity 
	to avoid moving uninitialized memory beyond the valid elements.
*/
void shift(Vector *vector){
	assert(vector->length > 0 && "Length of vector must be > 0");
	memmove(&vector->data[0], &vector->data[1], (vector->length - 1) * (sizeof(*vector->data)));
	vector->length--;
}

void unshift(Vector *vector, int32_t value){
	resize(vector);
	memmove(&vector->data[1], &vector->data[0], vector->length * (sizeof(*vector->data)));
	vector->data[0] = value;
	vector->length++;
}


void insert(Vector *vector, int32_t index, int32_t value){
	resize(vector);
	memmove(&vector->data[index + 1], &vector->data[index], (vector->length - index) * (sizeof(*vector->data)));
	vector->data[index] = value;
	vector->length++;
}

int main(void) 
{
	Vector vector = {0};

	for(int32_t i = 1;i < 10 ; i++){
		append(&vector, i);
	}

	for(int32_t i = 0; i < 2; i++){
	
		pop(&vector);
	}

	for(int32_t i = 0; i < 2; i++){
	
		shift(&vector);
	}

	for(int32_t i = 0; i < vector.length; i++){
	
		printf("%d\n", vector.data[i]);
	}
	

	printf("\n\nPost shift\n\n");

	for(int32_t i = 0; i < 2; i++){
		unshift(&vector, i);
	}


	insert(&vector, 2, 10);

	for(int32_t i = 0; i < vector.length; i++){
	
		printf("%d\n", vector.data[i]);
	}
	
	free(vector.data);	

}

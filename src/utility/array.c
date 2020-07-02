#include "array.h"


array arr_init(size_t initial_size, size_t element_size){
	assert(element_size > 0);
	assert(initial_size >= 0);
	if(initial_size < 1){
		printf("Error in arr_init initial size input!\n");
		exit(-1);
	}
	//printf("initial size is: %d\n", initial_size);
	size_t alloc_size = initial_size;
	if((initial_size & (initial_size - 1)) != 0){
		printf("Initial size is not a power of 2!\n");
		size_t carry = 2;
		while( alloc_size > 1 ){
			alloc_size /= 2;
			carry *= 2;
		}
		alloc_size =  carry;
	}
	//printf( "Allocated size is %d elements, equalling %d bytes.\n" , alloc_size, alloc_size * element_size );
	void * data = malloc( element_size * alloc_size );
	assert(data != NULL);
	if(data == NULL){
		printf("arr_init memory allocation error! Exiting...\n");
		exit(-1);
	}
	array arr;
	arr.size		= 0;
	arr.capacity 	= alloc_size;
	arr.elementsize = element_size;
	arr.data = data;
	return arr;
};

int arr_cleanup(array * arr){
	arr->size = 0;
	arr->capacity = 0;
	arr->elementsize = 0;
	free(arr->data);
};

int arr_add(  array * arr , void * element ){
	int err = 0;
	if( arr->size >= arr->capacity ){
		//printf("grow!\n");
		err += arr_grow(arr);
		if ( err != 0 ){
			printf("ERROR IN arr_add!  Quitting...");
			exit(-1);
		}
	}
	void * loc = (arr->data + ((arr->size) * arr->elementsize));
	loc = memcpy(loc, element, arr->elementsize);
	if( loc == NULL){
		printf("Memory copy error in arr_add!");
		exit(-1);
	}
	arr->size++;
	return err;
}
int arr_remove( array * arr , int i ){
	int err = 0;
	assert(arr->size > 0);
	assert((arr->size >= i)&&(i >= 0));
	memmove(	(arr->data + i * arr->elementsize),
				(arr->data + ((i + 1) * arr->elementsize)),
				(size_t)((arr->size - (i + 1))*arr->elementsize) );
	arr->size--;
	if(arr->size <= arr->capacity/2){
		arr_shrink(arr);
	}
	return err;
}
int arr_grow( array * arr ){
	int err = 0;
	size_t newcap = 2 * arr->capacity;
	assert(newcap >= arr->capacity);
	arr->data = realloc(arr->data, newcap * arr->elementsize);
	if(arr->data == NULL){
		err = -1;
		printf("Memory allocation error in arr_grow!\n");
		exit(-1);
	}
	arr->capacity = newcap;
	//printf("GROW: capacity is now %d\n", arr->capacity);
	return err;
}
int arr_shrink( array * arr ){
	int err = 0;
	size_t newcap = arr->capacity / 2;
	arr->data = realloc(arr->data, newcap * arr->elementsize);
	if(arr->data == NULL){
		err = -1;
		printf("Memory allocation error in arr_shrink!\n");
		exit(-1);
	}
	arr->capacity = newcap;
	//printf("SHRINK: capacity is now %d\n", arr->capacity);
	return err;
}

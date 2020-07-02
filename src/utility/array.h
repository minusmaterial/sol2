#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//dynamic array.  allocated space expands geometrically (power of 2).  This means:  for element number n, optimal memory usage is at n == power of 2, and least optimal memory usage is at n== (power of 2) + 1.

typedef struct{
	size_t size;
	size_t capacity;
	size_t elementsize;
	void * data;
}array;

#define arr_get(type, arr, i ) \
((i >= arr.size)? (type*)NULL : (type * )arr.data)[i]
#define arr_set(type, arr, i, value) ((type * )arr.data)[i] = value
array arr_init(size_t initial_size, size_t element_size);
int arr_cleanup(array * arr);

int arr_add( array * arr, void * element );
int arr_remove( array * arr, int i );
int arr_grow(  array * arr );
int arr_shrink(  array * arr );


#ifndef _POOL_
#define _POOL
#include <stdlib.h>

class MemoryPool {
	size_t unit_size;
	size_t block_size;
	
	size_t curr;
	void *memblock;
	void **elements;
	
public:
	
	MemoryPool(size_t _unit_size, size_t _unit_num)\
	: unit_size(_unit_size),block_size(_unit_num*_unit_size),curr(0) {
		memblock = malloc(block_size);
		elements  = (void**)malloc(_unit_num*sizeof(void*));
		
		if(memblock!=NULL && elements!=NULL) {
			for(curr=0; curr<_unit_num;curr++)
				elements[curr] = (void*)((char*)memblock+curr*unit_size);
		}
	}
	~MemoryPool() {
		if(elements!=NULL)
			free(elements);
		if(memblock!=NULL)
			free(memblock);			
	}
	void* alloc(size_t size) {
		if(size<=unit_size&&curr>0) //pool is still available
			return elements[--curr];
		else  //allocate using system call
			return malloc(size);
	}
	void dealloc(void *p) {
		
		if(memblock<=p && p<(void*)((char*)memblock+block_size))
			elements[curr++] = p;//in the range of memory block
		else
			free(p);	
	}
};
#endif
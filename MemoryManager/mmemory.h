#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdbool.h>

typedef char* VA;	// Тип описывающий адрес блока 	

#define  SUCCESSFUL_EXECUTION  0
#define  INCORRECT_PARAMETERS  -1
#define  LACK_OF_MEMORY  -2
#define  UNKNOWN_ERROR  1
#define  ACCESS_OUTSIDE_BLOCK  -2

struct segment {
	VA adress;
	struct segment* physical_adress;
	size_t size;
	bool isFree;
	char* info;
	struct segment * next;
};

struct real_memory {
	VA adress;
	size_t size;
};

int _malloc(VA* ptr, size_t szBlock);

int _free(VA ptr);

int _read(VA ptr, char* pBuffer, size_t szBuffer);

int _write(VA ptr, void* pBuffer, size_t szBuffer);

int _init(int n, int szPage);

void combine_free_segments(struct segment * segment);

void create_new_segment(struct segment * new_segment, size_t block_size, struct segment * old_segment, size_t szBlock);

#endif // MEMORY_H

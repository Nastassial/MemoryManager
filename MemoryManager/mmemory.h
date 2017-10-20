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

struct v_segment {
	VA adress;
	struct p_segment* physical_adress;
	size_t size;
	bool isFree;
	char* info;
	struct v_segment * next;
};

struct p_segment {
	struct v_segment* virtual_adress;
	size_t size;
	bool isFree;
	struct p_segment* next;
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

void unionFreeSegments();

void unionFreePhysicalSegments();

void create_new_physical_segment(struct p_segment * p_old_segment, struct p_segment * p_new_segment, size_t newBlockSize);

void read_information(char* temp, void* pBuffer, struct v_segment* current_virtual_segment, VA ptr, size_t infoSize);

void write_information(struct v_segment* current_virtual_segment, VA ptr, char* newInfo, size_t infoSize);
#endif // MEMORY_H

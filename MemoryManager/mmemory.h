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
	VA physical_adress;
	struct p_segment* physical_segment;
	size_t size;
	bool isFree;
	VA info;
	struct v_segment * next;
};

struct p_segment {
	struct v_segment* virtual_segment;
	size_t size;
	bool isFree;
	VA info;
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

VA malloc_info(VA* info, size_t size);

void set_links(struct p_segment* physical_segment, struct v_segment* virtual_segment);

void connect_segments(struct p_segment * first_segment, struct  p_segment * second_segment);

struct p_segment * create_new_physical_segment(size_t newBlockSize);

void read_information(char* temp, void* pBuffer, struct v_segment* current_virtual_segment, VA ptr, size_t infoSize);

void write_information(struct v_segment* current_virtual_segment, VA ptr, char* newInfo, size_t infoSize);
#endif // MEMORY_H

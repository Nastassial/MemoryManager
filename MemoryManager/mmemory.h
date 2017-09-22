#include <stddef.h>
#include <stdbool.h>


typedef char* VA;	// Тип описывающий адрес блока 	

struct segment {
	VA* adress;
	size_t size;
	bool isFree;
	bool access;
	struct segment * next;
};

struct real_memory {
	VA* adress;
	size_t size;
};

int _malloc(VA* ptr, size_t szBlock);

int _free(VA ptr);

int _read(VA ptr, void* pBuffer, size_t szBuffer);

int _write(VA ptr, void* pBuffer, size_t szBuffer);

int _init(int n, int szPage);
#include <stddef.h>
#include <stdbool.h>


typedef char* VA;		

struct segment {
	VA* adress;
	unsigned int size;
	bool isFree;
	bool access;
};

struct real_memory {
	VA* adress;
	unsigned int size;
};

int _malloc(VA* ptr, size_t szBlock);

int _free(VA ptr);

int _read(VA ptr, void* pBuffer, size_t szBuffer);

int _write(VA ptr, void* pBuffer, size_t szBuffer);

int _init(int n, int szPage);
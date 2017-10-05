#include <stddef.h>
#include <stdbool.h>

typedef char* VA;	// ��� ����������� ����� ����� 	

struct segment {
	VA adress;
	VA physical_adress;
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

int _read(VA ptr, void* pBuffer, size_t szBuffer);

int _write(VA ptr, void* pBuffer, size_t szBuffer);

int _init(int n, int szPage);

//void freeSegment(VA* ptr, struct segment* current_segment);
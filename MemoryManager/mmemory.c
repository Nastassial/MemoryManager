#include "mmemory.h"
#include <stddef.h>

struct real_memory virtual_memory;
struct real_memory physical_memory;
struct segment* virtual_memory_array;
struct segment* physical_memory_array;
int number_of_virtual_segments = 0;
int number_of_physical_segments = 0;

const int SUCCESSFUL_EXECUTION = 0;
const int INCORRECT_PARAMETERS = -1;
const int LACK_OF_MEMORY = -2;
const int UNKNOWN_ERROR = 1;
const int ACCESS_OUTSIDE_BLOCK = -2;


//Выделяет блок памяти определенного размера
//адрес блока, размер блока, код ошибки: 0, -1, -2, 1
int _malloc(VA* ptr, size_t szBlock)
{
	return SUCCESSFUL_EXECUTION;
}


//Удаление блока памяти
//адрес блока, код ошибки: 0, -1, 1
int _free(VA ptr)
{
	return SUCCESSFUL_EXECUTION;
}

//Чтение информации из блока памяти
//адрес блока, адрес буфера(куда копируется информация), размер буфера, код ошибки: 0, -1, -2, 1
int _read(VA ptr, void* pBuffer, size_t szBuffer)
{
	return SUCCESSFUL_EXECUTION;
}

//Запись информации в блок памяти
//адрес блока, адрес буфера(куда копируется информация), размер буфера, код ошибки: 0, -1, -2, 1
int _write(VA ptr, void* pBuffer, size_t szBuffer) 
{
	return SUCCESSFUL_EXECUTION;
}

//количество страниц и размер страницы, код ошибки: 0, -1, 1 
int _init(int n, int szPage) 
{
	if (n <= 0 || szPage <= 0) return INCORRECT_PARAMETERS;

	int virtual_size = n*szPage;
	virtual_memory.adress = (VA)malloc(virtual_size);

	if (virtual_memory.adress == NULL) return UNKNOWN_ERROR;

	virtual_memory.size = virtual_size;
	physical_memory.size = round(virtual_size / 2);
	physical_memory.adress = (VA)malloc(physical_memory.size);
	
	if (physical_memory.adress == NULL) return UNKNOWN_ERROR;
	
	virtual_memory_array = (struct segment*)malloc(virtual_size * sizeof(struct segment));
	physical_memory_array = (struct segment*)malloc(physical_memory.size * sizeof(struct segment));
	struct segment initial_virtual_segment;
	initial_virtual_segment.adress = virtual_memory.adress;
	initial_virtual_segment.size = virtual_memory.size;
	initial_virtual_segment.isFree = true;
	virtual_memory_array[number_of_virtual_segments] = initial_virtual_segment;
	struct segment initial_physical_segment;
	initial_physical_segment.adress = physical_memory.adress;
	initial_physical_segment.size = physical_memory.size;
	initial_physical_segment.isFree = true;
	physical_memory_array[number_of_physical_segments] = initial_physical_segment;

	return SUCCESSFUL_EXECUTION;
}
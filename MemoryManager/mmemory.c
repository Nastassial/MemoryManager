#include "mmemory.h"
#include <stddef.h>
#include <math.h>  
#include <stdio.h>
#include <stdlib.h>

struct real_memory virtual_memory;
struct real_memory physical_memory;
struct segment* virtual_memory_array;
struct segment* physical_memory_array;
int number_of_virtual_segments = 0;
int number_of_physical_segments = 0;
size_t free_physical_memory = 0;

const int SUCCESSFUL_EXECUTION = 0;
const int INCORRECT_PARAMETERS = -1;
const int LACK_OF_MEMORY = -2;
const int UNKNOWN_ERROR = 1;
const int ACCESS_OUTSIDE_BLOCK = -2;

struct segment *initial_virtual_segment;
struct segment *initial_physical_segment;

//�������� ���� ������ ������������� �������
//����� �����, ������ �����, ��� ������: 0, -1, -2, 1
int _malloc(VA* ptr, size_t szBlock)
{
	if (physical_memory.size < szBlock) return LACK_OF_MEMORY;
	
	struct segment *current_virtual_segment = initial_virtual_segment;

	while (current_virtual_segment->next != NULL)
	{
		if (current_virtual_segment->isFree == true && current_virtual_segment->size >= szBlock)
		{
			size_t block_size = current_virtual_segment->size;

			if (block_size == szBlock)
			{
				current_virtual_segment->isFree = false;
				ptr = current_virtual_segment->adress;
				break;
			}
			else
			{
				current_virtual_segment->size = szBlock;
				current_virtual_segment->isFree = false;
				ptr = current_virtual_segment->adress;
				struct segment *new_segment;
				new_segment->isFree = true;
				new_segment->adress = current_virtual_segment->adress + szBlock;
				new_segment->size = block_size - szBlock;
				current_virtual_segment->next = new_segment;
				new_segment->next = NULL;
				number_of_virtual_segments++;
				break;
			}
			current_virtual_segment = current_virtual_segment->next;
		}
	}

	struct segment* current_physical_segment = initial_physical_segment;

	while(current_physical_segment->next != NULL)
	{
		if (current_physical_segment->isFree == true && current_physical_segment->size >= szBlock)
		{
			size_t block_size = current_physical_segment->size;

			if (block_size == szBlock)
			{
				current_physical_segment->isFree = false;
				break;
			}
			else
			{
				current_physical_segment->size = szBlock;
				current_physical_segment->isFree = false;
				struct segment *new_segment;
				new_segment->isFree = true;
				new_segment->adress = current_physical_segment->adress + szBlock;
				new_segment->size = block_size - szBlock;
				current_physical_segment->next = new_segment;
				new_segment->next = NULL;
				number_of_physical_segments++;
				break;
			}
			current_physical_segment = current_physical_segment->next;
		}
	}
	return SUCCESSFUL_EXECUTION;
}


//�������� ����� ������
//����� �����, ��� ������: 0, -1, 1
int _free(VA ptr)
{
	free(ptr);
	return SUCCESSFUL_EXECUTION;
}

//������ ���������� �� ����� ������
//����� �����, ����� ������(���� ���������� ����������), ������ ������, ��� ������: 0, -1, -2, 1
int _read(VA ptr, void* pBuffer, size_t szBuffer)
{
	return SUCCESSFUL_EXECUTION;
}

//������ ���������� � ���� ������
//����� �����, ����� ������(���� ���������� ����������), ������ ������, ��� ������: 0, -1, -2, 1
int _write(VA ptr, void* pBuffer, size_t szBuffer) 
{
	return SUCCESSFUL_EXECUTION;
}

//���������� ������� � ������ ��������, ��� ������: 0, -1, 1 
int _init(int n, int szPage) 
{
	if (n <= 0 || szPage <= 0) return INCORRECT_PARAMETERS;

	int virtual_size = n*szPage;
	virtual_memory.adress = (VA)malloc(virtual_size);

	if (virtual_memory.adress == NULL) return UNKNOWN_ERROR;

	virtual_memory.size = virtual_size;
	physical_memory.size = round(virtual_size / 2);
	physical_memory.adress = virtual_memory.adress;
	
	if (physical_memory.adress == NULL) return UNKNOWN_ERROR;

	initial_virtual_segment->adress = virtual_memory.adress;
	initial_virtual_segment->size = virtual_memory.size;
	initial_virtual_segment->isFree = true;
	initial_virtual_segment->next = NULL;
	number_of_virtual_segments++;

	initial_physical_segment->adress = physical_memory.adress;
	initial_physical_segment->size = physical_memory.size;
	initial_physical_segment->isFree = true;
	initial_physical_segment->next = NULL;
	number_of_physical_segments++;

	return SUCCESSFUL_EXECUTION;
}
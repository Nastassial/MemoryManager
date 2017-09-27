#include "mmemory.h"
#include <stddef.h>
#include <math.h>  
#include <stdio.h>
#include <stdlib.h>

const int SUCCESSFUL_EXECUTION = 0;
const int INCORRECT_PARAMETERS = -1;
const int LACK_OF_MEMORY = -2;
const int UNKNOWN_ERROR = 1;
const int ACCESS_OUTSIDE_BLOCK = -2;

struct real_memory virtual_memory;
struct real_memory physical_memory;

struct segment *initial_virtual_segment;
struct segment *initial_physical_segment;

bool flag;

//�������� ���� ������ ������������� �������
//����� �����, ������ �����, ��� ������: 0, -1, -2, 1
int _malloc(VA* ptr, size_t szBlock)
{
	if (ptr == NULL) return INCORRECT_PARAMETERS;

	if (physical_memory.size < szBlock) return LACK_OF_MEMORY;
	
	flag = false;

	struct segment *current_virtual_segment = (struct segment*)malloc(sizeof(struct segment));
	current_virtual_segment = initial_virtual_segment;

	do
	{
		if (current_virtual_segment->isFree == true && current_virtual_segment->size >= szBlock)
		{
			size_t block_size = current_virtual_segment->size;

			if (block_size == szBlock)
			{
				current_virtual_segment->isFree = false;
				*ptr = current_virtual_segment->adress;
				flag = true;
				break;
			}
			else
			{
				current_virtual_segment->size = szBlock;
				current_virtual_segment->isFree = false;
				*ptr = current_virtual_segment->adress;
				struct segment new_segment;
				new_segment.isFree = true;
				new_segment.adress = current_virtual_segment->adress + szBlock;
				new_segment.size = block_size - szBlock;
				new_segment.next = current_virtual_segment->next;
				current_virtual_segment->next = &new_segment;
				flag = true;
				break;
			}
			current_virtual_segment = current_virtual_segment->next;
		}
	} while (current_virtual_segment->next != NULL);

	if (!flag) return LACK_OF_MEMORY;

	struct segment* current_physical_segment = (struct segment*)malloc(sizeof(struct segment));
	current_physical_segment = initial_physical_segment;

	do
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
				struct segment new_segment;
				new_segment.isFree = true;
				new_segment.adress = current_physical_segment->adress + szBlock;
				new_segment.size = block_size - szBlock;
				new_segment.next = current_physical_segment->next;
				current_physical_segment->next = &new_segment;
				break;
			}
			current_virtual_segment->physical_adress = current_physical_segment->adress;
			current_physical_segment = current_physical_segment->next;
		}
	} while (current_physical_segment->next != NULL);
	return SUCCESSFUL_EXECUTION;
}


//�������� ����� ������
//����� �����, ��� ������: 0, -1, 1
int _free(VA ptr)
{
	if (ptr == NULL) return INCORRECT_PARAMETERS;

	flag = false;

	struct segment *current_virtual_segment = (struct segment*)malloc(sizeof(struct segment));
	current_virtual_segment = initial_virtual_segment;
	
	freeSegment(*ptr, current_virtual_segment);

	if (!flag) return UNKNOWN_ERROR;

	struct segment* current_physical_segment = (struct segment*)malloc(sizeof(struct segment));
	current_physical_segment = initial_physical_segment;
	
	freeSegment(*ptr, current_physical_segment);

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
/*int _write(VA ptr, void* pBuffer, size_t szBuffer) 
{
	if (ptr == NULL) return INCORRECT_PARAMETERS;
	bool flag1 = false;
	struct segment *current_physical_segment = initial_physical_segment;
	while (current_physical_segment->next != NULL)
	{
		if (ptr == current_physical_segment->adress) {
			if (current_physical_segment->isFree = true) return UNKNOWN_ERROR;
				//����������
				if (szBuffer > current_physical_segment->size) return LACK_OF_MEMORY;
				char *newInfo;
				newInfo = (char*)pBuffer;
				char *resultStr = malloc(sizeof(char) * (szBuffer + 1));
				if (strlen(newInfo) > (szBuffer + 1)) {
					strncpy(resultStr, newInfo, szBuffer);
					int len = strlen(resultStr);
					resultStr[len + 1] = '\0';
					free(resultStr);
				}
				else {
					strncpy(resultStr, newInfo, szBuffer);
					free(resultStr);
				}
				current_physical_segment->info = resultStr;
				flag1 = true;
				break;
			}
		current_physical_segment = current_physical_segment->next;
	}
	bool flag2 = false;
	struct segment *neededSegment;
	if (!flag1) {
		struct segment *current_virtual_segment = initial_virtual_segment;
		while (current_virtual_segment->next != NULL)
		{
			if (ptr == current_virtual_segment) {
				if (current_virtual_segment->isFree) return UNKNOWN_ERROR;
				neededSegment = current_virtual_segment;
				int blockSize = current_virtual_segment->size;
				current_physical_segment = initial_physical_segment;
				while (current_physical_segment->next != NULL)
				{
					if (current_physical_segment->isFree = true) {
						if (current_physical_segment->size == blockSize) {
							current_physical_segment->adress = current_virtual_segment->adress;
							//����������
							if (szBuffer > current_physical_segment->size) return LACK_OF_MEMORY;
							char *newInfo;
							newInfo = (char*)pBuffer;
							char *resultStr = malloc(sizeof(char) * (szBuffer + 1));
							if (strlen(newInfo) > (szBuffer + 1)) {
								strncpy(resultStr, newInfo, szBuffer);
								int len = strlen(resultStr);
								resultStr[len + 1] = '\0';
								free(resultStr);
							}
							else {
								strncpy(resultStr, newInfo, szBuffer);
								free(resultStr);
							}
							current_physical_segment->info = resultStr;
							flag2 = true;
							break;

						}
						if (current_physical_segment->size > blockSize) {
							current_physical_segment->adress = current_virtual_segment->adress;

							struct segment *new_segment;
							new_segment->isFree = true;
							new_segment->adress = current_physical_segment->adress + current_physical_segment->size;
							new_segment->size = current_physical_segment->size - blockSize;;
							new_segment->next = current_physical_segment->next;
							current_physical_segment->next = new_segment;

							//����������
							if (szBuffer > current_physical_segment->size) return LACK_OF_MEMORY;
							char *newInfo;
							newInfo = (char*)pBuffer;
							char *resultStr = malloc(sizeof(char) * (szBuffer + 1));
							if (strlen(newInfo) > (szBuffer + 1)) {
								strncpy(resultStr, newInfo, szBuffer);
								int len = strlen(resultStr);
								resultStr[len + 1] = '\0';
								free(resultStr);
							}
							else {
								strncpy(resultStr, newInfo, szBuffer);
								free(resultStr);
							}
							current_physical_segment->info = resultStr;
							flag2 = true;
							break;

						}
					}
				}
				break;
			}
			current_virtual_segment = current_virtual_segment->next;
		}
	}
	if (!flag2) {
		if (neededSegment == NULL) return UNKNOWN_ERROR;
		current_physical_segment = initial_physical_segment;
		int size = 0;
		int numberOfNeededSegments = 0;
		while (current_physical_segment->next != NULL)
		{
			numberOfNeededSegments++;
			size += current_physical_segment->size;
			if (neededSegment->size > size) {
				current_physical_segment = current_physical_segment->next;
			}
			else break;
		}
		struct segment * newSegment1;
		newSegment1->adress = initial_physical_segment->adress;
		newSegment1->size = neededSegment->size;
		newSegment1->isFree = false;
		if (neededSegment->size == size) {
			newSegment1->next = current_physical_segment->next;
			free(current_physical_segment);

			//����������
			if (szBuffer > newSegment1->size) return LACK_OF_MEMORY;
			char *newInfo;
			newInfo = (char*)pBuffer;
			char *resultStr = malloc(sizeof(char) * (szBuffer + 1));
			if (strlen(newInfo) > (szBuffer + 1)) {
				strncpy(resultStr, newInfo, szBuffer);
				int len = strlen(resultStr);
				resultStr[len + 1] = '\0';
				free(resultStr);
			}
			else {
				strncpy(resultStr, newInfo, szBuffer);
				free(resultStr);
			}
			newSegment1->info = resultStr;
		}
		else {
			struct segment * newSegment2;
			newSegment2->adress = initial_physical_segment->adress + newSegment1->size;
			newSegment2->next = current_physical_segment->next;
			newSegment2->isFree = true;
			newSegment2->size = size - neededSegment->size;
			newSegment1->next = newSegment2->adress;
			free(current_physical_segment);

			//����������
			if (szBuffer > newSegment1->size) return LACK_OF_MEMORY;
			char *newInfo;
			newInfo = (char*)pBuffer;
			char *resultStr = malloc(sizeof(char) * (szBuffer + 1));
			if (strlen(newInfo) > (szBuffer + 1)) {
				strncpy(resultStr, newInfo, szBuffer);
				int len = strlen(resultStr);
				resultStr[len + 1] = '\0';
				free(resultStr);
			}
			else {
				strncpy(resultStr, newInfo, szBuffer);
				free(resultStr);
			}
			newSegment1->info = resultStr;

		}
	
	}

	return SUCCESSFUL_EXECUTION;
}*/

//���������� ������� � ������ ��������, ��� ������: 0, -1, 1 
int _init(int n, int szPage) 
{
	if (n <= 0 || szPage <= 0) return INCORRECT_PARAMETERS;

	int virtual_size = n*szPage;

	virtual_memory.size = virtual_size;
	physical_memory.size = (size_t)round(virtual_size / 2);

	virtual_memory.adress = (VA)malloc(virtual_memory.size);
	physical_memory.adress = (VA)malloc(physical_memory.size);

	if (virtual_memory.adress == NULL || physical_memory.adress == NULL) return UNKNOWN_ERROR;
	
	initial_virtual_segment = (struct segment*)malloc(sizeof(struct segment));
	initial_physical_segment = (struct segment*)malloc(sizeof(struct segment));

	initial_physical_segment->adress = physical_memory.adress;
	initial_physical_segment->size = physical_memory.size;
	initial_physical_segment->isFree = true;
	initial_physical_segment->next = NULL;

	initial_virtual_segment->adress = virtual_memory.adress;
	initial_virtual_segment->physical_adress = initial_physical_segment->adress;
	initial_virtual_segment->size = virtual_memory.size;
	initial_virtual_segment->isFree = true;
	initial_virtual_segment->next = NULL;
	
	return SUCCESSFUL_EXECUTION;
}


void freeSegment(VA* ptr, struct segment* current_segment)
{
	do
	{
		if (ptr == current_segment->adress) {
			current_segment->isFree = true;
			free(current_segment->info);
			flag = true;
			break;
		}
		current_segment = current_segment->next;
	} while (current_segment->next != NULL);
}
#include <stddef.h>
#include <math.h>  
#include <stdio.h>
#include <stdlib.h>
#include "mmemory.h"

struct real_memory virtual_memory;  //переписать освобождение, из физ ссылка на вирт
struct real_memory physical_memory;

struct v_segment* initial_virtual_segment;
struct p_segment* initial_physical_segment;

bool flag;

//Выделяет блок памяти определенного размера
//адрес блока, размер блока, код ошибки: 0, -1, -2, 1
int _malloc(VA* ptr, size_t szBlock)
{
	if (*ptr == NULL || szBlock == 0) return INCORRECT_PARAMETERS;
	if (initial_virtual_segment == NULL || initial_physical_segment == NULL) return UNKNOWN_ERROR;
	if (physical_memory.size < szBlock) return LACK_OF_MEMORY;

	flag = false;

	struct v_segment* current_virtual_segment = initial_virtual_segment;
	

	while (current_virtual_segment != NULL) {
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
				
				struct v_segment * new_virtual_segment = (struct v_segment*)malloc(sizeof(struct v_segment));
				new_virtual_segment->isFree = true;
				new_virtual_segment->adress = current_virtual_segment->adress + szBlock;
				new_virtual_segment->size = block_size - szBlock;
				new_virtual_segment->next = (struct v_segment*)malloc(sizeof(struct v_segment));
				new_virtual_segment->next = current_virtual_segment->next;
				new_virtual_segment->info = NULL;
				if (current_virtual_segment->next == NULL) {  //подумать, нужно ли
					current_virtual_segment->next = (struct v_segment*)malloc(sizeof(struct v_segment));
				}
				current_virtual_segment->next = new_virtual_segment;
				flag = true;
				break;
			}
		}
		current_virtual_segment = current_virtual_segment->next;
	}

	if (!flag) return LACK_OF_MEMORY;

	flag = false;

	struct p_segment* current_physical_segment = initial_physical_segment;

	while (current_physical_segment != NULL) {

		if (current_physical_segment->isFree == true && current_physical_segment->size >= szBlock)
		{
			size_t block_size = current_physical_segment->size;

			if (block_size == szBlock)
			{ 
				current_physical_segment->isFree = false;
				current_virtual_segment->physical_adress = (struct p_segment*)malloc(sizeof(struct p_segment));
				current_virtual_segment->physical_adress = current_physical_segment;
				current_physical_segment->virtual_adress = (struct v_segment*)malloc(sizeof(struct v_segment));
				current_physical_segment->virtual_adress = current_virtual_segment;
				flag = true;
				break;
			}
			else
			{
				current_physical_segment->size = szBlock;
				current_physical_segment->isFree = false;
			
				struct p_segment* new_physical_segment = (struct p_segment*)malloc(sizeof(struct p_segment));
				new_physical_segment->isFree = true;
				new_physical_segment->size = block_size - szBlock;
				new_physical_segment->next = (struct p_segment*)malloc(sizeof(struct p_segment));
				
				new_physical_segment->next = current_physical_segment->next;
				if (current_physical_segment->next == NULL) {
					current_physical_segment->next = (struct p_segment*)malloc(sizeof(struct p_segment));
				}
				current_physical_segment->next = new_physical_segment;

				current_virtual_segment->physical_adress = (struct p_segment*)malloc(sizeof(struct p_segment));
				current_virtual_segment->physical_adress = current_physical_segment;
				current_physical_segment->virtual_adress = (struct v_segment*)malloc(sizeof(struct v_segment));
				current_physical_segment->virtual_adress = current_virtual_segment;
				flag = true;
				break;
			}
		}
		current_physical_segment = current_physical_segment->next;
	}
	if (!flag) current_virtual_segment->physical_adress = NULL;
	return SUCCESSFUL_EXECUTION;
}


//Удаление блока памяти
//адрес блока, код ошибки: 0, -1, 1
int _free(VA ptr)
{
	if (ptr == NULL) return INCORRECT_PARAMETERS;

	flag = false;

    struct v_segment *current_virtual_segment = initial_virtual_segment;
	
	while (current_virtual_segment != NULL)
	{
		if (ptr == current_virtual_segment->adress) {
			current_virtual_segment->isFree = true;
			current_virtual_segment->info = NULL;
			flag = true;
			break;
		}
		current_virtual_segment = current_virtual_segment->next;
	};

	if (!flag) return UNKNOWN_ERROR;

	if (current_virtual_segment->physical_adress != NULL) {
		current_virtual_segment->physical_adress->isFree = true;
		current_virtual_segment->physical_adress->virtual_adress = NULL;
		
		current_virtual_segment->physical_adress = NULL;
	}

	unionFreeSegments();
	return SUCCESSFUL_EXECUTION;
}

//Чтение информации из блока памяти
//адрес блока, адрес буфера(куда копируется информация), размер буфера, код ошибки: 0, -1, -2, 1
int _read(VA ptr, char* pBuffer, size_t szBuffer)
{
	char* temp = (char*)pBuffer;
	int k = strlen(temp);
	if (ptr == NULL || strlen(temp)<szBuffer || szBuffer == 0) return INCORRECT_PARAMETERS;
	bool flag = false;
	struct v_segment* current_virtual_segment = initial_virtual_segment;
	while (current_virtual_segment != NULL)
	{
		if (ptr == current_virtual_segment->adress) {
			if (current_virtual_segment->info == NULL) return UNKNOWN_ERROR;
			size_t infoSize;
			if (szBuffer > strlen(current_virtual_segment->info)) return INCORRECT_PARAMETERS;
			
			infoSize = szBuffer;
			
			if (current_virtual_segment->physical_adress != NULL) {
				memcpy(pBuffer, current_virtual_segment->info, infoSize);
				pBuffer[infoSize] = '\0';
				flag = true;
				break;

			}
			else {
				struct p_segment* current_physical_segment = initial_physical_segment;
				while (current_physical_segment != NULL)
				{
					if (current_physical_segment->isFree == true && current_physical_segment->size >= current_virtual_segment->size)
					{
						if (current_physical_segment->size >= current_virtual_segment->size)
						{
							size_t newBlockSize = current_physical_segment->size - current_virtual_segment->size;
							struct p_segment* newSegment = (struct p_segment*)malloc(sizeof(struct p_segment));
							newSegment->size = newBlockSize;
							newSegment->isFree = true;
							newSegment->next = current_physical_segment->next;
							newSegment->virtual_adress = NULL;
							current_physical_segment->next = newSegment;
							current_physical_segment->size = current_virtual_segment->size;
							unionFreePhysicalSegments();
						}

						current_physical_segment->isFree = false;
						current_physical_segment->virtual_adress->physical_adress = NULL;
						current_physical_segment->virtual_adress = current_virtual_segment;
						current_virtual_segment->physical_adress = (struct p_segment*)malloc(sizeof(struct p_segment));
						current_virtual_segment->physical_adress = current_physical_segment;

						//чтение
						memcpy(pBuffer, current_virtual_segment->info, infoSize);
						pBuffer[infoSize] = '\0';
						flag = true;
						break;
					}
					current_physical_segment = current_physical_segment->next;
				};
				if (!flag) {
					size_t size = 0;
					current_physical_segment = initial_physical_segment;
					while (current_physical_segment != NULL)
					{
						size += current_physical_segment->size;
						current_physical_segment->virtual_adress->physical_adress = NULL;
						if (current_virtual_segment->size <= size) break;
						current_physical_segment = current_physical_segment->next;
					};
					initial_physical_segment->size = current_virtual_segment->size;
					if (size > current_virtual_segment->size) {
						struct p_segment* newSegment = (struct p_segment*)malloc(sizeof(struct p_segment));
						newSegment->size = size - current_virtual_segment->size;
						newSegment->isFree = true;
						newSegment->next = initial_physical_segment->next;
						newSegment->virtual_adress = NULL;
						initial_physical_segment->next = newSegment;
						unionFreePhysicalSegments();
					}
					current_virtual_segment->physical_adress = (struct p_segment*)malloc(sizeof(struct p_segment));
					current_virtual_segment->physical_adress = initial_physical_segment; //initual->adress
					if (initial_physical_segment->virtual_adress != NULL) {
						initial_physical_segment->virtual_adress = NULL;
					}
					initial_physical_segment->virtual_adress = (struct v_segment*)malloc(sizeof(struct v_segment));
					initial_physical_segment->virtual_adress = current_virtual_segment;
					
					//чтение
					memcpy(pBuffer, current_virtual_segment->info, infoSize);
					pBuffer[infoSize] = '\0';
					flag = true;
					break;
				}
			}
		}
		current_virtual_segment = current_virtual_segment->next;
	}
	if (!flag) return UNKNOWN_ERROR;
	return SUCCESSFUL_EXECUTION;
}

//Запись информации в блок памяти
//адрес блока, адрес буфера(куда копируется информация), размер буфера, код ошибки: 0, -1, -2, 1
int _write(VA ptr, void* pBuffer, size_t szBuffer) 
{
	char *newInfo = (char*)pBuffer;
	if (ptr == NULL || strlen(newInfo) < szBuffer || szBuffer == 0) return INCORRECT_PARAMETERS;
	bool flag = false;
	struct v_segment* current_virtual_segment = initial_virtual_segment;
	while (current_virtual_segment != NULL)
	{
		if (ptr == current_virtual_segment->adress) {
			size_t infoSize;
			if (szBuffer > current_virtual_segment->size) return INCORRECT_PARAMETERS;
			
			infoSize = szBuffer;

			if (current_virtual_segment->physical_adress != NULL) {
				//записываем
				if (current_virtual_segment->info != NULL) {
					current_virtual_segment->info = NULL;
				}
				current_virtual_segment->info = (char*)malloc(sizeof(char) * (infoSize));
				memcpy(current_virtual_segment->info, newInfo, infoSize);
				current_virtual_segment->info[infoSize] = '\0';
				flag = true;
				break;
			}
			else {
				struct p_segment* current_physical_segment = initial_physical_segment;
				while (current_physical_segment != NULL)
				{
					if (current_physical_segment->isFree == true && current_physical_segment->size >= current_virtual_segment->size)
					{
						if (current_physical_segment->size >= current_virtual_segment->size) 
						 {
							size_t newBlockSize = current_physical_segment->size - current_virtual_segment->size;
							struct p_segment* newSegment = (struct p_segment*)malloc(sizeof(struct p_segment));
							newSegment->size = newBlockSize;
							newSegment->isFree = true;
							newSegment->next = current_physical_segment->next;
							newSegment->virtual_adress = NULL;
							current_physical_segment->next = newSegment;
							current_physical_segment->size = current_virtual_segment->size;
							unionFreePhysicalSegments();
						}

						current_physical_segment->isFree = false;
						current_physical_segment->virtual_adress->physical_adress = NULL;
						current_physical_segment->virtual_adress = current_virtual_segment;
						current_virtual_segment->physical_adress = (struct p_segment*)malloc(sizeof(struct p_segment));
						current_virtual_segment->physical_adress = current_physical_segment;

						//запись
						if (current_virtual_segment->info != NULL) {
							current_virtual_segment->info = NULL;
						}
						current_virtual_segment->info = (char*)malloc(sizeof(char) * (infoSize));
						char *newInfo = (char*)pBuffer;
						memcpy(current_virtual_segment->info, newInfo, infoSize);
						current_virtual_segment->info[infoSize] = '\0';
					
						flag = true;
						break;
					}
					current_physical_segment = current_physical_segment->next;
				};
				if (!flag) {
					size_t size = 0;
					current_physical_segment = initial_physical_segment;
					while (current_physical_segment != NULL)
					{
						size += current_physical_segment->size;
						current_physical_segment->virtual_adress->physical_adress = NULL;
						if (current_virtual_segment->size <= size) break;
						current_physical_segment = current_physical_segment->next;
					};
					initial_physical_segment->size = current_virtual_segment->size;
					if (size > current_virtual_segment->size) {
						struct p_segment* newSegment = (struct p_segment*)malloc(sizeof(struct p_segment));
						newSegment->size = size - current_virtual_segment->size;
						newSegment->isFree = true;
						newSegment->next = initial_physical_segment->next;
						newSegment->virtual_adress = NULL;
						initial_physical_segment->next = newSegment;
						unionFreePhysicalSegments();
					}
					current_virtual_segment->physical_adress = (struct p_segment*)malloc(sizeof(struct p_segment));
					current_virtual_segment->physical_adress = initial_physical_segment; //initual->adress
					if (initial_physical_segment->virtual_adress != NULL) {
						initial_physical_segment->virtual_adress = NULL;
					}
					initial_physical_segment->virtual_adress = (struct v_segment*)malloc(sizeof(struct v_segment));
					initial_physical_segment->virtual_adress = current_virtual_segment;

					//запись
					if (current_virtual_segment->info != NULL) {
						current_virtual_segment->info = NULL;
					}
					current_virtual_segment->info = (char*)malloc(sizeof(char) * (infoSize));
					char *newInfo = (char*)pBuffer;
					memcpy(current_virtual_segment->info, newInfo, infoSize);
					current_virtual_segment->info[infoSize] = '\0';
					flag = true;
					break;
				}
			}
		}
		current_virtual_segment = current_virtual_segment->next;
	}
	if (!flag) return UNKNOWN_ERROR;
	return SUCCESSFUL_EXECUTION;
}

//количество страниц и размер страницы, код ошибки: 0, -1, 1 
int _init(int n, int szPage) 
{
	if (n <= 0 || szPage <= 0) return INCORRECT_PARAMETERS;

	int virtual_size = n*szPage;

	virtual_memory.size = virtual_size;
	physical_memory.size = (size_t)round(virtual_size / 2);

	virtual_memory.adress = (VA)malloc(virtual_memory.size);

	initial_physical_segment = (struct p_segment*)malloc(sizeof(struct p_segment));

	initial_physical_segment->size = physical_memory.size;
	initial_physical_segment->isFree = true;
	initial_physical_segment->next = NULL;
	initial_physical_segment->virtual_adress = NULL;

	initial_virtual_segment = (struct v_segment*)malloc(sizeof(struct v_segment));

	initial_virtual_segment->adress = virtual_memory.adress;
	initial_virtual_segment->physical_adress = initial_physical_segment;
	initial_virtual_segment->size = virtual_memory.size;
	initial_virtual_segment->isFree = true;
	initial_virtual_segment->next = NULL;
	initial_virtual_segment->info = NULL;

	return SUCCESSFUL_EXECUTION;
}


void unionFreeSegments() {
	struct v_segment* current_virtual_segment = initial_virtual_segment;
	while (current_virtual_segment != NULL) {
		if (current_virtual_segment->isFree = true && current_virtual_segment->next != NULL && current_virtual_segment->next->isFree == true) {
			while (current_virtual_segment->next != NULL && current_virtual_segment->next->isFree == true) {
				current_virtual_segment->size += current_virtual_segment->next->size;
				current_virtual_segment->next = current_virtual_segment->next->next;
			}
		}
		current_virtual_segment = current_virtual_segment->next;
	}
	unionFreePhysicalSegments();
}

unionFreePhysicalSegments() {
	struct p_segment* current_physical_segment = initial_physical_segment;
	while (current_physical_segment != NULL) {
		if (current_physical_segment->isFree = true && current_physical_segment->next != NULL && current_physical_segment->next->isFree == true) {
			while (current_physical_segment->next != NULL && current_physical_segment->next->isFree == true) {
				current_physical_segment->size += current_physical_segment->next->size;
				current_physical_segment->next = current_physical_segment->next->next;
			}
		}
		current_physical_segment = current_physical_segment->next;
	}
}
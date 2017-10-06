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

struct segment* initial_virtual_segment;
struct segment* initial_physical_segment;

bool flag;

//Выделяет блок памяти определенного размера
//адрес блока, размер блока, код ошибки: 0, -1, -2, 1
int _malloc(VA* ptr, size_t szBlock)
{
	if (ptr == NULL) return INCORRECT_PARAMETERS;

	if (physical_memory.size < szBlock) return LACK_OF_MEMORY;

	flag = false;

	struct segment* current_virtual_segment = initial_virtual_segment;
	

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
				
				struct segment * new_virtual_segment = malloc(sizeof(struct segment));
				new_virtual_segment->isFree = true;
				new_virtual_segment->adress = current_virtual_segment->adress + szBlock;
				new_virtual_segment->size = block_size - szBlock;
				new_virtual_segment->next = (struct segment*)malloc(sizeof(struct segment));
				new_virtual_segment->next = current_virtual_segment->next;
				new_virtual_segment->info = NULL;
				if (current_virtual_segment->next == NULL) {
					current_virtual_segment->next = (struct segment*)malloc(sizeof(struct segment));
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

	struct segment* current_physical_segment = initial_physical_segment;

	while (current_physical_segment != NULL) {

		if (current_physical_segment->isFree == true && current_physical_segment->size >= szBlock)
		{
			size_t block_size = current_physical_segment->size;

			if (block_size == szBlock)
			{
				current_physical_segment->isFree = false;
				current_virtual_segment->physical_adress = (struct segment*)malloc(sizeof(struct segment));
				current_virtual_segment->physical_adress = current_physical_segment;
				flag = true;
				break;
			}
			else
			{
				current_physical_segment->size = szBlock;
				current_physical_segment->isFree = false;
			
				struct segment* new_physical_segment = (struct segment*)malloc(sizeof(struct segment));
				new_physical_segment->isFree = true;
				new_physical_segment->adress = current_physical_segment->adress + szBlock;
				new_physical_segment->size = block_size - szBlock;
				new_physical_segment->info = NULL;
				new_physical_segment->next = (struct segment*)malloc(sizeof(struct segment));
				
				new_physical_segment->next = current_physical_segment->next;
				if (current_physical_segment->next == NULL) {
					current_physical_segment->next = (struct segment*)malloc(sizeof(struct segment));
				}
				current_physical_segment->next = new_physical_segment;

				current_virtual_segment->physical_adress = (struct segment*)malloc(sizeof(struct segment));
				current_virtual_segment->physical_adress = current_physical_segment;
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

    struct segment *current_virtual_segment = initial_virtual_segment;
	
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
		struct segment* current_physical_segment = initial_physical_segment;

		while (current_physical_segment != NULL)
		{
			if (current_virtual_segment->physical_adress == current_physical_segment->adress) {
				current_physical_segment->isFree = true;
				current_physical_segment->info= NULL;
				current_virtual_segment->physical_adress = NULL;
				break;
			}
				current_physical_segment = current_physical_segment->next;
		}
	}

	return SUCCESSFUL_EXECUTION;
}

//Чтение информации из блока памяти
//адрес блока, адрес буфера(куда копируется информация), размер буфера, код ошибки: 0, -1, -2, 1
int _read(VA ptr, char* pBuffer, size_t szBuffer)
{
	char* temp = (char*)pBuffer;
	int k = strlen(temp);
	if (ptr == NULL || strlen(temp)<szBuffer || szBuffer <=0) return INCORRECT_PARAMETERS;
	bool flag = false;
	struct segment* current_virtual_segment = initial_virtual_segment;
	while (current_virtual_segment != NULL)
	{
		if (ptr == current_virtual_segment->adress) {
			if (current_virtual_segment->isFree == true || current_virtual_segment->info == NULL) return UNKNOWN_ERROR;
			size_t infoSize;
			if (szBuffer > strlen(current_virtual_segment->info)) {
				return INCORRECT_PARAMETERS;
			}

			else {
				infoSize = szBuffer;
			}
			if (current_virtual_segment->physical_adress != NULL) {
				
				//читаем
				for (int i = 0; i < infoSize; i++) {
					pBuffer[i] = current_virtual_segment->info[i];
				}
				pBuffer[infoSize] = '\0';
				flag = true;
				break;

			}
			else {
				struct segment* current_physical_segment = initial_physical_segment;
				while (current_physical_segment != NULL)
				{
					if (current_physical_segment->isFree == true && current_physical_segment->size >= current_virtual_segment->size)
					{
						if (current_physical_segment->size == current_virtual_segment->size) {
							current_virtual_segment->physical_adress = (struct segment*)malloc(sizeof(struct segment));
							current_virtual_segment->physical_adress = current_physical_segment->adress;
							current_physical_segment->isFree = false;

						}
						else {
							size_t newBlockSize = current_physical_segment->size - current_virtual_segment->size;
							struct segment* newSegment = (struct segment*)malloc(sizeof(struct segment));
							newSegment->adress = current_physical_segment->adress + current_virtual_segment->size;
							newSegment->size = newBlockSize;
							newSegment->isFree = true;
							newSegment->next = current_physical_segment->next;
							current_physical_segment->next = newSegment;
							current_physical_segment->size = current_virtual_segment->size;
							current_physical_segment->isFree = false;
							current_virtual_segment->physical_adress = (struct segment*)malloc(sizeof(struct segment));
							current_virtual_segment->physical_adress = current_physical_segment->adress;
						}
						struct segment * virtual_segment = (struct segment*)malloc(sizeof(struct segment));
						virtual_segment = initial_virtual_segment;
						while (virtual_segment != NULL) {
							if (virtual_segment->physical_adress == current_physical_segment->physical_adress) {
								virtual_segment->physical_adress = NULL;
								break;
							}
						}
						//чтение
						for (int i = 0; i < infoSize; i++) {
							pBuffer[i] = current_virtual_segment->info[i];
						}
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
						struct  segment * virtual_segment = initial_virtual_segment;
						while (virtual_segment != NULL)
						{
							if (virtual_segment->physical_adress == current_physical_segment) {
								virtual_segment->physical_adress = NULL;
								break;
							}
							virtual_segment = virtual_segment->next;
						};
						if (current_virtual_segment->size <= size) break;
						current_physical_segment = current_physical_segment->next;
					};
					initial_physical_segment->size = current_virtual_segment->size;
					if (size > current_virtual_segment->size) {
						struct segment* newSegment = (struct segment*)malloc(sizeof(struct segment));
						newSegment->size = size - current_virtual_segment->size;
						newSegment->adress = initial_physical_segment->adress + current_virtual_segment->size;
						newSegment->isFree = true;
						newSegment->next = initial_physical_segment->next;
						initial_physical_segment->next = newSegment;
					}
					current_virtual_segment->physical_adress = (struct segment*)malloc(sizeof(struct segment));
					current_virtual_segment->physical_adress = initial_physical_segment->adress;
					//чтение
					for (int i = 0; i < infoSize; i++) {
						pBuffer[i] = current_virtual_segment->info[i];
					}
					pBuffer[infoSize] = '\0';
					flag = true;
					break;
				}
			}
		}
		current_virtual_segment = current_virtual_segment->next;
	}
	if (!flag) return UNKNOWN_ERROR;
	else return SUCCESSFUL_EXECUTION;
}

//Запись информации в блок памяти
//адрес блока, адрес буфера(куда копируется информация), размер буфера, код ошибки: 0, -1, -2, 1
int _write(VA ptr, void* pBuffer, size_t szBuffer) 
{
	char *newInfo = (char*)pBuffer;
	if (ptr == NULL || strlen(newInfo) < szBuffer || szBuffer <= 0) return INCORRECT_PARAMETERS;
	bool flag = false;
	struct segment* current_virtual_segment = initial_virtual_segment;
	while (current_virtual_segment != NULL)
	{
		if (ptr == current_virtual_segment->adress) {
			if (current_virtual_segment->isFree == true) return UNKNOWN_ERROR;
			size_t infoSize;
			if (szBuffer > current_virtual_segment->size) {
				return INCORRECT_PARAMETERS;
			}

			else {
				infoSize = szBuffer;
			}
			if (current_virtual_segment->physical_adress != NULL) {

				//записываем
				if (current_virtual_segment->info != NULL) {
					current_virtual_segment->info = NULL;
				}
				current_virtual_segment->info = malloc(sizeof(char) * (infoSize));
				strncpy(current_virtual_segment->info, newInfo, infoSize);
				current_virtual_segment->info[infoSize] = '\0';
				struct segment* physical = current_virtual_segment->physical_adress;
				physical->info = (char*)malloc(sizeof(current_virtual_segment->info));
				physical->info = current_virtual_segment->info;
				flag = true;
				break;

			}
			else {
				struct segment* current_physical_segment = initial_physical_segment;
				while (current_physical_segment != NULL)
				{
					if (current_physical_segment->isFree == true && current_physical_segment->size >= current_virtual_segment->size)
					{
						if (current_physical_segment->size == current_virtual_segment->size) {
							current_virtual_segment->physical_adress = (struct segment*)malloc(sizeof(struct segment));
							current_virtual_segment->physical_adress = current_physical_segment->adress;
							current_physical_segment->isFree = false;

						}
						else {
							size_t newBlockSize = current_physical_segment->size - current_virtual_segment->size;
							struct segment* newSegment = (struct segment*)malloc(sizeof(struct segment));
							newSegment->adress = current_physical_segment->adress + current_virtual_segment->size;
							newSegment->size = newBlockSize;
							newSegment->isFree = true;
							newSegment->next = current_physical_segment->next;
							newSegment->info = NULL;
							current_physical_segment->next = newSegment;
							current_physical_segment->size = current_virtual_segment->size;
							current_physical_segment->isFree = false;
							current_virtual_segment->physical_adress = (struct segment*)malloc(sizeof(struct segment));
							current_virtual_segment->physical_adress = current_physical_segment->adress;
						}
						struct segment * virtual_segment = (struct segment*)malloc(sizeof(struct segment));
						virtual_segment = initial_virtual_segment;
						while (virtual_segment != NULL) {
							if (virtual_segment->physical_adress == current_physical_segment->physical_adress) {
								virtual_segment->physical_adress = NULL;
								break;
							}
						}
						//запись
						if (current_virtual_segment->info != NULL) {
							current_virtual_segment->info = NULL;
						}
						current_virtual_segment->info = malloc(sizeof(char) * (infoSize));
						char *newInfo = (char*)pBuffer;
						strncpy(current_virtual_segment->info, newInfo, infoSize);
						current_virtual_segment->info[infoSize] = '\0';
						struct segment* physical = current_virtual_segment->physical_adress;
						physical->info = current_virtual_segment->info;
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
						struct  segment * virtual_segment = initial_virtual_segment;
						while (virtual_segment != NULL)
						{
							if (virtual_segment->physical_adress == current_physical_segment) {
								virtual_segment->physical_adress = NULL;
								break;
							}
							virtual_segment = virtual_segment->next;
						};
						if (current_virtual_segment->size <= size) break;
						current_physical_segment = current_physical_segment->next;
					};
					initial_physical_segment->size = current_virtual_segment->size;
					if (size > current_virtual_segment->size) {
						struct segment* newSegment = (struct segment*)malloc(sizeof(struct segment));
						newSegment->size = size - current_virtual_segment->size;
						newSegment->adress = initial_physical_segment->adress + current_virtual_segment->size;
						newSegment->isFree = true;
						newSegment->next = initial_physical_segment->next;
						newSegment->info = NULL;
						initial_physical_segment->next = newSegment;
					}
					current_virtual_segment->physical_adress = (struct segment*)malloc(sizeof(struct segment));
					current_virtual_segment->physical_adress = initial_physical_segment->adress;
					//запись
					if (current_virtual_segment->info != NULL) {
						current_virtual_segment->info = NULL;
					}
					current_virtual_segment->info = malloc(sizeof(char) * (infoSize));
					char *newInfo = (char*)pBuffer;
					strncpy(current_virtual_segment->info, newInfo, infoSize);
					current_virtual_segment->info[infoSize] = '\0';
					struct segment* physical = current_virtual_segment->physical_adress;
					physical->info = current_virtual_segment->info;
					flag = true;
					break;
				}
			}
		}
		current_virtual_segment = current_virtual_segment->next;
	}
	if (!flag) return UNKNOWN_ERROR;
	else return SUCCESSFUL_EXECUTION;
}

//количество страниц и размер страницы, код ошибки: 0, -1, 1 
int _init(int n, int szPage) 
{
	if (n <= 0 || szPage <= 0) return INCORRECT_PARAMETERS;

	int virtual_size = n*szPage;

	virtual_memory.size = virtual_size;
	physical_memory.size = (size_t)round(virtual_size / 2);

	virtual_memory.adress = (VA)malloc(virtual_memory.size);
	physical_memory.adress = (VA)malloc(physical_memory.size);

	if (virtual_memory.adress == NULL || physical_memory.adress == NULL) return UNKNOWN_ERROR;

	initial_physical_segment = (struct segment*)malloc(sizeof(struct segment));

	initial_physical_segment->adress = physical_memory.adress;
	initial_physical_segment->info = NULL;
	initial_physical_segment->size = physical_memory.size;
	initial_physical_segment->isFree = true;
	initial_physical_segment->next = NULL;

	initial_virtual_segment = (struct segment*)malloc(sizeof(struct segment));

	initial_virtual_segment->adress = virtual_memory.adress;
	initial_virtual_segment->physical_adress = initial_physical_segment->adress;
	initial_virtual_segment->size = virtual_memory.size;
	initial_virtual_segment->isFree = true;
	initial_virtual_segment->next = NULL;
	initial_virtual_segment->info = NULL;

	return SUCCESSFUL_EXECUTION;
}


void freeSegment(VA* ptr, struct segment current_segment)
{
	do
	{
		if (ptr == current_segment.adress) {
			current_segment.isFree = true;
			free(current_segment.info);
			flag = true;
			break;
		}
		current_segment = *current_segment.next;
	} while (current_segment.next != NULL);
}
#include <stddef.h>
#include <math.h>  
#include <stdio.h>
#include <stdlib.h>
#include "mmemory.h"

struct real_memory memory;  

struct v_segment* initial_virtual_segment;
struct p_segment* initial_physical_segment;

bool flag;
//*ptr = current_virtual_segment->adress;
//Выделяет блок памяти определенного размера
//адрес блока, размер блока, код ошибки: 0, -1, -2, 1

int _malloc(VA* ptr, size_t szBlock)
{
	if (*ptr == NULL || szBlock == 0) return INCORRECT_PARAMETERS;
	if (initial_virtual_segment == NULL || initial_physical_segment == NULL) return UNKNOWN_ERROR;
	if (memory.size < szBlock) return LACK_OF_MEMORY;

	flag = false;

	struct v_segment* current_virtual_segment = initial_virtual_segment;

	while (current_virtual_segment != NULL) {
		if (current_virtual_segment->isFree == true && current_virtual_segment->size >= szBlock) 
		{
			size_t block_size = current_virtual_segment->size;

			if (block_size == szBlock)
			{
				current_virtual_segment->isFree = false;
				if (current_virtual_segment->info == NULL) {
					current_virtual_segment->info = malloc_info(current_virtual_segment->info, current_virtual_segment->size);
				}
				*ptr = current_virtual_segment->adress;
				flag = true;
				break;
			}
			else
			{
				current_virtual_segment->size = szBlock;
				current_virtual_segment->info = malloc_info(current_virtual_segment->info, current_virtual_segment->size);
				current_virtual_segment->isFree = false;
				*ptr = current_virtual_segment->adress;
				
				struct v_segment * new_virtual_segment = (struct v_segment*)malloc(sizeof(struct v_segment));
				new_virtual_segment->isFree = true;
				new_virtual_segment->adress = current_virtual_segment->adress + szBlock;
				new_virtual_segment->size = block_size - szBlock;
				new_virtual_segment->info = NULL;
				new_virtual_segment->next = (struct v_segment*)malloc(sizeof(struct v_segment));
				new_virtual_segment->next = current_virtual_segment->next;
				new_virtual_segment->physical_segment = NULL;
				new_virtual_segment->physical_adress = NULL;

				if (current_virtual_segment->next == NULL) {  
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
				set_links(current_physical_segment, current_virtual_segment);
				flag = true;
				break;
			}
			else
			{
				current_physical_segment->size = szBlock;
				current_physical_segment->isFree = false;
				connect_segments(current_physical_segment, create_new_physical_segment(block_size - szBlock));
				set_links(current_physical_segment, current_virtual_segment);
				flag = true;
				break;
			}
		}
		current_physical_segment = current_physical_segment->next;
	}
	if (!flag) current_virtual_segment->physical_adress = NULL;
	else {
		current_virtual_segment->info = malloc_info(current_virtual_segment->info, current_virtual_segment->size);
		current_physical_segment->info = current_virtual_segment->info;
		current_virtual_segment->physical_adress = current_physical_segment->info;
		current_virtual_segment->physical_segment = current_physical_segment;
	};
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
			current_virtual_segment->physical_segment = NULL;
			current_virtual_segment->physical_adress = NULL;
			//free(current_virtual_segment->info);
			flag = true;
			break;
		}
		current_virtual_segment = current_virtual_segment->next;
	};

	if (!flag) return UNKNOWN_ERROR;

	if (current_virtual_segment->physical_segment != NULL) {
			//free(current_virtual_segment->physical_segment->info);
		current_virtual_segment->physical_segment->info = NULL;
		current_virtual_segment->physical_segment->isFree = true;
		current_virtual_segment->physical_segment->virtual_segment = NULL;
		
	}

	unionFreeSegments();
	return SUCCESSFUL_EXECUTION;
}


//Чтение информации из блока памяти
//адрес блока, адрес буфера(куда копируется информация), размер буфера, код ошибки: 0, -1, -2, 1
int _read(VA ptr, void* pBuffer, size_t szBuffer)
{
	char* temp = (char*)pBuffer;
	if (ptr == NULL || strlen(temp)<szBuffer || szBuffer == 0) return INCORRECT_PARAMETERS;
	flag = false;
	struct v_segment* current_virtual_segment = initial_virtual_segment;
	while (current_virtual_segment != NULL)
	{
		if (ptr >= current_virtual_segment->adress && ptr < (current_virtual_segment->adress + current_virtual_segment->size)) {
			if ((szBuffer + ptr) >(current_virtual_segment->adress + current_virtual_segment->size)) return INCORRECT_PARAMETERS;
			size_t infoSize;
			if (szBuffer > strlen(current_virtual_segment->info)) return INCORRECT_PARAMETERS;
			
			infoSize = szBuffer;
			
			if (current_virtual_segment->physical_segment != NULL) {
				read_information(temp, pBuffer, current_virtual_segment, ptr, infoSize);
				break;
			}
			else {
				struct p_segment* current_physical_segment = initial_physical_segment;
				while (current_physical_segment != NULL)
				{
					if (current_physical_segment->isFree == true && current_physical_segment->size >= current_virtual_segment->size)
					{
						if (current_physical_segment->size > current_virtual_segment->size)
						{
							size_t newBlockSize = current_physical_segment->size - current_virtual_segment->size;
							connect_segments(current_physical_segment, create_new_physical_segment(newBlockSize));
							unionFreePhysicalSegments();
						}
						current_physical_segment->size = current_virtual_segment->size;
						current_physical_segment->isFree = false;

						if (current_physical_segment->virtual_segment != NULL) {
							current_physical_segment->virtual_segment->physical_segment = NULL;
						}
						current_physical_segment->virtual_segment = current_virtual_segment;
						current_virtual_segment->physical_segment = (struct p_segment*)malloc(sizeof(struct p_segment));
						current_virtual_segment->physical_segment = current_physical_segment;
						current_physical_segment->info = current_virtual_segment->info;
						current_virtual_segment->physical_adress = current_physical_segment->info;
						//чтение
						read_information(temp, pBuffer, current_virtual_segment, ptr, infoSize);
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
						if (current_physical_segment->virtual_segment != NULL) {
							current_physical_segment->virtual_segment->physical_segment = NULL;
						}
						if (current_virtual_segment->size <= size) break;
						current_physical_segment = current_physical_segment->next;
					};
					initial_physical_segment->size = current_virtual_segment->size;
					if (size > current_virtual_segment->size) {
						connect_segments(initial_physical_segment, create_new_physical_segment(size - current_virtual_segment->size));
						unionFreePhysicalSegments();
					}
					else {
						initial_physical_segment->next = current_physical_segment->next;
					}
					current_virtual_segment->physical_segment = (struct p_segment*)malloc(sizeof(struct p_segment));
					current_virtual_segment->physical_segment = initial_physical_segment;
					if (initial_physical_segment->virtual_segment != NULL) {
						initial_physical_segment->virtual_segment = NULL;
					}
					initial_physical_segment->virtual_segment = (struct v_segment*)malloc(sizeof(struct v_segment));
					initial_physical_segment->virtual_segment = current_virtual_segment;
					initial_physical_segment->info = current_virtual_segment->info;
					current_virtual_segment->physical_adress = initial_physical_segment->info;
					//чтение
					read_information(temp, pBuffer, current_virtual_segment, ptr, infoSize);
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
	if (ptr == NULL ||pBuffer == NULL|| strlen(newInfo) < szBuffer || szBuffer == 0) return INCORRECT_PARAMETERS;
	flag = false;
	struct v_segment* current_virtual_segment = initial_virtual_segment;
	while (current_virtual_segment != NULL)
	{
		if (ptr >= current_virtual_segment->adress && ptr < (current_virtual_segment->adress + current_virtual_segment->size)) { //туточки
			if ((szBuffer + ptr) > (current_virtual_segment->adress + current_virtual_segment->size)) return INCORRECT_PARAMETERS;
			size_t infoSize;
			if (szBuffer > current_virtual_segment->size) return INCORRECT_PARAMETERS;
			
			infoSize = szBuffer;

			if (current_virtual_segment->physical_segment != NULL) {
				//записываем
				write_information(current_virtual_segment, ptr, newInfo, infoSize);
				break;
			}
			else {
				struct p_segment* current_physical_segment = initial_physical_segment;
				while (current_physical_segment != NULL)
				{
					if (current_physical_segment->isFree == true && current_physical_segment->size >= current_virtual_segment->size)
					{
						if (current_physical_segment->size > current_virtual_segment->size) 
						 {
							size_t newBlockSize = current_physical_segment->size - current_virtual_segment->size;
							connect_segments(current_physical_segment, create_new_physical_segment(newBlockSize));
							unionFreePhysicalSegments();
						}
						current_physical_segment->size = current_virtual_segment->size;
						current_physical_segment->isFree = false;
						if (current_physical_segment->virtual_segment != NULL) {
							current_physical_segment->virtual_segment->physical_segment = NULL;
						}
						current_physical_segment->virtual_segment = current_virtual_segment;
						current_virtual_segment->physical_segment = (struct p_segment*)malloc(sizeof(struct p_segment));
						current_virtual_segment->physical_segment = current_physical_segment;
						//запись
						write_information(current_virtual_segment, ptr, newInfo, infoSize);
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
						if (current_physical_segment->virtual_segment != NULL) {
							current_physical_segment->virtual_segment->physical_segment = NULL;
						}
						if (current_virtual_segment->size <= size) break;
						current_physical_segment = current_physical_segment->next;
					};
					initial_physical_segment->size = current_virtual_segment->size;
					if (size > current_virtual_segment->size) {
						connect_segments(initial_physical_segment, create_new_physical_segment(size - current_virtual_segment->size));
						unionFreePhysicalSegments();
					}
					else {
						initial_physical_segment->next = current_physical_segment->next;
					}
					current_virtual_segment->physical_segment = (struct p_segment*)malloc(sizeof(struct p_segment));
					current_virtual_segment->physical_segment = initial_physical_segment; //initual->adress
					if (initial_physical_segment->virtual_segment != NULL) {
						initial_physical_segment->virtual_segment = NULL;
					}
					initial_physical_segment->virtual_segment = (struct v_segment*)malloc(sizeof(struct v_segment));
					initial_physical_segment->virtual_segment = current_virtual_segment;
					//запись
					write_information(current_virtual_segment, ptr, newInfo, infoSize);
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
	//if (initial_physical_segment != NULL || initial_virtual_segment != NULL) return UNKNOWN_ERROR;
	int virtual_size = n*szPage;

	memory.size = (size_t)round(virtual_size / 2);
	
	memory.adress = (VA)malloc(sizeof(char));

	initial_physical_segment = (struct p_segment*)malloc(sizeof(struct p_segment));

	initial_physical_segment->size = memory.size;
	initial_physical_segment->isFree = true;
	initial_physical_segment->info = NULL;
	initial_physical_segment->next = NULL;
	initial_physical_segment->virtual_segment = NULL;

	initial_virtual_segment = (struct v_segment*)malloc(sizeof(struct v_segment));

	initial_virtual_segment->adress = memory.adress;
	initial_virtual_segment->size = virtual_size;
	initial_virtual_segment->physical_adress = NULL;
	initial_virtual_segment->physical_segment = initial_physical_segment;
	initial_virtual_segment->info = NULL;
	initial_virtual_segment->isFree = true;
	initial_virtual_segment->next = NULL;



	return SUCCESSFUL_EXECUTION;
}


void unionFreeSegments() {
	struct v_segment* current_virtual_segment = initial_virtual_segment;
	while (current_virtual_segment != NULL) {
		if (current_virtual_segment->isFree == true && current_virtual_segment->next != NULL && current_virtual_segment->next->isFree == true) {
			while (current_virtual_segment->next != NULL && current_virtual_segment->next->isFree == true) {
				current_virtual_segment->size += current_virtual_segment->next->size;
				current_virtual_segment->next = current_virtual_segment->next->next;
			}
		}
		current_virtual_segment = current_virtual_segment->next;
	}
	unionFreePhysicalSegments();
}

void unionFreePhysicalSegments() {
	struct p_segment* current_physical_segment = initial_physical_segment;
	while (current_physical_segment != NULL) {
		if (current_physical_segment->isFree == true && current_physical_segment->next != NULL && current_physical_segment->next->isFree == true) {
			while (current_physical_segment->next != NULL && current_physical_segment->next->isFree == true) {
				current_physical_segment->size += current_physical_segment->next->size;
				current_physical_segment->next = current_physical_segment->next->next;
			}
		}
		current_physical_segment = current_physical_segment->next;
	}
}

VA malloc_info(VA info, size_t size) {
	info = (VA)malloc(sizeof(char)*size);
	info[size] = '\0';
	return info;
}

void set_links(struct p_segment* physical_segment, struct v_segment* virtual_segment) {
	virtual_segment->physical_adress = (struct p_segment*)malloc(sizeof(struct p_segment));
	virtual_segment->physical_adress = physical_segment;
	physical_segment->virtual_segment = (struct v_segment*)malloc(sizeof(struct v_segment));
	physical_segment->virtual_segment = virtual_segment;
}

void connect_segments(struct p_segment * first_segment, struct  p_segment * second_segment) {
	if (second_segment->next == NULL) {
		second_segment->next = (struct p_segment*)malloc(sizeof(struct p_segment));
	}
	second_segment->next = first_segment->next;
	if (first_segment->next == NULL) {
		first_segment->next = (struct p_segment*)malloc(sizeof(struct p_segment));
	}
	first_segment->next = second_segment;
}

struct p_segment * create_new_physical_segment(size_t newBlockSize)
{
	struct p_segment* new_physical_segment = (struct p_segment*)malloc(sizeof(struct p_segment));
	new_physical_segment->virtual_segment = NULL;
	new_physical_segment->isFree = true;
	new_physical_segment->size = newBlockSize;
	new_physical_segment->next = NULL;
	new_physical_segment->info = NULL;
	return new_physical_segment;
}

void read_information(char* temp ,void* pBuffer, struct v_segment* current_virtual_segment, VA ptr, size_t infoSize)
{
	memcpy(temp, current_virtual_segment->physical_adress + (ptr - current_virtual_segment->adress), infoSize);
	temp[infoSize] = '\0';
	pBuffer = temp;
	flag = true;
}

void write_information(struct v_segment* current_virtual_segment, VA ptr, char* newInfo, size_t infoSize) {
	memcpy(current_virtual_segment->info + (ptr - current_virtual_segment->adress), newInfo, infoSize);
	current_virtual_segment->physical_segment->info = current_virtual_segment->info;
	current_virtual_segment->physical_adress = current_virtual_segment->physical_segment->info;
	flag = true;
}

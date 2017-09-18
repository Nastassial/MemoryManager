#include "mmemory.h"
#include <stddef.h>

typedef char* VA;				// Тип описывающий адрес блока 
struct real_memory virtual_memory;
struct real_memory physical_memory;
struct segment* virtual_memory_array;
struct segment* physical_memory_array;
int number_of_virtual_segments = 0;
int number_of_physical_segments = 0;

								/**
								@func	_malloc
								@brief	Выделяет блок памяти определенного размера

								@param	[out] ptr		адресс блока
								@param	[in]  szBlock	размер блока

								@return	код ошибки
								@retval	0	успешное выполнение
								@retval	-1	неверные параметры
								@retval	-2	нехватка памяти
								@retval	1	неизвестная ошибка
								**/
int _malloc(VA* ptr, size_t szBlock)
{
	return 1;
}



/**
@func	_free
@brief	Удаление блока памяти

@param	[in] ptr		адресс блока

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	1	неизвестная ошибка
**/
int _free(VA ptr)
{
	return 1;
}



/**
@func	_read
@brief	Чтение информации из блока памяти

@param	[in] ptr		адресс блока
@param	[in] pBuffer	адресс буфера куда копируется инфомация
@param	[in] szBuffer	размер буфера

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	-2	доступ за пределы блока
@retval	1	неизвестная ошибка
**/
int _read(VA ptr, void* pBuffer, size_t szBuffer)
{
	return 1;
}



/**
@func	_write
@brief	Запись информации в блок памяти

@param	[in] ptr		адресс блока
@param	[in] pBuffer	адресс буфера куда копируется инфомация
@param	[in] szBuffer	размер буфера

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	-2	доступ за пределы блока
@retval	1	неизвестная ошибка
**/
int _write(VA ptr, void* pBuffer, size_t szBuffer)
{
	return 1;
}



/**
@func	_init
@brief	Инициализация модели менеджера памяти

@param	[in] n		количество страниц
@param	[in] szPage	размер страницы

В варианте 1 и 2 общий объем памяти расчитывается как n*szPage

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	1	неизвестная ошибка
**/
int _init(int n, int szPage)
{
	if (n <= 0 || szPage <= 0) return -1;
	int virtual_size = n*szPage;
	virtual_memory.adress = (char*)malloc(virtual_size);
	if (virtual_memory.adress == NULL) return 1;
	virtual_memory.size = virtual_size;
	physical_memory.size = round(virtual_size / 2);
	physical_memory.adress = (char*)malloc(physical_memory.size);
	if (physical_memory.adress == NULL) return 1;
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
	return 0;
}
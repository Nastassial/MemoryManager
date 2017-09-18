#include "mmemory.h"
#include <stddef.h>

typedef char* VA;				// ��� ����������� ����� ����� 
struct real_memory virtual_memory;
struct real_memory physical_memory;
struct segment* virtual_memory_array;
struct segment* physical_memory_array;
int number_of_virtual_segments = 0;
int number_of_physical_segments = 0;

								/**
								@func	_malloc
								@brief	�������� ���� ������ ������������� �������

								@param	[out] ptr		������ �����
								@param	[in]  szBlock	������ �����

								@return	��� ������
								@retval	0	�������� ����������
								@retval	-1	�������� ���������
								@retval	-2	�������� ������
								@retval	1	����������� ������
								**/
int _malloc(VA* ptr, size_t szBlock)
{
	return 1;
}



/**
@func	_free
@brief	�������� ����� ������

@param	[in] ptr		������ �����

@return	��� ������
@retval	0	�������� ����������
@retval	-1	�������� ���������
@retval	1	����������� ������
**/
int _free(VA ptr)
{
	return 1;
}



/**
@func	_read
@brief	������ ���������� �� ����� ������

@param	[in] ptr		������ �����
@param	[in] pBuffer	������ ������ ���� ���������� ���������
@param	[in] szBuffer	������ ������

@return	��� ������
@retval	0	�������� ����������
@retval	-1	�������� ���������
@retval	-2	������ �� ������� �����
@retval	1	����������� ������
**/
int _read(VA ptr, void* pBuffer, size_t szBuffer)
{
	return 1;
}



/**
@func	_write
@brief	������ ���������� � ���� ������

@param	[in] ptr		������ �����
@param	[in] pBuffer	������ ������ ���� ���������� ���������
@param	[in] szBuffer	������ ������

@return	��� ������
@retval	0	�������� ����������
@retval	-1	�������� ���������
@retval	-2	������ �� ������� �����
@retval	1	����������� ������
**/
int _write(VA ptr, void* pBuffer, size_t szBuffer)
{
	return 1;
}



/**
@func	_init
@brief	������������� ������ ��������� ������

@param	[in] n		���������� �������
@param	[in] szPage	������ ��������

� �������� 1 � 2 ����� ����� ������ ������������� ��� n*szPage

@return	��� ������
@retval	0	�������� ����������
@retval	-1	�������� ���������
@retval	1	����������� ������
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
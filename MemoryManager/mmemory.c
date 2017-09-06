#include "mmemory.h"
#include <stddef.h>

typedef char* VA;				// ��� ����������� ����� ����� 

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
	return 1;
}
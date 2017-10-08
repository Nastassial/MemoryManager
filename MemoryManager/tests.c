#include <stddef.h> 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tests.h"
#include "mmemory.h"

//_init()

void test_init_success()
{
	assert(_init(2, 40) == SUCCESSFUL_EXECUTION);
}

void test_init_incorrect_first_param()
{
	assert(_init(-2, 30) == INCORRECT_PARAMETERS);
}

void test_init_incorrect_second_param()
{
	assert(_init(4, -5) == INCORRECT_PARAMETERS);
}


//_malloc()

void test_malloc_incorrect_param() 
{
	VA adress;
	_init(2, 40);
	assert(_malloc(&adress, 0) == INCORRECT_PARAMETERS);
}

void test_malloc_null_pointer() 
{
	VA adress = NULL;
	_init(2, 40);
	assert(_malloc(&adress, 10) == INCORRECT_PARAMETERS);
}

void test_malloc_undefined_init() 
{
	VA adress;
	//_init(2, 40);
	assert(_malloc(&adress, 20) == UNKNOWN_ERROR);
}

void test_malloc_lack_of_memory() 
{
	VA adress;
	_init(2, 40);
	assert(_malloc(&adress, 50) == LACK_OF_MEMORY);
}

void test_malloc_no_enough_memory() 
{
	VA adress;
	VA some;
	VA another;
	_init(2, 40);
	_malloc(&adress, 40);
	_malloc(&some, 40);
	assert(_malloc(&another, 50) == LACK_OF_MEMORY);
}

void test_malloc_success() 
{
	VA adress;
	_init(2, 40);
	assert(_malloc(&adress, 20) == SUCCESSFUL_EXECUTION);
}


//_free()

void test_free_nullptr() 
{
	VA adress = NULL;
	_init(2, 40);
	_malloc(&adress, 20);
	assert(_free(adress) == INCORRECT_PARAMETERS);
}

void test_free_undefined_malloc() 
{
	VA adress = "00010";
	_init(2,30);
	assert(_free(adress) == UNKNOWN_ERROR);
}

void test_free_success()
{
	VA adress;
	_init(3,20);
	_malloc(&adress, 20);
	assert(_free(adress) == SUCCESSFUL_EXECUTION);
}


//_write()

void test_write_incorrect_first_param()
{
	VA first_adress;
	VA second_adress;
	VA third_adress = NULL;
	char str[] = "symbols";
	_init(3, 20);
	_malloc(&first_adress, 20);
	_malloc(&second_adress, 10);
	assert(_write(third_adress,str,3) == INCORRECT_PARAMETERS);
}

void test_write_incorrect_second_param()
{
	VA first_adress;
	VA second_adress;
	char str[] = "symbols";
	_init(3, 20);
	_malloc(&first_adress, 20);
	_malloc(&second_adress, 10);
	assert(_write(first_adress, str, 15) == INCORRECT_PARAMETERS);
}

void test_write_incorrect_third_param()
{
	VA first_adress;
	VA second_adress;
	char str[] = "symbols";
	_init(3, 20);
	_malloc(&first_adress, 20);
	_malloc(&second_adress, 10);
	assert(_write(first_adress, str, 0) == INCORRECT_PARAMETERS);
}

void test_write_big_szbuffer() //пытаемся записать больше, чем есть места в сегменте
{
	VA first_adress;
	char str[] = "symbols";
	_init(3, 20);
	_malloc(&first_adress, 4);
	assert(_write(first_adress, str, 6) == INCORRECT_PARAMETERS);
}

void test_write_undefined_malloc() 
{
	VA adress = "01001";
	char str[] = "symbols";
	_init(3, 20);
	assert(_write(adress, str, 5) == UNKNOWN_ERROR);
}

void test_write_success()
{
	VA first_adress;
	VA second_adress;
	char str[] = "symbols";
	_init(3, 20);
	_malloc(&first_adress, 7);
	_malloc(&second_adress, 10);
	assert(_write(second_adress, str, 7) == SUCCESSFUL_EXECUTION);
}


//_read()

void test_read_incorrect_first_param()
{
	VA adress = NULL;
	char buff[] = "";
	char str[] = "symbols";
	assert(_read(adress, buff, 3) == INCORRECT_PARAMETERS);
}

void test_read_incorrect_second_param()
{
	VA adress = "010110";
	char buff[] = "";
	char str[] = "symbols";
	assert(_read(adress, buff, 15) == INCORRECT_PARAMETERS);
}

void test_read_incorrect_third_param()
{
	VA adress = "001010";
	char buff[] = "";
	char str[] = "symbols";
	assert(_read(adress, buff, 0) == INCORRECT_PARAMETERS);
}

void test_read_big_szbuffer() 
{
	VA first_adress;
	VA second_adress;
	char buff[] = "somebigtext";
	char str[] = "symboltext";
	_init(3, 20);
	_malloc(&first_adress, 15);
	_malloc(&second_adress, 10);
	_write(second_adress, str, 5);
	assert(_read(second_adress, buff, 10) == INCORRECT_PARAMETERS);
}

void test_read_undefined_write()
{
	VA first_adress;
	VA second_adress;
	char buff[] = "sometext";
	char str[] = "symbols";
	_init(3, 20);
	_malloc(&first_adress, 15);
	_malloc(&second_adress, 10);
	assert(_read(first_adress, buff, 5) == UNKNOWN_ERROR);
}

void test_read_success()
{
	VA adress;
	char str[] = "symbols";
	char buff[] = "qwertyuiop";
	_init(3, 20);
	_malloc(&adress, 12);
	_write(adress, str, 6);
	assert(_read(adress, buff, 4) == SUCCESSFUL_EXECUTION);
}

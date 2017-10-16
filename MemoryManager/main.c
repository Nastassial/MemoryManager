#include <stdio.h>
#include "tests.h"

void main()
{
	char* adress;
	char str[] = "symbols";
	char buff[] = "qwertyuiop";
	_init(3, 20);
	_malloc(&adress, 12);
	_write(adress, str, 6);
	_read(adress, buff, 4);
	//test_init_success();
	//test_init_incorrect_first_param();
	//test_init_incorrect_second_param();
	//test_malloc_incorrect_param();
	//test_malloc_null_pointer();
	//test_malloc_undefined_init();
	//test_malloc_lack_of_memory();
	//test_malloc_no_enough_memory();
	//test_malloc_null_physical_adress();
	test_malloc_success();
	//test_free_nullptr();
	//test_free_undefined_malloc();
	//test_free_null_physical_adress();
	//test_free_union_of_free_segments();
	//test_free_success();
	//test_write_incorrect_first_param();
	//test_write_incorrect_second_param();
	//test_write_incorrect_third_param();
	//test_write_big_szbuffer();
	//test_write_undefined_malloc();
//	test_write_null_physical_segment_free_memory();
	//test_write_null_physical_segment_no_free_memory();
	//test_write_null_physical_segment_all_memory_busy();
	//test_write_success();
	//test_read_incorrect_first_param();
	//test_read_incorrect_second_param();
	//test_read_incorrect_third_param();
	//test_read_big_szbuffer();
	//test_read_undefined_write();
	//test_read_null_physical_segment_free_memory();
	//test_read_success();

}
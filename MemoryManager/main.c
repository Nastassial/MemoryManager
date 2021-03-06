#include <stdio.h>
#include "mmemory.h""
#include "tests.h"

void main()
{
	test_malloc_undefined_init();
	test_init_success();
	test_init_incorrect_first_param();
	test_init_incorrect_second_param();
	test_malloc_incorrect_param();
	test_malloc_null_pointer();
	test_malloc_lack_of_memory();
	test_malloc_no_enough_memory();
	test_malloc_null_physical_adress();
	test_malloc_success();
	test_free_nullptr();
	test_free_undefined_malloc();
	test_free_null_physical_adress();
	test_free_union_of_free_segments();
	test_free_success();
	test_write_incorrect_first_param();
	test_write_incorrect_second_param();
	test_write_incorrect_third_param();
	test_write_big_szbuffer();
	test_write_undefined_malloc();
	test_write_null_physical_segment_free_memory();
	test_write_null_physical_segment_no_free_memory();
	test_write_null_physical_segment_all_memory_busy();
	test_write_success();
	test_read_incorrect_first_param();
	test_read_incorrect_second_param();
	test_read_incorrect_third_param();
	test_read_big_szbuffer();
	test_read_null_physical_segment_no_free_memory();
	test_read_success();
	test_read_info();
	load_test();
	load_test_segments_number();
}
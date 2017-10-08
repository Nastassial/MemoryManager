#ifndef TESTS_H
#define TESTS_H

void test_init_success();
void test_init_incorrect_first_param();
void test_init_incorrect_second_param();
//test_init_unknown_error

void test_malloc_incorrect_param();
void test_malloc_null_pointer();
void test_malloc_undefined_init();
void test_malloc_lack_of_memory();
void test_malloc_no_enough_memory();
void test_malloc_success();

void test_free_nullptr();
void test_free_undefined_malloc();
void test_free_success();

void test_write_incorrect_first_param();
void test_write_incorrect_second_param();
void test_write_incorrect_third_param();
void test_write_big_szbuffer();
void test_write_undefined_malloc();
void test_write_success();

void test_read_incorrect_first_param();
void test_read_incorrect_second_param();
void test_read_incorrect_third_param();
void test_read_big_szbuffer();
void test_read_success();
void test_read_undefined_write();

#endif //TEST_H

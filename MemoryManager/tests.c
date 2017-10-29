#include <stddef.h> 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "tests.h"
#include <conio.h>
#include <io.h>
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
	assert(_malloc(&another, 30) == LACK_OF_MEMORY);
}

void test_malloc_success() 
{
	VA adress;
	_init(2, 40);
	assert(_malloc(&adress, 20) == SUCCESSFUL_EXECUTION);
}

void test_malloc_null_physical_adress()
{
	VA adress;
	VA some_adress;
	_init(2, 40);
	_malloc(&adress, 40);
	assert(_malloc(&some_adress, 40) == SUCCESSFUL_EXECUTION);
}


//_free()

void test_free_nullptr() 
{
	VA adress = NULL;
	assert(_free(adress) == INCORRECT_PARAMETERS);
}

void test_free_undefined_malloc() 
{
	VA adress = "undefined";
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

void test_free_null_physical_adress()
{
	VA adress;
	VA another_adress;
	_init(3,20);
	_malloc(&adress, 30);
	_malloc(&another_adress, 20);
	assert(_free(another_adress) == SUCCESSFUL_EXECUTION);
}

void test_free_union_of_free_segments()
{
	VA first_adress;
	VA second_adress;
	_init(5, 20);
	_malloc(&first_adress, 50);
	_malloc(&second_adress, 25);
	_free(second_adress);
	assert(_malloc(&second_adress, 50) == SUCCESSFUL_EXECUTION);
}


//_write()

void test_write_incorrect_first_param()
{
	VA third_adress = NULL;
	char str[] = "symbols";
	assert(_write(third_adress,str,3) == INCORRECT_PARAMETERS);
}
/*
void test_write_incorrect_second_param()
{
	VA first_adress;
	char* str = NULL;
	_init(3, 20);
	_malloc(&first_adress, 20);
	assert(_write(first_adress, str, 15) == INCORRECT_PARAMETERS);
}
*/

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

void test_write_null_physical_segment_free_memory()
{
	VA first_adress;
	VA second_adress;
	VA third_adress;
	char str[] = "symbols";
	_init(2, 20);
	_malloc(&first_adress, 10);
	_malloc(&second_adress, 10);
	_malloc(&third_adress, 10);
	_free(second_adress);
	assert(_write(third_adress, str, 7) == SUCCESSFUL_EXECUTION);
}

void test_write_null_physical_segment_no_free_memory()
{
	VA first_adress;
	VA second_adress;
	VA third_adress;
	char str[] = "symbols";
	_init(2, 20);
	_malloc(&first_adress, 7);
	_malloc(&second_adress, 10);
	_malloc(&third_adress, 10);
	_free(second_adress);
	assert(_write(third_adress, str, 7) == SUCCESSFUL_EXECUTION);
}

void test_write_null_physical_segment_all_memory_busy()
{
	VA first_adress;
	VA second_adress;
	VA third_adress;
	char str[] = "symbols";
	_init(2, 20);
	_malloc(&first_adress, 7);
	_malloc(&second_adress, 10);
	_malloc(&third_adress, 10);
	assert(_write(third_adress, str, 7) == SUCCESSFUL_EXECUTION);
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
	_malloc(&second_adress, 9);
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

void test_read_null_physical_segment_no_free_memory()
{
	VA first_adress;
	VA second_adress;
	VA third_adress;
	char str[] = "symbols";
	char buff[] = "asdffhgj";
	_init(2, 20);
	_malloc(&first_adress, 7);
	_malloc(&second_adress, 10);
	_malloc(&third_adress, 8);
	_write(third_adress, str, 7);
	assert(_read(third_adress, buff, 5) == SUCCESSFUL_EXECUTION);
}

void test_read_info() {
	char * some;
	char buff[] = "qwertyuiop";
	char result[] = "fgskvusbuite";
	_init(2, 20);
	_malloc(&some, 20);
	_write(some + 2, buff, 10);
	assert(_read(some + 1, result, 12)== SUCCESSFUL_EXECUTION);
}


//load


void load_test(){
	FILE* result;
	result = fopen("result_1.txt", "w");
	fprintf(result, "N\t\\t\ts/S\n");
	int numberOfSegments = 500;
	size_t sizes[500];
	VA adresses[500];
	size_t rand_size;
	int numberOfSegment = 0;
	size_t maxSize = 0;
	size_t freeSize = 0;
	size_t maxFreeSize = 0;
	int numberOfFreeSegments = 0;
	srand(time(NULL));
	for (int i = 0; i < numberOfSegments; i++) {
		rand_size = 1 + rand() % 10;
		sizes[i] = rand_size;
		maxSize += rand_size;
	}
	_init(2, maxSize);
	for (int i = 0; i < numberOfSegments; i++) {
		VA adress;
		_malloc(&adress, sizes[i]);
		adresses[i] = adress;
	}
	while (numberOfSegment< numberOfSegments) {
		_free(adresses[numberOfSegment]);
		adresses[numberOfSegment] = NULL;
		freeSize += sizes[numberOfSegment];
		if (numberOfSegment != 0) {
			int prev = numberOfSegment-1;
			if (adresses[prev] == NULL) {
				sizes[numberOfSegment] += sizes[prev];
			}
		}
		if (sizes[numberOfSegment] > maxFreeSize) {
			maxFreeSize = sizes[numberOfSegment];
		}
		numberOfSegment += 1 + rand() % 2;
		numberOfFreeSegments++;
		double res =(double) maxFreeSize/freeSize;
		fprintf(result, "%d\t\\t\t%f\n", numberOfFreeSegments, res);
	}
	numberOfSegment = 1;
	while (numberOfSegment < numberOfSegments) {
		if (adresses[numberOfSegment] != NULL) {
			_free(adresses[numberOfSegment]);
			adresses[numberOfSegment] = NULL;
			freeSize += sizes[numberOfSegment];
			int prev = numberOfSegment-1;
			if (adresses[prev] == NULL) {
					sizes[numberOfSegment] += sizes[prev];
				}
				prev += 2;
				if (prev<numberOfSegments && adresses[prev] == NULL) {
					while (adresses[prev] == NULL) {
						prev++;
					}
					prev--;
					sizes[numberOfSegment] += sizes[prev];
					sizes[prev] = sizes[numberOfSegment];
			}
			if (sizes[numberOfSegment] > maxFreeSize) {
				maxFreeSize = sizes[numberOfSegment];
			}
			numberOfFreeSegments++;
			double res = (double) maxFreeSize/freeSize;
			fprintf(result, "%d\t\\t\t%f\n", numberOfFreeSegments, res);
		}
		numberOfSegment++;
	}
	fclose(result);
}

void load_test_segments_number() {
	FILE* result;
	result = fopen("result_2.txt", "w");
	fprintf(result, "N\t\\t\tn/N\n");
	int numberOfSegments = 500;
	size_t sizes[500];
	VA adresses[500];
	size_t rand_size;
	size_t size = 0;
	int segmentsNumber = numberOfSegments;
	int numberOfFreeSegments = 1;
	int numberOfSegment = 0;
	int freeSegments = 0;
	srand(time(NULL));
	for (int i = 0; i < numberOfSegments; i++) {
		rand_size = 1 + rand() % 10;
		sizes[i] = rand_size;
		size += rand_size;
	}
	_init(2, size);
	for (int i = 0; i < numberOfSegments; i++) {
		VA adress;
		_malloc(&adress, sizes[i]);
		adresses[i] = adress;
	}
	while (numberOfSegment< numberOfSegments) {
		_free(adresses[numberOfSegment]);
		freeSegments++;
		adresses[numberOfSegment] = NULL;
		if (numberOfSegment != 0) {
			int prev = numberOfSegment - 1;
			if (adresses[prev] != NULL) {
				numberOfFreeSegments++;
			}
			else {
				segmentsNumber--;
			}
		}
		numberOfSegment += 1 + rand() % 2;
		double res = (double)numberOfFreeSegments / segmentsNumber;
		fprintf(result, "%d\t\\t\t%f\n", freeSegments, res);
	}

	numberOfSegment = 1;

	while (numberOfSegment < numberOfSegments) {
		if (adresses[numberOfSegment] != NULL) {
			_free(adresses[numberOfSegment]);
			freeSegments++;
			adresses[numberOfSegment] = NULL;
			int next = numberOfSegment + 1;
			if (next<numberOfSegments && adresses[next] == NULL) {
				numberOfFreeSegments--;
				if (adresses[numberOfSegment - 1] == NULL) {
				segmentsNumber--;
				}
				segmentsNumber--;
			}
			double res = (double)numberOfFreeSegments / segmentsNumber;
			fprintf(result, "%d\t\\t\t%f\n", freeSegments, res);
		}
		numberOfSegment++;
	}
	fclose(result);
}
#include <stdio.h>
#include "mmemory.h"

void main()
{
	_init(2, 40);
	VA some_address;
	VA new_adress;
	VA third_adress;
	VA prof;
	char str[] = "sergey";
	char temp[] = "fgjfmj";
	_malloc(&some_address,20);
	_malloc(&new_adress, 20);
	_malloc(&third_adress, 20);
	_malloc(&prof, 30);
	_write(some_address, str, 3);
	_write(third_adress, str, 4);
	_read(some_address, temp, 2);
	printf("%s", temp);
	getch();
}
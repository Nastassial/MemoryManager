#include <stdio.h>
#include "mmemory.h"

void main()
{
	_init(2, 40);
	VA some_address;
	VA new_adress;
	VA third_adress;
	char str[] = "sergey";
	_malloc(&some_address,20);
	_malloc(&new_adress, 20);
	_malloc(&third_adress, 20);
	_write(third_adress, str, sizeof(str));
	//_malloc(&new_adress, 40);
	//_free(some_address);
	//_malloc(&some_address, 40);
	//_write();
	//_free(some_address);
	getch();
}
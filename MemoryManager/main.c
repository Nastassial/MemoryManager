#include <stdio.h>
#include "mmemory.h"

void main()
{
	_init(2, 40);
	VA some_address;
	_malloc(&some_address, 40);
	//_malloc(&some_address, 40);
	//_write();
	_free(some_address);
	_getch();
}
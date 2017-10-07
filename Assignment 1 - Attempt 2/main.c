#include <stdio.h>

#include "structs.h"
#include "headersOne.h"
#include "headersTwo.h"
#include "headersOther.h"

#include "functionsOne.c"
#include "functionsTwo.c"
#include "functionsOther.c"

int main( int argc, char *argv[] )  {

	if( argc == 2 ) {
		printf("The argument supplied is %s\n", argv[1]);
	}
	else if( argc > 2 ) {
		printf("Too many arguments supplied.\n");
	}
	else {
		printf("One argument expected.\n");
	}

	return 1;
}
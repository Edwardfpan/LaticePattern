// Edward Pan, edwardfpan@yahoo.com, Feb 1, 2016
// For XXX Inc.
// Rev 2, Feb 5, 2016

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "LatticePattern.h"
extern int NoOfP_o, NoOfP_n;
int LatticeSize = -1;
void main(int argc, char *argv[])
{

#if 0
	switch (argc) {
		case 2:
			LatticeSize = atoi(argv[1]);
			break;
		default:
			printf("LatticePattern.exe LatticeSize\n");
			exit(1);
	}

	LatticePattern(LatticeSize);

#else
	int input;

	while (LatticeSize != 0 )
	{
		printf("Please enter the block size ('0' to exit): ");
	
		scanf("%d",&LatticeSize);

		if (LatticeSize >0) 
		{
			printf("\n");

			LatticePattern(LatticeSize);

			printf("\nThe total number of blocks for Square %dx%x is %d\n\n", LatticeSize, LatticeSize, NoOfP_n);
		}
		else if (LatticeSize < 0) 
		{
			LatticeSize = 0;
			printf("Must be positive digits!\n\n");
		}
		else
		{
			printf("Thank you very much for using the software. Bye Bye!\n\n");
		}
	}
#endif

}
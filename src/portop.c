#include <msp430f5529.h>
#include <stdio.h>
#include <stdlib.h>
#include "portop.h"

int fnPBitExtract(volatile int iPort, int iBit) //Checks the value of a bit (i.e fnBitExtract(P2IN,BIT3))
{
	if(iPort & iBit)
			return 1;
		else
			return 0;
}

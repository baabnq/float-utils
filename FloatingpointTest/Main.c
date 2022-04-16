
#include <stdio.h>
#include "Float.h"



void printFp(float16 value)
{
	printf("Mantissa: %d\n", getMantissa(value));
	printf("Exponent: %d\n", getExponent(value));
	printf("Sign    : %d\n", value & SIGN_MASK ? 1 : 0);
	printf("\n");

}




int main()
{


	float16 fp1 = value2fp(50);
	float16 fp2 = value2fp(20);
	fp1 |= SIGN_MASK;

	float16 result = add(fp1, fp2);

	/*
	printFp(fp1);
	printFp(fp2);
	printFp(result);
	*/

	printf("%d\n", fp2value(result));


	return 0;
}

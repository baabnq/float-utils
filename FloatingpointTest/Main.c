
#include <stdio.h>
#include "Float.h"



void printFp(float16 value)
{
	printf("Mantissa: %x\n", getMantissa(value));
	printf("Exponent: %x\n", getExponent(value));
	printf("Sign    : %x\n", value & SIGN_MASK ? 1 : 0);
	printf("\n");

}




int main()
{


	float16 fp1 = value2fp(10);
	float16 fp2 = value2fp(5);

	float16 test = add(fp1, fp2);

	printFp(test);
	printf("%d\n", fp2value(test));


	return 0;
}

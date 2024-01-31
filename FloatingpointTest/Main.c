#pragma warning (disable : 4996 )

#include <stdio.h>
#include "Float.h"
#include <stdbool.h>

int mainOFF()
{
	//float16 f = root10(value2fp(1000));
	//printf("(int)f: %hu\n", f);
	//printFp(f);

	//return 0;
}


union floatCast
{
	float f;
	struct
	{
		unsigned int mantisa  : 23;
		unsigned int exponent : 8;
		unsigned int sign     : 1;
	} parts;
};



int main(int argc, char* argv[])
{
	//simple converter
	if (argc != 3)
	{
		printf("Invaild arguments\n");
		return 0;
	}


	char mode = *argv[1];
	float16 f = 0;
	union floatCast val;

	switch (mode)
	{
	case 'f':; //to float
		//"ignoring return value of sscanf", how 'bout that
		sscanf(argv[2], "%f", &(val.f));

		setMantissa(&f, val.parts.mantisa >> (23 - 7));
		setExponent(&f, val.parts.exponent - 127 - 7);
		f |= val.parts.sign ? SIGN_MASK : 0;

		//f = value2fp(val);
		printf("val:    %f\n", val.f);
		printf("(int)f: %hu\n", f);

		printf("\n\n--- details ---\n");
		printFp(f);
		break;

	case 'v':; //to value
		sscanf(argv[2], "%hu", &f);

		unsigned short v = fp2value(f);
		printf("(int)f: %hu\n", f);
		printf("val: %hu\n", v);

		printf("\n\n--- details ---\n");
		printFp(f);

		break;


	}

	return 0;
}

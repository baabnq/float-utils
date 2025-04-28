
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

#include "float.c"


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

    int opt;
    char* param = 0;
    enum
    {
        PARAM_MODE_ENCODE,
        PARAM_MODE_DECODE,
    } mode;

    while ((opt = getopt(argc, argv, "de")) != -1)
        switch(opt)
        {
            case 'd': mode = PARAM_MODE_DECODE; break;
            case 'e': mode = PARAM_MODE_ENCODE; break;
            default:
                fprintf(stderr, "Usage: %s [-e real] [-d float]\n", argv[0]);
                exit(EXIT_FAILURE); 
        }


    switch (mode)
    {
        case PARAM_MODE_ENCODE:
            printf("encode\n");
            break;
        case PARAM_MODE_DECODE:
            printf("decode\n");
            break;
    }

    return 0;

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

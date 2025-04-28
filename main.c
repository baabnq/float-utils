
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

#include "float.c"







float16 encode(float in)
{
    union ieee_decompose
    {
        float encoded;
        struct 
        {
            uint32_t man : 23;
            uint32_t exp : 8;
            uint32_t sgn : 1;
        };
    } parts = (union ieee_decompose)in;


    //true absolute zero
    if (in == 0.0f)
        return value2fp(0); 
    

    float16 out = 0;
    setMantissa(&out, parts.man >> (23 - 7));
    setExponent(&out, parts.exp - 127 - 7);
    out |= parts.sgn ? SIGN_MASK : 0;

    return out;
}


float decode(float16 in)
{
	uint16_t man = getMantissa(in);
     int16_t exp = getExponent(in);

    float sign = (in & SIGN_MASK ? -1 : 1);
    return man * pow(2.0f, exp) * sign;
}


float decompose(float16 value)
{
	printf("mantissa: %d\n", getMantissa(value));
	printf("exponent: %d\n", getExponent(value));
	printf("sign    : %c\n", value & SIGN_MASK ? '-' : '+');
	printf("\n");

	printf("decoded: %f\n\n", decode(value));
}



void usage(char* name)
{
    fprintf(stderr, "Usage: %s [-h] [-e real] [-d float]\n", name);
    exit(EXIT_FAILURE); 
}




int main(int argc, char* argv[])
{

    enum
    {
        PARAM_MODE_DEFAULT,
        PARAM_MODE_ENCODE,
        PARAM_MODE_DECODE,
    } mode = PARAM_MODE_DEFAULT;

    const char* argfmt = "hd:e:";
    float real;
    float16 code;


    while (true)
        switch(getopt(argc, argv, argfmt))
        {
            case -1: goto arg_parse_done;
            case 'e':
                mode = PARAM_MODE_ENCODE;
                sscanf(optarg, "%f", &real);
                break;
            case 'd':
                mode = PARAM_MODE_DECODE;
                sscanf(optarg, "%hu", &code);
                break;

            case 'h':
            default:
                usage(argv[0]);
        }
    arg_parse_done:;



	switch (mode)
	{
        case PARAM_MODE_ENCODE:
            code = encode(real);
            printf("--- overview ---\n");
            printf("real: %f\n", real);
            printf("code: %hu\n", code);
            printf("native: %hu\n", fp2value(code));

            printf("\n");
            printf("--- encoding decomposition  ---\n");
            decompose(code);

            printf("--- advanced ---\n");
            printf("encoding error: %f\n", fabs(decode(code) - real));

            break;

        case PARAM_MODE_DECODE:
            real = decode(code);
            printf("--- overview ---\n");
            printf("real: %f\n", real);
            printf("code: %hu\n", code);
            printf("native: %hu\n", fp2value(code));

            printf("\n");
            printf("--- encoding decomposition  ---\n");
            decompose(code);

            break;
            
        case PARAM_MODE_DEFAULT:
            usage(argv[0]);

	}

	return 0;
}

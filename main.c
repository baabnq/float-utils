
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
    fprintf(stderr, "Usage: %s [-h] [-e real] [-d float] [-p]\n", name);
    exit(EXIT_FAILURE); 
}




int main(int argc, char* argv[])
{

    enum
    {
        PARAM_BEHAVE_DEFAULT,
        PARAM_BEHAVE_ENCODE,
        PARAM_BEHAVE_DECODE,
    } behaviour = PARAM_BEHAVE_DEFAULT;

    enum
    {
        PARAM_OUTPUT_FULL,
        PARAM_OUTPUT_PIPE,
    } output = PARAM_OUTPUT_FULL;

    const char* argfmt = "hd:e:p";
    float real;
    float16 code;


    while (true)
        switch(getopt(argc, argv, argfmt))
        {
            case -1: goto arg_parse_done;
            case 'e':
                behaviour = PARAM_BEHAVE_ENCODE;
                sscanf(optarg, "%f", &real);
                break;
            case 'd':
                behaviour = PARAM_BEHAVE_DECODE;
                sscanf(optarg, "%hu", &code);
                break;

            case 'p':
                output = PARAM_OUTPUT_PIPE;
                break;

            case 'h':
            default:
                usage(argv[0]);
        }
    arg_parse_done:;



	switch (behaviour)
	{
        case PARAM_BEHAVE_ENCODE: code = encode(real); break;
        case PARAM_BEHAVE_DECODE: real = decode(code); break;
        case PARAM_BEHAVE_DEFAULT: usage(argv[0]);
	}

    switch(output)
    {
        case PARAM_OUTPUT_FULL:
            printf("--- overview ---\n");
            printf("real: %f\n", real);
            printf("code: %hu\n", code);
            printf("native: %hu\n", fp2value(code));

            printf("\n");
            printf("--- encoding decomposition  ---\n");
            decompose(code);

            if (behaviour == PARAM_BEHAVE_ENCODE)
            {
                printf("--- advanced ---\n");
                float errFull = fabs(decode(code) - real);
                float errRate = errFull / real;
                float accRate = 1 - errRate;

                #define precent_render(x) (x * 100.0f)
                printf("encoding error full: %f\n\n", errFull);
                printf("encoding error rate: %f%%\n", precent_render(errRate));
                printf("encoding accuracy  : %f%%\n", precent_render(accRate));
                #undef precent_render
            }

            break;

        case PARAM_OUTPUT_PIPE:
            switch(behaviour)
            {
                case PARAM_BEHAVE_ENCODE: printf("%d\n", code); break;
                case PARAM_BEHAVE_DECODE: printf("%f\n", real); break;
                case PARAM_BEHAVE_DEFAULT: break;
            }

    }



	return 0;
}

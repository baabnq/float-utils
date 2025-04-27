
#ifndef _FLOAT_H
#define _FLOAT_H

typedef unsigned short float16;

#define px(x)			printf("%x\n", x);


#define upper16			0b1111111100000000
#define posNaN			0b0011111110000000

#define MANTISSA_SIZE   8
#define EXPONENT_SIZE   8
#define SIGN_SIZE       15
#define MANTISSA_MASK   (short)0b0000000001111111
#define EXPONENT_MASK   (short)0b0000000011111111
#define SIGN_MASK       (short)0b1000000000000000
#define MIN_SIGNED_CHAR -(1 << (8 - 1))


#define PI 0b0111110101001001

union floatDec
{
	struct 
	{
		char man;
		char exp;
	} parts;

	unsigned short value;
};


void printFp(float16 value);

unsigned char getMantissa(float16 fpValue);
char getExponent(float16 fpValue);

void setMantissa(float16* fpValue, unsigned char newValue);
void setExponent(float16* fpValue,          char newValue);

float16 value2fp(unsigned short value);
short fp2value(float16 fpValue);

void shs(float16* fpValue, short offset);
void shg(float16* fpValue, short offset);

void normalizeAndSet(float16* fpValue, unsigned char mantissa, short exponent);
float16 add(float16 val1, float16 val2);
float16 mul(float16 val1, float16 val2);
float16 div(float16 val1, float16 val2);

int comp(float16 big, float16 small);


#endif

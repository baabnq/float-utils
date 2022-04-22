
/*

	16 bit floating point test for baabnq std lib
	format:
		msb [(1 bit sign) (8 bit exponent) (7 bit mantissa)] lsb

*/

#include "Float.h"
#include <stdio.h>

unsigned char getMantissa(float16 fpValue)
{
	return (char)((fpValue & MANTISSA_MASK) | 1 << (MANTISSA_SIZE - 1));

}

char getExponent(float16 fpValue)
{
	return (char)((fpValue >> (MANTISSA_SIZE - 1)) & EXPONENT_MASK);

}

//the MANTISSA_SIZE is the virtual size of the mantissa
//MANTISSA_SIZE - 1 bits are actually being stored
void setMantissa(float16* fpValue, unsigned char newValue)
{
	//clear mantissa
	*fpValue &= ~MANTISSA_MASK;

	//set new value
	*fpValue |= newValue & MANTISSA_MASK;
}

void setExponent(float16* fpValue, char newValue)
{
	//clear exponent
	*fpValue &= ~(EXPONENT_MASK << (MANTISSA_SIZE - 1));

	//set new value
	*fpValue |= (newValue & EXPONENT_MASK) << (MANTISSA_SIZE - 1);
}


void invValue(char* value)
{
	*value = ~(*value) + 1;

}

void invExponent(float16* fpValue)
{
	setExponent(fpValue, (~getExponent(*fpValue)) + 1);

}

void reci(float16* value)
{


}


float16 value2fp(short value)
{
	float16 output = 0;
	char exponent = 0;

	while (value >> MANTISSA_SIZE)
	{
		value >>= 1;
		exponent += 1;
	}

	normalizeAndSet(&output, value, exponent);
	return output;
	
}

short fp2value(float16 fpValue)
{
	unsigned char mantissa = getMantissa(fpValue);
	         char exponent = getExponent(fpValue);
	short output = 0;

	if (exponent < 0)
	{
		invValue(&exponent);
		output = (mantissa >> exponent);
	}
	else
	{
		output = (mantissa << exponent);
	}

	printf("output: %d\n", output);
	return output;
}



void normalizeAndSet(float16* fpValue, unsigned char mantissa, char exponent)
{
	while (!(mantissa >> (MANTISSA_SIZE - 1)))
	{
		mantissa <<= 1;
		exponent -= 1;
	}

	setMantissa(fpValue, mantissa);
	setExponent(fpValue, exponent);

}


float16 add(float16 val1, float16 val2)
{

	float16 valLow = 0;
	float16 valBig = 0;
	if (getExponent(val1) < getExponent(val2))
	{
		valBig = val2;
		valLow = val1;
	}
	else
	{
		valBig = val1;
		valLow = val2;
	}


	char outputExponent = getExponent(valBig);
	char deltaExponent  = outputExponent - getExponent(valLow);

	//cast to short for the addition to catch carry
	short valBigMantissaMod = (short)getMantissa(valBig);
	short valLowMantissaMod = (short)getMantissa(valLow) >> deltaExponent;

	//apply the sign
	if (valLow & SIGN_MASK) invValue(&valLowMantissaMod);
	if (valBig & SIGN_MASK) invValue(&valBigMantissaMod);

	//do the addition
	short outputMantissa = valBigMantissaMod + valLowMantissaMod;
	short outputSign = 0;

	//check for sign and get abs
	if (outputMantissa & SIGN_MASK)
	{
		outputSign = 1;
		invValue(&outputMantissa);

	}

	//check for carry overflow
	if (outputMantissa & 1 << MANTISSA_SIZE)
	{
		outputMantissa >>= 1;
		outputExponent += 1;
	}

	float16 output = 0;
	normalizeAndSet(&output, (char)outputMantissa, outputExponent);
	if (outputSign) output |= SIGN_MASK;
	return output;

}

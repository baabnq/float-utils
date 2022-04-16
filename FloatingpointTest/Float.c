
/*

	16 bit floating point test for baabnq std lib
	format:
		msb [(1 bit sign) (8 bit exponent) (7 bit mantissa)] lsb

*/

#include "Float.h"
#include <stdio.h>

char getMantissa(float16 fpValue)
{
	return (char)(fpValue & MANTISSA_MASK);

}

char getExponent(float16 fpValue)
{
	return (char)((fpValue >> MANTISSA_SIZE) & EXPONENT_MASK);

}

void setMantissa(float16* fpValue, float16 newValue)
{
	//clear mantissa
	*fpValue &= ~MANTISSA_MASK;

	//set new value
	*fpValue |= newValue & MANTISSA_MASK;
}

void setExponent(float16* fpValue, float16 newValue)
{
	//clear exponent
	*fpValue &= ~(EXPONENT_MASK << MANTISSA_SIZE);

	//set new value
	*fpValue |= (newValue & EXPONENT_MASK) << MANTISSA_SIZE;
}


void inv(short* value)
{
	*value = ~(*value) + 1;

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

	setMantissa(&output, value);
	setExponent(&output, exponent);

	normalize(&output);
	return output;
	
}

short fp2value(float16 fpValue)
{
	short exponent = (short)getExponent(fpValue);
	short mantissa = (short)getMantissa(fpValue);
	short output = 0;

	if (exponent < 0)
	{
		inv(&exponent);
		output = (mantissa >> exponent);
	}
	else
	{
		output = (mantissa << exponent);
	}

	return (short)output;
}



void normalize(float16* fpValue)
{
	char mantissa = getMantissa(*fpValue);
	char exponent = getExponent(*fpValue);

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
	if (valLow & SIGN_MASK) inv(&valLowMantissaMod);
	if (valBig & SIGN_MASK) inv(&valBigMantissaMod);

	//do the addition
	short outputMantissa = valBigMantissaMod + valLowMantissaMod;
	short outputSign = 0;

	//check for sign and get abs
	if (outputMantissa & SIGN_MASK)
	{
		outputSign = 1;
		inv(&outputMantissa);

	}

	//check for carry overflow
	if (outputMantissa & 1 << MANTISSA_SIZE)
	{
		outputMantissa >>= 1;
		outputExponent += 1;
	}

	float16 output = 0;
	setExponent(&output, outputExponent);
	setMantissa(&output, (char)outputMantissa);
	if (outputSign) output |= SIGN_MASK;

	normalize(&output);
	return output;

}




/*

	16 bit floating point test for baabnq std lib
	format:
		msb [(1 bit sign) (8 bit exponent) (7 bit mantissa)] lsb

*/

#include <stdio.h>
#include <math.h>


typedef unsigned short float16;

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


void printFp(float16 value);

unsigned char getMantissa(float16 fpValue);
char getExponent(float16 fpValue);

void setMantissa(float16* fpValue, unsigned char newValue);
void setExponent(float16* fpValue,          char newValue);

float16 value2fp(unsigned short value);
short fp2value(float16 fpValue);

void fshs(float16* fpValue, short offset);
void fshg(float16* fpValue, short offset);

void normalizeAndSet(float16* fpValue, unsigned char mantissa, short exponent);
float16 fadd(float16 val1, float16 val2);
float16 fmul(float16 val1, float16 val2);
float16 fdiv(float16 val1, float16 val2);

int fcomp(float16 big, float16 small);









void printFp(float16 value)
{
	printf("Mantissa: %d\n", getMantissa(value));
	printf("Exponent: %d\n", getExponent(value));
	printf("Sign    : %d\n", value & SIGN_MASK ? 1 : 0);
	printf("\n");

	unsigned char man = getMantissa(value);
	         char exp = getExponent(value);

	float absVal = man * pow(2.0f, exp);
	float val = value & SIGN_MASK ? -absVal : absVal;

	printf("Value: %f\n\n", val);
}



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

void invChar(char* value)
{
	*value = ~(*value) + 1;
}
void invShort(short* value)
{
	*value = ~(*value) + 1;
}

//fpValue >>= offset;
void fshs(float16* fpValue, short offset)
{
	setExponent(fpValue, getExponent(*fpValue) - offset);
}

//fpValue <<= offset;
void fshg(float16* fpValue, short offset)
{
	setExponent(fpValue, getExponent(*fpValue) + offset);
}



/*
void invExponent(float16* fpValue)
{
	setExponent(fpValue, (~getExponent(*fpValue)) + 1);

}
*/


float16 value2fp(unsigned short value)
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
	  signed char exponent = getExponent(fpValue);


	short output = exponent >= 0 ?
		(mantissa << exponent) :
		(mantissa >> (-exponent));

	return (fpValue & SIGN_MASK) ? -output : output;
}



void normalizeAndSet(float16* fpValue, unsigned char mantissa, short exponent)
{
	if ((exponent & upper16) && !((exponent & upper16) == upper16))
		*fpValue = posNaN;

	else if (mantissa)
	{
		while (!(mantissa >> (MANTISSA_SIZE - 1)))
		{
			mantissa <<= 1;
			exponent -= 1;
		}

		setMantissa(fpValue, mantissa);
		setExponent(fpValue, exponent);
	}
	else
	{
		setMantissa(fpValue, 0);
		setExponent(fpValue, MIN_SIGNED_CHAR);

	}
}


float16 fadd(float16 val1, float16 val2)
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
	if (valLow & SIGN_MASK) invShort(&valLowMantissaMod);
	if (valBig & SIGN_MASK) invShort(&valBigMantissaMod);

	//do the addition
	short outputMantissa = valBigMantissaMod + valLowMantissaMod;
	short outputSign = 0;

	//check for sign and get abs
	if (outputMantissa & SIGN_MASK)
	{
		outputSign = 1;
		invShort(&outputMantissa);

	}

	//check for carry overflow
	if (outputMantissa & (1 << MANTISSA_SIZE))
	{
		outputMantissa >>= 1;
		outputExponent += 1;
	}


	float16 output = 0;
	normalizeAndSet(&output, (char)outputMantissa, outputExponent);
	if (outputSign) output |= SIGN_MASK;

	return output;

}


float16 fmul(float16 val1, float16 val2)
{
	short val1Mantissa = (short)getMantissa(val1);
	short val2Mantissa = (short)getMantissa(val2);

	         short outputExponent = getExponent(val1) + getExponent(val2);             
	unsigned short outputMantissa = val1Mantissa * val2Mantissa;
    
    if (val1 == value2fp(0)) return val1;
    if (val2 == value2fp(0)) return val2;
    
    //cap exponent
    if (outputExponent < -128) return value2fp(0);

	short sign = (val1 & SIGN_MASK) ^ (val2 & SIGN_MASK);

	//move the outputMantissa back till it fit back into the char, in the process discarding the precision the we can't keep
	while ((unsigned)outputMantissa > (1 << MANTISSA_SIZE) - 1)
	{
		outputMantissa >>= 1;
		outputExponent += 1;
	}

	float16 output = 0;

	normalizeAndSet(&output, outputMantissa, outputExponent);
	return output | (sign ? SIGN_MASK : 0);
}


//does val1 / val2
float16 fdiv(float16 val1, float16 val2)
{
	unsigned short val1Mantissa = ((short)getMantissa(val1)) << 8;
	unsigned short val2Mantissa = (short)getMantissa(val2);

	short val1Exponent = getExponent(val1) - 8;
	short val2Exponent = getExponent(val2);

	short outputExponent = val1Exponent - val2Exponent;
	unsigned short outputMantissa = val1Mantissa / val2Mantissa;

	float16 output = 0;
	normalizeAndSet(&output, outputMantissa, outputExponent);
	return output;


}


//return 1 if val1 > val2
int fcomp(float16 big, float16 small)
{
	short val1Exponent = getExponent(big);
	short val2Exponent = getExponent(small);

	short val1Mantissa = getMantissa(big);
	short val2Mantissa = getMantissa(small);

	if (val1Exponent != val2Exponent)
		return val1Exponent > val2Exponent;
	else
		return val1Mantissa > val2Mantissa;
}

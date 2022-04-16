
#ifndef _FLOAT_H
#define _FLOAT_H

typedef short float16;


#define MANTISSA_SIZE 7
#define EXPONENT_SIZE 8
#define SIGN_SIZE     15
#define MANTISSA_MASK (float16)0b0000000001111111
#define EXPONENT_MASK (float16)0b0000000011111111
#define SIGN_MASK     (float16)0b1000000000000000







char getMantissa(float16 fpValue);
char getExponent(float16 fpValue);

void setMantissa(float16* fpValue, float16 newValue);
void setExponent(float16* fpValue, float16 newValue);

float16 value2fp(short value);
short fp2value(float16 fpValue);

void normalize(float16* fpValue);
float16 add(float16 val1, float16 val2);


#endif

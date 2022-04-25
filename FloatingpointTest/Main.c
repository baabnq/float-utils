
#include <stdio.h>
#include "Float.h"







int main()
{

	float16 a = value2fp(1000);
	float16 b = value2fp(1000);

	float16 c = mul(a, b);

	printFp(c);


	return 0;
}


#include <stdio.h>
#include "Float.h"







int main()
{

	float16 a = value2fp(1);
	float16 b = value2fp(1);

	setExponent(&a, 127);
	setExponent(&b, 127);


	float16 c = mul(a, b);



	printFp(c);





	return 0;
}

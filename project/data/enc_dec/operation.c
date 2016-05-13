#include<stdio.h>
#include<math.h>
#include<string.h>

int add2(int a, int b)
{
	return a+b;
}

int add3(int a, int b, int c)
{
	return a+b+c;
}

int substract(int a, int b)
{
	return a-b;
}

int mutiply(int a, int b)
{
	return a*b;
}

int divide(int a, int b)
{
	if(b!=0)
		return a/b;
	else
		return 0;
}

int a_mod_b(int a, int b)
{
	return a%b;
}

int char2_int(char c)
{
	return c-'0';
}

int hex2_integer(char *hexString)
{
	char *hex;
	int i, length=0, decimalNumber=0;
	const int base=16;

	if(hexString[1]=='\n')
		hexString[1]='\0';

	for (hex = hexString; *hex!='\0'; hex++)
		length++;

	for (i=0; *hexString!='\0' && i<length; i++, hexString++)
	{
		if (*hexString >= 48 && *hexString <= 57)
			decimalNumber += (((int)(*hexString)) - 48) * pow(base, length - i - 1);
		else
			decimalNumber += (((int)(*hexString)) - 87) * pow(base, length - i - 1);

	}
	return decimalNumber;
}

int compare(int a, int b)
{
	if(a<b)
		return 1;
	else
		return 0;
}

int greater2(int a, int b)
{
	if(a>b)
		return a;
	else
		return b;
}

int greater3(int a, int b, int c)
{
	if(a>b)
	{
		if(a>c)
			return a;
		else
			return c;
	}
	else
	{
		if(b>c)
			return b;
		else
			return c;
	}
}

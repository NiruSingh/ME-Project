#include<stdio.h>
#include"operation.h"

int main()
{
	int a, b, c;
	printf("Enter 3 numbers\n");
	scanf("%d%d%d",&a,&b,&c);
	printf("\nGreatest is %d",greater3(a,b,c));
	return 0;
}

#include<stdio.h>
int main()
{
	char offset_ar[4],*addr_loc="abcdef";
	memcpy( offset_ar,&addr_loc[3], 3 );
		offset_ar[4] = '\0';
	printf("%s\n",offset_ar);
	return 0;
}

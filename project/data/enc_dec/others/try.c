#include<stdio.h>
#include<stdlib.h>
int main()
{
char hexbyte[]="A3";
long da;
char ch='0';
int a;
sscanf( hexbyte, "%X", &da) ;
da+=15;
printf( "0x%2.2X\n", da ) ;
a=ch-'0';
printf("%d",a);
}

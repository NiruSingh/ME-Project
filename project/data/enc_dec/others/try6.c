#include<stdio.h>
#include<stdlib.h>
#include<math.h>
 
long convertToDecimal(char hex[]);
 
int main()
{
    char hex[5]="fc616";
    printf("%ld",convertToDecimal(hex));
    return 0;
}
 
long convertToDecimal(char hex[])
{
    char *hexString;
    int length = 0;
    const int base = 16; // Base of Hexadecimal Number
    long decimalNumber = 0;
    int i;
    // Find length of Hexadecimal Number
    for (hexString = hex; *hexString != '\0'; hexString++)
    {
        length++;
    }
    // Find Hexadecimal Number
    hexString = hex;
    for (i = 0; *hexString != '\0' && i < length; i++, hexString++)
    {
        // Compare *hexString with ASCII values
        if (*hexString >= 48 && *hexString <= 57)   // is *hexString Between 0-9
        {
            decimalNumber += (((int)(*hexString)) - 48) * pow(base, length - i - 1);
        }
        else if ((*hexString >= 65 && *hexString <= 70))   // is *hexString Between A-F
        {
            decimalNumber += (((int)(*hexString)) - 55) * pow(base, length - i - 1);
        }
        else if (*hexString >= 97 && *hexString <= 102)   // is *hexString Between a-f
        {
            decimalNumber += (((int)(*hexString)) - 87) * pow(base, length - i - 1);
        }
        else
        {
            printf(" Invalid Hexadecimal Number \n");
            fflush(stdin);
            return 0;
            exit(0);
        }
    }
    return decimalNumber;
}
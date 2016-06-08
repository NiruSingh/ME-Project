#include<stdio.h>
#include<stdlib.h>

long hex2_dec(char hex[])
{
	char *hexString;
	int length = 0;
	int base = 16; // Base of Hexadecimal Number
	long decimalNumber = 0;
	int i;

	for (hexString = hex; *hexString != '\0'; hexString++)
		length++;

    // Find Hexadecimal Number
	hexString = hex;
	for (i = 0; *hexString != '\0' && i < length; i++, hexString++)
	{
		if (*hexString >= 48 && *hexString <= 57)   // is *hexString Between 0-9
			decimalNumber += (((int)(*hexString)) - 48) * pow(base, length - i - 1);
        else if ((*hexString >= 65 && *hexString <= 70))   // is *hexString Between A-F
		decimalNumber += (((int)(*hexString)) - 55) * pow(base, length - i - 1);
        else if (*hexString >= 97 && *hexString <= 102)   // is *hexString Between a-f
		decimalNumber += (((int)(*hexString)) - 87) * pow(base, length - i - 1);
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

int main(int argc, char *argv[])
{
	int len, i;
	long offset;
	char data[10], towrite[10];
	FILE *ptr_bin, *ptr_data, *ptr_len_off;
	ptr_bin=fopen(argv[1],"rb+");
	ptr_data=fopen("data.txt","r");
	ptr_len_off=fopen("len_off.txt","r");

	if(!ptr_bin || !ptr_len_off || !ptr_data)
	{
		printf("unable to open files\n");
		exit(1);
	}

	while(1)
	{
		// read len and offset of encrypted instructions
		fscanf(ptr_len_off,"%d",&len);
		fseek(ptr_len_off,1,SEEK_CUR);
		fscanf(ptr_len_off,"%ld",&offset);
		//printf("len=%d off=%d\n",len,offset);

		// read original data and write to binary
		i=0;
		fseek(ptr_bin,offset,SEEK_SET);
		while(len--)
		{
			fseek(ptr_data,1,SEEK_CUR);
			fread(data,2,1,ptr_data);
			data[2]='\0';
			if(data[1]=='\n' || data[1]==' ')
			{	
				fseek(ptr_data,-1,SEEK_CUR);
				data[1]='\0';
			}
			towrite[i]=hex2_dec(data);

			// write original data to binary
			fwrite(&towrite[i],sizeof(char),1,ptr_bin);

			//printf("data1=%s data2=%d\n",data,hex2_dec(data));
			i++;
		}
		fseek(ptr_data,1,SEEK_CUR);
		rewind(ptr_bin);

/*		fseek(ptr_bin,offset,SEEK_SET);
		fwrite(towrite,sizeof(char),1,ptr_bin);
		rewind(ptr_bin);
*/
		fseek(ptr_len_off,1,SEEK_CUR);
		if(fread(data,1,1,ptr_len_off)!=1)
			break;
		else
			fseek(ptr_len_off,-1,SEEK_CUR);
	}

	fclose(ptr_bin);
	fclose(ptr_data);
	fclose(ptr_len_off);

	return 0;
}

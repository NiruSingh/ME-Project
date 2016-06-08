#include<stdio.h>

int hex2_dec(char *hexString)
{
	char *hex;
	int i, length=0, decimalNumber=0;
	const int base=16;

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

int main(int argc, char *argv[])
{
	int len, offset, i;
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
		fscanf(ptr_len_off,"%d",&offset);
		//printf("len=%d off=%d\n",len,offset);

		// read original data and write to binary
		i=0;
		fseek(ptr_bin,offset,SEEK_SET);
		while(len--)
		{
			fseek(ptr_data,1,SEEK_CUR);
			fread(data,2,1,ptr_data);
			data[2]='\0';
			if(data[1]=='\n')
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

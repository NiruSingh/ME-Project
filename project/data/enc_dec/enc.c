#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int sub_hexs(int *a1, int *a2)
{
	int i, diff[3];
	for(i=2;i>=0;i--)
	{
		if(a2[i]<a1[i])
		{
			a2[i-1]-=1;
			diff[i]=a2[i]+16-a1[i];
		}
		else
			diff[i]=a2[i]-a1[i];
	}
	return diff[2];
}

int calculate_len(char *off1, char *off2)
{
	int i, diff, len, off_int1[3], off_int2[3];

	for(i=0;i<3;i++)
	{	
		switch(off1[i])
		{
			case 'a': off_int1[i]=10;
				break;
			case 'b': off_int1[i]=11;
				break;
			case 'c': off_int1[i]=12;
				break;
			case 'd': off_int1[i]=13;
				break;
			case 'e': off_int1[i]=14;
				break;
			case 'f': off_int1[i]=15;
				break;
			default: off_int1[i]=off1[i]-'0';
				break;
		}

		switch(off2[i])
		{
			case 'a': off_int2[i]=10;
				break;
			case 'b': off_int2[i]=11;
				break;
			case 'c': off_int2[i]=12;
				break;
			case 'd': off_int2[i]=13;
				break;
			case 'e': off_int2[i]=14;
				break;
			case 'f': off_int2[i]=15;
				break;
			default: off_int2[i]=off2[i]-'0';
				break;
		}
	}

	return sub_hexs(off_int1, off_int2);
}

long find_len(FILE *ptr, char *addr)
{
	int i=0;
	unsigned long off_int1, off_int2, len;
	char tmp[100],off1[4],off2[4],ch;
	FILE *ptr2;
	while(1)
	{
		ch=fgetc(ptr);
		if(ch == '\n')
			continue;

		fread(off1,3,1,ptr);
		off1[3]='\0';

		if(strcmp(addr,off1)==0)
		{
			ptr2=ptr;
			do
			{
				ch=fgetc(ptr2);
			}while(ch != '\n');
			fseek(ptr2,1,SEEK_CUR);
			fread(off2,3,1,ptr2);
			off2[3]='\0';

			return calculate_len(off1,off2);
		}

		do
		{
			ch=fgetc(ptr);
		}while(ch != '\n');

		if(fread(tmp,1,1,ptr)!=1)
			break;
		else
			fseek(ptr,-1,SEEK_CUR);
	}

	return 0;
}

int hex2_dec(char *hexString)
{
	int i, length=3;
	const int base=16;
	int decimalNumber=0;

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
	char ch, addr_loc[4], line[100], offset_ar[4];
	unsigned char data[10];
	int len, i, offset, ctr;
	char leave=201;		// byte to replace with
	FILE *ptr_loc, *ptr_dump, *ptr_bin, *ptr_bin_r, *ptr_len_off, *ptr_data;

	ptr_loc=fopen("locations.txt","r");
	ptr_dump=fopen("objdump.txt","r");
	ptr_bin=fopen(argv[1],"rb+");
	ptr_bin_r=fopen(argv[1],"rb");
	ptr_len_off=fopen("len_off.txt","w+");
	ptr_data=fopen("data.txt","w+");

	if(!ptr_loc || !ptr_dump || !ptr_bin || !ptr_bin_r || !ptr_len_off || !ptr_data)
	{
		printf("unable to open files\n");
		exit(1);
	}
	
	fseek(ptr_loc,1,SEEK_SET);
	while(1)
	{
		// read address from file locations.txt
		fseek(ptr_loc,1,SEEK_CUR);
		fread(addr_loc,3,1,ptr_loc);
		addr_loc[3]='\0';
		printf("%s\n",addr_loc);

		// find length of instruction to be replaced
		len=find_len(ptr_dump,addr_loc);
		rewind(ptr_dump);
/*
		// convert offset to decimal
		memcpy(offset_ar,&addr_loc[4], 3);
		offset_ar[4] = '\0';
		offset=hex2_dec(offset_ar);
*/
		offset=hex2_dec(addr_loc);
		printf("offset=%d, len=%d\n",offset,len);

		// read from binary and update files.
		fseek(ptr_bin_r,offset,SEEK_SET);
		fread(data,len,1,ptr_bin_r);
		
		// write to data.txt
		for(i=0;i<len;i++)
			fprintf(ptr_data, " %x", data[i]);
		fprintf(ptr_data, "%c", '\n');

		// write to len_off.txt
		fprintf(ptr_len_off, "%d %d\n", len, offset);

		for(i = 0; i<3; i++)
		    printf("%x ", data[i]);
		printf("\n");

		rewind(ptr_bin_r);


		// replacement in binary file
		fseek(ptr_bin,offset,SEEK_SET);
		ctr=len;

		while(ctr--)
			fprintf(ptr_bin,"%c",leave);
			//fwrite(&leave, sizeof(unsigned char), 1, ptr_bin);
		rewind(ptr_bin);

//		exit(1);

		do
		{
			ch=fgetc(ptr_loc);
		}while(ch != '\n');
		if(fread(addr_loc,1,1,ptr_loc)!=1)
			break;
	}

	fclose(ptr_len_off);
	fclose(ptr_data);
	fclose(ptr_bin);
	fclose(ptr_bin_r);
	fclose(ptr_dump);
	fclose(ptr_loc);

	return 0;
}

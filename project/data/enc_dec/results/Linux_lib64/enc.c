#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int sub_hexs(int *a1, int *a2, int len)
{
	int i, diff[10];
	for(i=len-1;i>=0;i--)
	{
		if(a2[i]<a1[i])
		{
			a2[i-1]-=1;
			diff[i]=a2[i]+16-a1[i];
		}
		else
			diff[i]=a2[i]-a1[i];
	}
	return diff[len-1];
}

int calculate_len(char *off1, char *off2, int len)
{
	int i, diff, off_int1[100], off_int2[100];

	for(i=0;i<len;i++)
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

	diff=sub_hexs(off_int1, off_int2, len);
//	printf("diff=%d\n",diff);
	return diff;
}

long find_len(FILE *ptr, char *addr)
{
	int i=0, tmp_len, flag;
	unsigned long off_int1, off_int2, len;
	char tmp[100],off1[100],off2[100],ch;
	FILE *ptr2;
	while(1)
	{
		ch=fgetc(ptr);
		if(ch == '\n')
			continue;

		tmp_len=0;
		flag=0;
		do
		{
			ch=fgetc(ptr);
		}while(ch == ' ');
		fseek(ptr,-1,SEEK_CUR);

		while(1)
		{
			if(fread(&ch,1,1,ptr)!=1)
			{
				flag=1;
				break;
			}
			if(ch!=':')
				tmp_len++;
			else
				break;
		}
		
		if(flag==1)
			break;
		tmp_len++;
		fseek(ptr,-tmp_len,SEEK_CUR);
		tmp_len--;
		fread(off1,tmp_len,1,ptr);
		off1[tmp_len]='\0';

		//printf("tmp_len: %d , off1 : %s, addr : %s, flag : %d, ch : %c\n",tmp_len,off1,addr,flag,ch);

		if(strcmp(addr,off1)==0)
		{
			do
			{
				ch=fgetc(ptr);
			}while(ch != '\n');
			do
			{
				ch=fgetc(ptr);
			}while(ch == ' ');
			fseek(ptr,-1,SEEK_CUR);
			fread(off2,tmp_len,1,ptr);
			off2[tmp_len]='\0';
//			printf("off1=%s, off2=%s\n",off1,off2);
			return calculate_len(off1,off2,tmp_len);
		}		
/*
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
*/
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
	char ch, addr_loc[10], line[100], offset_ar[4];
	unsigned char data[100];
	int len, i, ctr, len_loc;
	long offset;

	char leave1[1], leave2[2], leave3[3];

	char leave[1];
	leave[0]=201;

	leave1[0]=83;//83
	leave2[0]=139;leave2[1]=06;//8b 06
	leave3[0]=131;leave3[1]=238;leave3[2]=04;//83 ee 04

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
		len_loc=0;
		do
		{
			ch=fgetc(ptr_loc);
			len_loc++;
		}while(ch != ':');
		len_loc-=2;
		//printf("len_loc : %d\n",len_loc);

		fseek(ptr_loc,-(len_loc+2),SEEK_CUR);
		fread(addr_loc,len_loc,1,ptr_loc);
		addr_loc[len_loc]='\0';
		//printf("addr_loc : %s\n",addr_loc);

		// find length of instruction to be replaced
		len=find_len(ptr_dump,addr_loc);
		rewind(ptr_dump);

		if(len!=0)
		{

		offset=hex2_dec(addr_loc);
		//printf("offset=%ld, len=%d\n",offset,len);

		// read from binary and update files.
		fseek(ptr_bin_r,offset,SEEK_SET);
		fread(data,len,1,ptr_bin_r);
		
		// write to data.txt
		for(i=0;i<len;i++)
			fprintf(ptr_data, " %x", data[i]);
		fprintf(ptr_data, "%c", '\n');

		// write to len_off.txt
		fprintf(ptr_len_off, "%d %ld\n", len, offset);

//		for(i = 0; i<3; i++)
//		    printf("%x ", data[i]);
//		printf("\n");

		rewind(ptr_bin_r);

//		exit(0);

		// replacement in binary file
		fseek(ptr_bin,offset,SEEK_SET);
		ctr=len;

		if(len==1)
			fwrite(leave1, sizeof(leave1), 1, ptr_bin);
		else if(len==2)
			fwrite(leave2, sizeof(leave2), 1, ptr_bin);
		else if(len==3)
			fwrite(leave3, sizeof(leave3), 1, ptr_bin);
		else
		{
			while(len--)
				fwrite(leave1, sizeof(leave1), 1, ptr_bin);
		}
		rewind(ptr_bin);

//		exit(0);

		}
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

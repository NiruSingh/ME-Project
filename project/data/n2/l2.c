#include<stdio.h>
int main()
{
  long diff;
  static char buf1[16], *buf2;

  buf2 = buf1;
  diff =  (long)buf1 -  (long)buf2;
  printf("buf1 = %p & buf2 = %p & diff %d\n",buf1,&buf2,diff);
  printf("BEFORE: buf1 = %p\n",buf1);
  /* An address is 4 long, so we overwrite 4 chars */
  memset(buf2,'A',diff+4);  
  printf("AFTER:  buf1 = %p\n",buf1);

  return 0;
}

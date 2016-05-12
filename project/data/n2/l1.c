#include<stdio.h>
#include<string.h>
#include <fcntl.h>
int bla(char *text)
{
  puts(text);
}

int main(int argc, char **argv)
{
  u_long diff;
  int (*funct)(char *)=bla;
  char buf[4];

  if(argc < 3)
  {
	printf("Usage: %s <contents> <text>\n",argv[0]);
	exit(-1);
  }
  
//  funct = (int (*)(char *))bla; 

  printf("BEFORE: funct = 0x%x\n",funct);
  
  strcpy(buf,argv[1]);

  printf("AFTER:  funct = 0x%x\n",funct);

  diff = (u_long)&buf - (u_long)&funct;
  printf("buf = %p, funct = %p, diff = 0x%x bytes\n", &buf, &funct, diff);

  (int)(*funct)(argv[2]);   
  return 0;
}

#include<stdio.h>
#include<malloc.h>
#include<string.h>
struct heap
{
	char buffer[32];
	void (*funcptr)();
};

void fun()
{
	printf("Hi\n");
}

int main(int argc, char **argv)
{
	char ch[4];
	struct heap *data;
	memcpy(ch,argv[2],strlen(argv[2]));
	data=(struct heap *)malloc(sizeof(struct heap));
	data->funcptr=&fun;
	memcpy(data->buffer,argv[1],strlen(argv[1]));
	(*data->funcptr)();
	return 0;
}

/*shellcodetest.c*/ 
char code[] = "\x31\xc0\x31\xdb\xcd\x80";
int main(int argc, char **argv)
{
	int (*func)();
	func = (int (*)()) code;
	(int)(*func)();
}

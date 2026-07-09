
#include <stdlib.h>
#include <stdio.h>
int fac(int n);

int main(int argc, char *argv[])
{ 
	int n = 7;
	if (argc == 2)
		n = atoi(argv[1]);
	printf("%i! = %d\n", n, fac(n));
	return 0;
}

int fac(int n)
{
	if (n == 0)
		return 1;
	return n * fac(n-1); 
}

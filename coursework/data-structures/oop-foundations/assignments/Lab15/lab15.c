//modified by: Soto Erik
//date:		   12/9/18
//purpose:	   Decrypt Files from a C prog.

#include <stdio.h>
#include <string.h>
#include <malloc.h>

const char fname[] = "file15";

int main()
{
	unsigned char *str = (unsigned char *)malloc(1000);
	FILE *fpi = fopen(fname,"r");
	if (!fpi) {
		printf("ERROR opening **%s**\n", fname);
		return 0;
	}
	fread(str, 1, 1000, fpi);
	int slen = strlen((char *)str);
	printf("str len: %i\n", slen);
	fclose(fpi);
	//
	//Encryption method:
	//
	//Each character in string is rolled 2 bits to the left.
	//No bits are lost in the roll operation.
	//
	//Call the decrypt15 function in lab15.s
	//Write the decryption operations there.
	//No changes are needed in this file.
	//
	extern void decrypt15(unsigned char *str);
	decrypt15(str);
	printf("%s\n", (char *)str);
	//
	free(str);
	return 0;
}

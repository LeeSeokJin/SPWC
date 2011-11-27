#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char name[] = "jue";
	char token[] = "*";
	char address[] = "12132132123";
	char message[100] ="";
	char* k = message;
	memcpy(k,name, strlen(name));
	k = k+strlen(name);
	*k = '*';
	k++;
	//memcpy(k,&token, strlen(token));
	//k = k+strlen(token);
	memcpy(k,address, strlen(address));
	k = k+strlen(address);
	
	//message = name + token + address;
	printf("String Length : %d \n", strlen(message));
	printf("String : %s \n", message);
	return 0;
}

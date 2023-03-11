#include "HttpRequest.h"
#include <stdio.h>

int main()
{
	char test[] = "PUT path/to/resource?well=shit HTTP/1.1";
	HttpRequest httpRequest(test);

	RequestType reqType = httpRequest.getRequestType();

	return 0;
}
#include "HttpRequest.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>

#define DEBUG_LOGS
#include "debug.h"

#define VALIDATE_PTR(src) if (src == nullptr) return

const char* HttpRequest::recognizedRequests[] =
{
	"GET",
	"POST",
	"PUT",
	"DELETE",
	"OPTIONS",
	"TRACE"
};
const int HttpRequest::numberOfRecognizedRequests = 6;
const char* HttpRequest::commonProtocolSubstring = "HTTP/";


char* HttpRequest::parseRequestType(char* src)
{
	for (int i = 0; src[i]; i++)
	{
		if (isspace(src[i]))
			continue;

		for (int reqIndex = 0; reqIndex < numberOfRecognizedRequests; reqIndex++)
		{
			bool matchFlag = true;
			int j;
			for (j = 0; src[j] && !isspace(src[j]); j++)
			{
				if (!recognizedRequests[reqIndex][j] || src[j] != recognizedRequests[reqIndex][j])
				{
					matchFlag = false;
					return nullptr;
				}
			}
			if (matchFlag == true)
			{
				requestType = (RequestType)reqIndex;
				return &src[j+1];
			}
		}
	}

	return nullptr;
}

char* HttpRequest::copyPathToResource(char* src)
{
	int pathLength = 0;
	for (int i = 0; src[i] && !isspace(src[i]) && src[i] != '?'; i++)
		pathLength++;

	if (pathLength == 0)
		return nullptr;

	pathToResource = new char[pathLength+1];
	int i, j;
	for (i = 0, j = 0; src[i] && !isspace(src[i]) && src[i] != '?'; i++, j++)
		pathToResource[j] = src[i];
	pathToResource[j] = '\0';

	return &src[i+1];
}

char* HttpRequest::parseProtocolVersion(char* src)
{
	DEBUG_OUT("info", "Parsing protocolVersion");
	bool matchFlag = true;
	int versionStartIndex = -1;
	int i, j;
	for (i = 0, j = 0; src[i] && src[i] != '\r' && matchFlag; i++)
	{
		if (commonProtocolSubstring[j] && src[i] != commonProtocolSubstring[j])
		{
			matchFlag = false;
			break;
		}

		if (commonProtocolSubstring[j])
		{
			j++;
			continue;
		}

		if (versionStartIndex == -1)
			versionStartIndex = i;
	}

	if (matchFlag)
		protocolVersion = strtod(&src[versionStartIndex], nullptr);
	else
	{
		DEBUG_OUT("info", "Something with protocolVersion did not parse correctly");
	}

	return &src[i + 1];
}

char* HttpRequest::parseParametersFromResourcePath(char* src)
{
	return nullptr;
}

bool HttpRequest::isValid() const
{
	return valid;
}

RequestType HttpRequest::getRequestType() const
{
	return requestType;
}

char* const HttpRequest::getPathToResource() const
{
	return pathToResource;
}

double HttpRequest::getProtocolVersion() const
{
	return protocolVersion;
}

HttpRequest::HttpRequest(char* src)
{
	VALIDATE_PTR(src);

	src = parseRequestType(src);
	VALIDATE_PTR(src);

	src = copyPathToResource(src);
	VALIDATE_PTR(src);

	src = parseParametersFromResourcePath(src);
	VALIDATE_PTR(src);

	parseProtocolVersion(src);

	valid = true;
}

std::ostream& operator<<(std::ostream& ostream, const HttpRequest& httpRequest)
{
	return ostream << "{" << std::endl <<
	"\tisValid:" << httpRequest.isValid() << std::endl <<
	"\trequestType: " << httpRequest.recognizedRequests[httpRequest.getRequestType()] << std::endl <<
	"\tprotocolVersion: " << httpRequest.getProtocolVersion() << std::endl <<
	"\tpathToResource: " << httpRequest.getPathToResource() << std::endl <<
	"}";
}
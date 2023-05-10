#include "HttpRequest.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>

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
const char* HttpRequest::commonProtocolSubstring = " HTTP/";


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
					matchFlag = false;
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
	int i;
	for (i = 0; src[i] && !isspace(src[i]) && src[i] != '?'; i++)
		pathLength++;

	if (pathLength == 0)
		return nullptr;

	pathToResource = new std::string(src, pathLength);

	return &src[i];
}

char* HttpRequest::parseProtocolVersion(char* src)
{
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

	if (protocolVersion == 0)
		return nullptr;

	return &src[i];
}

char* HttpRequest::parseParametersFromResourcePath(char* src)
{
	if (*src == '?')
		src++;
	else
		return src++;

	int keyStartIndex = 0;
	int valueStartIndex = -1;

	int i;
	for (i = keyStartIndex; ; i++)
	{
		if (src[i] == '\0')
			return nullptr;

		if (src[i] == '=')
			valueStartIndex = i + 1;

		else if (src[i] == '&' || isspace(src[i]))
		{
			if (valueStartIndex != -1)
			{
				std::string key(src + keyStartIndex, valueStartIndex - keyStartIndex - 1);
				std::string value(src + valueStartIndex, (src[i] == '\0' ? i : (i - valueStartIndex)));

				httpParametersBuilder.setValue(key, HttpParameterValue(value));

				keyStartIndex = i + 1;
				valueStartIndex = -1;
			}
			else
			{
				return nullptr;
			}
		}

		if (isspace(src[i]))
			break;
	}

	return src + i;
}

bool HttpRequest::isValid() const
{
	return valid;
}

RequestType HttpRequest::getRequestType() const
{
	return requestType;
}

std::string HttpRequest::getPathToResource() const
{
	if (pathToResource == nullptr)
		return "";
	return *pathToResource;
}

double HttpRequest::getProtocolVersion() const
{
	return protocolVersion;
}

HttpMap<HttpParameterValue>& HttpRequest::getParametersMap()
{
	return httpParametersBuilder.getContainer();
}

char* HttpRequest::validateNewlinePresent(char *src)
{
	if (*src != '\r')
		return nullptr;

	src++;

	if (*src != '\n')
		return nullptr;

	src++;
	return src;
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

	src = parseProtocolVersion(src);
	VALIDATE_PTR(src);

	src= validateNewlinePresent(src);
	VALIDATE_PTR(src);

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

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
const char* HttpRequest::commonProtocolSubstring = "HTTP/";


char* HttpRequest::parseRequestType(char* src)
{
	for (int i = 0; src[i]; i++)
	{
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

	if (!matchFlag)
		return nullptr;
	
	protocolVersion = strtod(&src[versionStartIndex], nullptr);

	if (isnan<double>(protocolVersion))
		return nullptr;

	return &src[i];
}

char* HttpRequest::parseParametersFromResourcePath(char* src)
{
	if (*src == '?')
	{
		src++;
		if (isspace(*src))
			return src++;
	}
	else
		return src++;

	int keyStartIndex = 0;
	int valueStartIndex = -1;
	bool isSpace = false;

	int i;
	for (i = 0; ; i++)
	{
		if (src[i] == '\0')
			return nullptr;

		if (src[i] == '=')
			valueStartIndex = i + 1;

		else if (src[i] == '&' || (isSpace = isspace(src[i])))
		{
			if (valueStartIndex != -1)
			{
				int keyEndIndex = valueStartIndex - 1;
				int valueEndIndex = ((src[i] == '\0') ? i + valueStartIndex : i);

				char tempKeyChar = src[keyEndIndex];
				char tempValueChar = src[valueEndIndex];

				src[keyEndIndex] = '\0';
				src[valueEndIndex] = '\0';

				httpParametersBuilder.setValue(src + keyStartIndex, HttpValue(src + valueStartIndex));

				src[keyEndIndex] = tempKeyChar;
				src[valueEndIndex] = tempValueChar;
			}

			keyStartIndex = i + 1;
			valueStartIndex = -1;
		}

		if (isSpace)
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

HttpMap& HttpRequest::getParametersMap()
{
	return httpParametersBuilder.getContainer();
}

HttpMap& HttpRequest::getHeadersMap()
{
	return httpHeadersBuilder.getContainer();
}

char* HttpRequest::validatePreHeaderBorderPresent(char *src)
{
	if (*src != '\r')
		return nullptr;

	src++;

	if (*src != '\n')
		return nullptr;

	src++;
	return src;
}

char* HttpRequest::parseHeaders(char* src)
{
	if (src == nullptr || *src == '\0')
		return nullptr;

	do
	{
		if (*src == '\r' && *(src + 1) == '\n')
			return src+=2;

		int i;
		int startIndex = 0;

		for (i = startIndex; src[i] && src[i] != ':'; i++);
		if (src[i] != ':')
			return nullptr;
		char* colonCharAddress = &src[i];
		src[i] = '\0';
		i++;
		if (!isspace(src[i]))
		{
			*colonCharAddress = ':';
			return nullptr;
		}

		char prevSpace = src[i];
		char* spaceCharAddress = &src[i];

		src[i] = '\0';
		if (!strcmp(src, "Cookie:"));
		else
			src = parseHeaderValueNonCookie(src, &src[i + 1]);

		*spaceCharAddress = prevSpace;
		*colonCharAddress = ':';
	} while (src && *src);

	return src;
}

char* HttpRequest::parseHeaderValueNonCookie(char* key, char* value)
{
	if (*key == '\0' || *value == '\0')
		return nullptr;

	int i;

	for (i = 0; value[i] && value[i] != '\r'; i++);
	if (value[i] != '\r')
		return nullptr;

	i++;
	if (value[i] != '\n')
		return nullptr;

	value[i - 1] = '\0';
	value[i] = '\0';
	httpHeadersBuilder.setValue(key, HttpValue(value));
	value[i - 1] = '\r';
	value[i] = '\n';

	return &value[i+1];
}

char* HttpRequest::ignoreExtraSpaces(char* src)
{
	if (src == nullptr)
		return nullptr;

	while (isspace(*src))
		src++;

	return src;
}

HttpRequest::HttpRequest(char* src)
{
	VALIDATE_PTR(src);

	src = parseRequestType(src);
	src = ignoreExtraSpaces(src);
	VALIDATE_PTR(src);

	src = copyPathToResource(src);
	VALIDATE_PTR(src);

	src = parseParametersFromResourcePath(src);
	src = ignoreExtraSpaces(src);
	VALIDATE_PTR(src);

	src = parseProtocolVersion(src);
	VALIDATE_PTR(src);

	src = validatePreHeaderBorderPresent(src);
	VALIDATE_PTR(src);

	src = parseHeaders(src);

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

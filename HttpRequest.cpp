#include "HttpRequest.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

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
	for (i = 0, j = 0; src[i] && src[i] != '\n' && matchFlag; i++)
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

	if (isnan(protocolVersion))
		return nullptr;

	return &src[i];
}

char* HttpRequest::parseParametersFromResourcePath(char* src)
{
	if (*src == '?')
		src++;
	else
		return src + 1;

	return parseKeyValuePairsCommon(src, '&', true, httpQueryParametersMapInternal);
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

HttpImmutableMap& HttpRequest::getQueryParametersMap()
{
	return httpQueryParametersMapImmutable;
}

HttpImmutableMap& HttpRequest::getHeadersMap()
{
	return httpHeadersMapImmutable;
}

HttpImmutableMap& HttpRequest::getCookiesMap()
{
	return httpCookiesMapImmutable;
}

HttpImmutableMap& HttpRequest::getBodyParametersMap()
{
	return httpBodyParametersMapImmutable;
}

char* HttpRequest::validateNewlinePresent(char *src)
{
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
		src = ignoreExtraSpaces(src);

		if (*src == '\n')
			return src + 1;

		int i;
		int startIndex = 0;

		for (i = startIndex; src[i] && src[i] != ':'; i++);
		if (src[i] != ':')
			return nullptr;
		char* colonCharAddress = &src[i];
		src[i] = '\0';

		char* value = ignoreExtraSpaces(src + i + 1);
		if (value == nullptr)
			return nullptr;

		if (!strcmp(src, "Cookie"))
			src = parseHeaderValueCookie(value);
		else
			src = parseHeaderValueNonCookie(src, value);

		*colonCharAddress = ':';
	} while (src && *src);

	return src;
}

char* HttpRequest::parseHeaderValueNonCookie(char* key, char* value)
{
	if (key == nullptr || value == nullptr || *key == '\0' || *value == '\0')
		return nullptr;

	int i;

	for (i = 0; value[i] && value[i] != '\n'; i++);
	if (value[i] != '\n')
		return nullptr;

	bool hasCRbeforeNewline = (i > 0) && (value[i-1] == '\r');

	if (hasCRbeforeNewline)
		value[i - 1] = '\0';
	value[i] = '\0';
	httpHeadersMapInternal.setValue(key, HttpValue(value));
	if (hasCRbeforeNewline)
		value[i - 1] = '\r';
	value[i] = '\n';

	return &value[i+1];
}

char* HttpRequest::parseHeaderValueCookie(char* value)
{
	if (value == nullptr || *value == '\0')
		return nullptr;

	return parseKeyValuePairsCommon(value, ';', false, httpCookiesMapInternal);
}

char* HttpRequest::parseBody(char *src)
{
	if (src == nullptr)
		return nullptr;

	if (*src == '\0')
		return src+1;

	return parseKeyValuePairsCommon(src, '&', false, httpBodyParametersMapInternal);

}

char* HttpRequest::ignoreExtraSpaces(char* src)
{
	if (src == nullptr)
		return nullptr;

	while (*src && *src != '\n' && isspace(*src))
		src++;

	return src;
}

char* HttpRequest::parseKeyValuePairsCommon(char* src, char seperator, bool useSpaceIfTrueElseNewline, HttpMap& httpMap)
{
	int keyStartIndex = -1;
	int valueStartIndex = -1;
	bool isLast = false;

	int i;
	for (i = 0; ; i++)
	{
		if (!useSpaceIfTrueElseNewline && valueStartIndex == -1 && src[i] != '\n' && isspace(src[i]))
			continue;
		
		if (keyStartIndex == -1)
			keyStartIndex = i;

		if (src[i] == '\0')
			return nullptr;

		if (src[i] == '=')
		{
			valueStartIndex = i + 1;
			continue;
		}

		isLast = useSpaceIfTrueElseNewline ? isspace(src[i]) : (src[i] == '\n');
		if (src[i] == seperator || isLast)
		{
			if (valueStartIndex != -1)
			{
				int keyEndIndex = valueStartIndex - 1;
				int valueEndIndex = i;

				char tempKeyChar = src[keyEndIndex];
				char tempValueChar = src[valueEndIndex];

				src[keyEndIndex] = '\0';
				src[valueEndIndex] = '\0';

				bool hasCRbeforeNewline = !useSpaceIfTrueElseNewline && isLast && valueEndIndex >= 1 && src[valueEndIndex - 1] == '\r';
				if (hasCRbeforeNewline)
					src[valueEndIndex - 1] = '\0';

				httpMap.setValue(src + keyStartIndex, HttpValue(src + valueStartIndex));

				if (hasCRbeforeNewline)
					src[valueEndIndex - 1] = '\r';

				src[keyEndIndex] = tempKeyChar;
				src[valueEndIndex] = tempValueChar;
			}

			keyStartIndex = -1;
			valueStartIndex = -1;
		}

		if (isLast)
			break;
	}

	return src + i + 1;
}

HttpRequest::HttpRequest(char* src)
	:	httpCookiesMapImmutable(httpCookiesMapInternal),
		httpHeadersMapImmutable(httpHeadersMapInternal),
		httpQueryParametersMapImmutable(httpQueryParametersMapInternal),
		httpBodyParametersMapImmutable(httpBodyParametersMapInternal)
{
	valid = false;
	protocolVersion = 0.0;
	
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

	src = ignoreExtraSpaces(src);
	src = validateNewlinePresent(src);
	VALIDATE_PTR(src);

	src = ignoreExtraSpaces(src);
	src = parseHeaders(src);
	VALIDATE_PTR(src);

	src = ignoreExtraSpaces(src);
	src = parseBody(src);
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

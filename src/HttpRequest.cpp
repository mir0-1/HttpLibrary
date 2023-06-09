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


char* HttpRequest::parseHttpRequestType(char* src)
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
				requestType = (HttpRequestType)reqIndex;
				return &src[j+1];
			}
		}
	}

	return nullptr;
}

char* HttpRequest::copyPathToResource(char* src)
{
	int pathLength = 0;
	int extensionStartIndex = -1;
	int i;

	static const std::string forbiddenChars = "\\%*:;|\"\'!@()<>";

	bool slashFlag = false;

	for (i = 0; src[i] && !isspace(src[i]) && src[i] != '?'; i++, pathLength++)
	{
		if (src[i] == '/')
		{
			if (extensionStartIndex != -1 || slashFlag == true)
				return nullptr;
			slashFlag = true;
			continue;
		}
		slashFlag = false;
		if (forbiddenChars.find_first_of(src[i]) != std::string::npos)
			return nullptr;
		if (src[i] == '.')
			extensionStartIndex = i + 1;
	}

	if (pathLength == 0)
		return nullptr;

	pathToResource.append(src, pathLength);
	if (extensionStartIndex != -1 && extensionStartIndex < i)
		resourceExtension.append(src + extensionStartIndex, pathLength - extensionStartIndex);

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

	return queryParametersMapInternal.parseKeyValuePairs(src, '&', ' ');
}

bool HttpRequest::isValid() const
{
	return valid;
}

HttpRequestType HttpRequest::getRequestType() const
{
	return requestType;
}

const std::string& HttpRequest::getPathToResource() const
{
	return pathToResource;
}

const std::string& HttpRequest::getResourceExtension() const
{
	return resourceExtension;
}

double HttpRequest::getProtocolVersion() const
{
	return protocolVersion;
}

const HttpImmutableMap& HttpRequest::getQueryParametersMap() const
{
	return queryParametersMapImmutable;
}

const HttpImmutableMap& HttpRequest::getHeadersMap() const
{
	return headersMapImmutable;
}

const HttpImmutableMap& HttpRequest::getCookiesMap() const
{
	return cookiesMapImmutable;
}

const HttpImmutableMap& HttpRequest::getBodyParametersMap() const
{
	return bodyParametersMapImmutable;
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
	headersMapInternal.setValue(key, ValueWrapper(value));
	if (hasCRbeforeNewline)
		value[i - 1] = '\r';
	value[i] = '\n';

	return &value[i+1];
}

char* HttpRequest::parseHeaderValueCookie(char* value)
{
	if (value == nullptr || *value == '\0')
		return nullptr;

	return cookiesMapInternal.parseKeyValuePairs(value, ';', '\n');
}

char* HttpRequest::parseBody(char *src)
{
	if (src == nullptr)
		return nullptr;

	if (*src == '\0')
		return src+1;

	return bodyParametersMapInternal.parseKeyValuePairs(src, '&', '\0');

}

char* HttpRequest::ignoreExtraSpaces(char* src)
{
	if (src == nullptr)
		return nullptr;

	while (*src && *src != '\n' && isspace(*src))
		src++;

	return src;
}

HttpRequest::HttpRequest(char* src)
	:	cookiesMapImmutable(cookiesMapInternal),
		headersMapImmutable(headersMapInternal),
		queryParametersMapImmutable(queryParametersMapInternal),
		bodyParametersMapImmutable(bodyParametersMapInternal)
{
	valid = false;
	protocolVersion = 0.0;
	
	VALIDATE_PTR(src);

	src = parseHttpRequestType(src);
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

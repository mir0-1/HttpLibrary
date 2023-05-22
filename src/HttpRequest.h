#pragma once
#include "RequestType.h"
#include "../../SimpleKeyValueParseLib/src/CommonImmutableMap.h"
#include <iostream>

class HttpRequest
{
	private:
		static const char* recognizedRequests[];
		static const int numberOfRecognizedRequests;
		static const char* commonProtocolSubstring;

		bool valid;
		RequestType requestType;
		std::string* pathToResource;
		double protocolVersion;

		CommonMutableMap queryParametersMapInternal;
		CommonMutableMap headersMapInternal;
		CommonMutableMap cookiesMapInternal;
		CommonMutableMap bodyParametersMapInternal;

		CommonImmutableMap queryParametersMapImmutable;
		CommonImmutableMap headersMapImmutable;
		CommonImmutableMap cookiesMapImmutable;
		CommonImmutableMap bodyParametersMapImmutable;

	private:
		char* parseRequestType(char* src);
		char* copyPathToResource(char* src);
		char* parseProtocolVersion(char* src);
		char* parseParametersFromResourcePath(char* src);
		char* validateNewlinePresent(char* src);
		char* parseHeaders(char* src);
		char* parseHeaderValueNonCookie(char* key, char* value);
		char* parseHeaderValueCookie(char* value);
		char* ignoreExtraSpaces(char* src);
		char* parseBody(char *src);

	public:
		bool isValid() const;
		RequestType getRequestType() const;
		std::string getPathToResource() const;
		double getProtocolVersion() const;
		CommonImmutableMap& getQueryParametersMap();
		CommonImmutableMap& getHeadersMap();
		CommonImmutableMap& getCookiesMap();
		CommonImmutableMap& getBodyParametersMap();

		HttpRequest(char* src);
};
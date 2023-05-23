#pragma once
#include "HttpRequestType.h"
#include "../../SimpleKeyValueParseLib/src/main/CommonParsableMap.h"
#include "../../SimpleKeyValueParseLib/src/template/CommonImmutableMap.h"
#include <iostream>

typedef CommonImmutableMap<std::string, ValueWrapper> HttpImmutableMap;

class HttpRequest
{
	private:
		static const char* recognizedRequests[];
		static const int numberOfRecognizedRequests;
		static const char* commonProtocolSubstring;

		bool valid;
		HttpRequestType requestType;
		std::string pathToResource;
		std::string resourceExtension;
		double protocolVersion;

		CommonParsableMap queryParametersMapInternal;
		CommonParsableMap headersMapInternal;
		CommonParsableMap cookiesMapInternal;
		CommonParsableMap bodyParametersMapInternal;

		HttpImmutableMap queryParametersMapImmutable;
		HttpImmutableMap headersMapImmutable;
		HttpImmutableMap cookiesMapImmutable;
		HttpImmutableMap bodyParametersMapImmutable;

	private:
		char* parseHttpRequestType(char* src);
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
		HttpRequestType getRequestType() const;
		const std::string& getPathToResource() const;
		const std::string& getResourceExtension() const;
		double getProtocolVersion() const;
		const HttpImmutableMap& getQueryParametersMap() const;
		const HttpImmutableMap& getHeadersMap() const;
		const HttpImmutableMap& getCookiesMap() const;
		const HttpImmutableMap& getBodyParametersMap() const;

		HttpRequest(char* src);
};
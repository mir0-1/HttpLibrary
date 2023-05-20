#pragma once
#include "RequestType.h"
#include "HttpImmutableMap.h"
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

		HttpMap httpQueryParametersMapInternal;
		HttpMap httpHeadersMapInternal;
		HttpMap httpCookiesMapInternal;

		HttpImmutableMap httpQueryParametersMapImmutable;
		HttpImmutableMap httpHeadersMapImmutable;
		HttpImmutableMap httpCookiesMapImmutable;

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
		char* parseKeyValuePairsCommon(char* src, char seperator, bool useSpaceOrNewlineAsLast, HttpMap& httpMap);

	public:
		bool isValid() const;
		RequestType getRequestType() const;
		std::string getPathToResource() const;
		double getProtocolVersion() const;
		HttpImmutableMap& getQueryParametersMap();
		HttpImmutableMap& getHeadersMap();
		HttpImmutableMap& getCookiesMap();

		friend std::ostream& operator<<(std::ostream& ostream, const HttpRequest& httpRequest);

		HttpRequest(char* src);
};
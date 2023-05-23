#pragma once
#include <iostream>
#include <string>
#include "HttpStatusCode.h"
#include "../../SimpleKeyValueParseLib/src/template/CommonMutableMap.h"
#include "HttpContentType.h"
#include "../../SimpleKeyValueParseLib/src/main/ValueWrapper.h"

typedef CommonIterableMap<std::string, ValueWrapper> HttpIterableMap;
typedef CommonMutableMap<std::string, ValueWrapper> HttpMutableMap;

class HttpResponseBuilder
{
	private:
		std::string output;
		std::string body;
		std::string contentType;
		HttpStatusCode statusCode;
		double protocolVersion;

		HttpIterableMap* headerMap;
		HttpIterableMap* cookieMap;
		HttpIterableMap* jsonMap;

		const char* mapStatusCode();
		void buildContentType();
		void buildHeaders();
		void buildCookies();
		void buildContentLengthAuto();
		void buildJsonBody();

	public:
		HttpResponseBuilder();

		HttpResponseBuilder& setStatusCode(HttpStatusCode code);
		HttpResponseBuilder& setProtocolVersion(double protocolVersion);
		HttpResponseBuilder& setHeaderMap(HttpIterableMap* headerMap);
		HttpResponseBuilder& setCookieMap(HttpIterableMap* cookieMap);
		HttpResponseBuilder& setJsonMap(HttpIterableMap* jsonMap);
		HttpResponseBuilder& setRawBody(const std::string& body);
		HttpResponseBuilder& setContentType(const std::string& contentType);
		HttpResponseBuilder& setContentType(HttpContentType contentType);
		std::string& build();
};

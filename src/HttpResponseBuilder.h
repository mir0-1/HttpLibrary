#pragma once
#include <iostream>
#include <string>
#include "HttpStatusCode.h"
#include "../../SimpleKeyValueParseLib/src/CommonIterableMap.h"
#include "HttpContentType.h"

class HttpResponseBuilder
{
	private:
		std::string output;
		std::string body;
		std::string contentType;
		HttpStatusCode statusCode;
		double protocolVersion;

		CommonIterableMap* headerMap;
		CommonIterableMap* cookieMap;
		CommonIterableMap* jsonMap;

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
		HttpResponseBuilder& setHeaderMap(CommonIterableMap* headerMap);
		HttpResponseBuilder& setCookieMap(CommonIterableMap* cookieMap);
		HttpResponseBuilder& setJsonMap(CommonIterableMap* jsonMap);
		HttpResponseBuilder& setRawBody(const std::string& body);
		HttpResponseBuilder& setContentType(const std::string& contentType);
		HttpResponseBuilder& setContentType(HttpContentType contentType);
		std::string& build();
};

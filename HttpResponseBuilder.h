#pragma once
#include <iostream>
#include "HttpStatusCode.h"
#include "HttpIterableMap.h"
#include "HttpContentType.h"

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

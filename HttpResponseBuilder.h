#pragma once
#include <iostream>
#include "HttpStatusCode.h"
#include "HttpMap.h"
#include "HttpContentType.h"

class HttpResponseBuilder
{
	private:
		std::string output;
		std::string body;
		std::string contentType;
		HttpStatusCode statusCode;
		double protocolVersion;

		HttpMap* headerMap;
		HttpMap* cookieMap;
		HttpMap* jsonMap;

	public:
		HttpResponseBuilder();

		HttpResponseBuilder& setStatusCode(HttpStatusCode code);
		HttpResponseBuilder& setProtocolVersion(double protocolVersion);
		HttpResponseBuilder& setHeaderMap(HttpMap* headerMap);
		HttpResponseBuilder& setCookieMap(HttpMap* cookieMap);
		HttpResponseBuilder& setJsonMap(HttpMap* jsonMap);
		HttpResponseBuilder& setRawBody(const std::string& body);
		HttpResponseBuilder& setContentType(const std::string& contentType);
		HttpResponseBuilder& setContentType(HttpContentType contentType);
};

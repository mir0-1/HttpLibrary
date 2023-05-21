#pragma once
#include <iostream>
#include "HttpStatusCode.h"
#include "HttpImmutableMap.h"
#include "HttpContentType.h"

class HttpResponseBuilder
{
	private:
		std::string output;
		std::string body;
		std::string contentType;
		HttpStatusCode statusCode;
		double protocolVersion;
		bool overallValid;
		bool responseCodeValid;

		HttpImmutableMap* headerMap;
		HttpImmutableMap* cookieMap;
		HttpImmutableMap* jsonMap;

		const char* mapStatusCode();

	public:
		HttpResponseBuilder();

		HttpResponseBuilder& setStatusCode(HttpStatusCode code);
		HttpResponseBuilder& setProtocolVersion(double protocolVersion);
		HttpResponseBuilder& setHeaderMap(HttpImmutableMap* headerMap);
		HttpResponseBuilder& setCookieMap(HttpImmutableMap* cookieMap);
		HttpResponseBuilder& setJsonMap(HttpImmutableMap* jsonMap);
		HttpResponseBuilder& setRawBody(const std::string& body);
		HttpResponseBuilder& setContentType(const std::string& contentType);
		HttpResponseBuilder& setContentType(HttpContentType contentType);
		std::string& build();
};

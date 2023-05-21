#include "HttpResponseBuilder.h"

HttpResponseBuilder::HttpResponseBuilder() 
{
	overallValid = false;
	responseCodeValid = false;
	cookieMap = nullptr;
	headerMap = nullptr;
	jsonMap = nullptr;
}

const char* HttpResponseBuilder::mapStatusCode()
{
	switch(statusCode)
	{
		case HttpStatusCode::OK:
			responseCodeValid = true;
			return "200 OK";
		case HttpStatusCode::BAD_REQUEST:
			responseCodeValid = true;
			return "400 Bad Request";
		case HttpStatusCode::NOT_FOUND:
			responseCodeValid = true;
			return "404 Not Found";
		case HttpStatusCode::UNAUTHORIZED:
			responseCodeValid = true;
			return "401 Unauthorized";
		case HttpStatusCode::TOO_MANY_REQUESTS:
			responseCodeValid = true;
			return "429 Too Many Requests";
	}

	responseCodeValid = false;
	return "500 Internal Server Error";
}

HttpResponseBuilder& HttpResponseBuilder::setStatusCode(HttpStatusCode code)
{
	this->statusCode = code;
	return *this;
}

HttpResponseBuilder& HttpResponseBuilder::setProtocolVersion(double protocolVersion)
{
	this->protocolVersion = protocolVersion;
	return *this;
}

HttpResponseBuilder& HttpResponseBuilder::setHeaderMap(HttpImmutableMap* headerMap)
{
	this->headerMap = headerMap;
	return *this;
}

HttpResponseBuilder& HttpResponseBuilder::setCookieMap(HttpImmutableMap* cookieMap)
{
	this->cookieMap = cookieMap;
	return *this;
}

HttpResponseBuilder& HttpResponseBuilder::setJsonMap(HttpImmutableMap* jsonMap)
{
	this->jsonMap = jsonMap;
	return *this;
}

HttpResponseBuilder& HttpResponseBuilder::setRawBody(const std::string& body)
{
	this->body = body;
	return *this;
}

HttpResponseBuilder& HttpResponseBuilder::setContentType(const std::string& contentType)
{
	this->contentType = contentType;
	return *this;
}

HttpResponseBuilder& HttpResponseBuilder::setContentType(HttpContentType contentType)
{
	if (contentType == HttpContentType::JSON)
		return setContentType("application/json");

	return setContentType("text/html");
}

std::string& HttpResponseBuilder::build()
{
	body.clear();
	body.append("HTTP/1.1 ");
	body.append(mapStatusCode());
	body.append("\r\n");
}

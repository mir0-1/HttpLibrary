#include "HttpResponseBuilder.h"

HttpResponseBuilder::HttpResponseBuilder() 
	:	output(128, '\0'),
		body(256, '\0')
{
	
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

HttpResponseBuilder& HttpResponseBuilder::setHeaderMap(HttpMap* headerMap)
{
	this->headerMap = headerMap;
	return *this;
}

HttpResponseBuilder& HttpResponseBuilder::setCookieMap(HttpMap* cookieMap)
{
	this->cookieMap = cookieMap;
	return *this;
}

HttpResponseBuilder& HttpResponseBuilder::setJsonMap(HttpMap* jsonMap)
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
	return *this;
}

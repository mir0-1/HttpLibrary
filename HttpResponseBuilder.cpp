#include "HttpResponseBuilder.h"

HttpResponseBuilder::HttpResponseBuilder() 
{
	cookieMap = nullptr;
	headerMap = nullptr;
	jsonMap = nullptr;
}

const char* HttpResponseBuilder::mapStatusCode()
{
	switch(statusCode)
	{
		case HttpStatusCode::OK:
			return "200 OK";
		case HttpStatusCode::BAD_REQUEST:
			return "400 Bad Request";
		case HttpStatusCode::NOT_FOUND:
			return "404 Not Found";
		case HttpStatusCode::UNAUTHORIZED:
			return "401 Unauthorized";
		case HttpStatusCode::TOO_MANY_REQUESTS:
			return "429 Too Many Requests";
	}

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

HttpResponseBuilder& HttpResponseBuilder::setHeaderMap(HttpIterableMap* headerMap)
{
	this->headerMap = headerMap;
	return *this;
}

HttpResponseBuilder& HttpResponseBuilder::setCookieMap(HttpIterableMap* cookieMap)
{
	this->cookieMap = cookieMap;
	return *this;
}

HttpResponseBuilder& HttpResponseBuilder::setJsonMap(HttpIterableMap* jsonMap)
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
	output.clear();
	output.append("HTTP/1.1 ");
	output.append(mapStatusCode());
	output.append("\r\n");

	if (!contentType.empty() && (headerMap == nullptr || !headerMap->hasKey("Content-Type")))
	{
		output.append("Content-Type: ");
		output.append(contentType);
		output.append("\r\n");
	}

	if (headerMap != nullptr)
	{
		headerMap->resetIterator();

		while (!headerMap->isIteratorAtEnd())
		{
			std::pair<std::string, HttpValue> header = headerMap->getNextPairFromIterator();
			output.append(header.first);
			output.append(": ");
			output.append(header.second.getAsString());
			output.append("\r\n");

			if (headerMap->isIteratorReset())
				break;
		}
	}

	if (cookieMap != nullptr)
	{
		cookieMap->resetIterator();

		while (!cookieMap->isIteratorAtEnd())
		{
			output.append("Set-Cookie: ");
			std::pair<std::string, HttpValue> cookie = cookieMap->getNextPairFromIterator();
			output.append(cookie.first);
			output.append("=");
			output.append(cookie.second.getAsString());
			output.append(";");
			output.append("\r\n");

			if (cookieMap->isIteratorReset())
				break;
		}
	}

	if (!body.empty() && (headerMap == nullptr || !headerMap->hasKey("Content-Length")))
	{
		output.append("Content-Length: ");
		output.append(std::to_string(body.length()));
		output.append("\r\n");
	}

	output.append("\r\n");

	if (!body.empty())
		output.append(body);

	return output;
}

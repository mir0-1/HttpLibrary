#include "HttpResponseBuilder.h"

HttpResponseBuilder::HttpResponseBuilder() 
{
	reset();
}

HttpResponseBuilder& HttpResponseBuilder::reset()
{
	cookieMap = nullptr;
	headerMap = nullptr;
	jsonMap = nullptr;
	protocolVersion = 1.1;
	output.clear();
	body.clear();
	contentType.clear();

	return *this;
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
			return "425 Too Early";
		case HttpStatusCode::FOUND:
			return "302 Found";
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

	if (contentType == HttpContentType::HTML)
		return setContentType("text/html");

	if (contentType == HttpContentType::CSS)
		return setContentType("text/css");

	if (contentType == HttpContentType::JS)
		return setContentType("text/javascript");

	return setContentType("text/plain");
}

void HttpResponseBuilder::buildContentType()
{
	output.append("Content-Type: ");
	output.append(contentType);
	output.append("; charset=utf-8\r\n");
}

void HttpResponseBuilder::buildHeaders()
{
	headerMap->resetIterator();

	while (!headerMap->isIteratorAtEnd())
	{
		std::pair<std::string, ValueWrapper> header = headerMap->getNextPairFromIterator();
		output.append(header.first);
		output.append(": ");
		output.append(header.second.getAsString());
		output.append("\r\n");

		if (headerMap->isIteratorReset())
			break;
	}
}

void HttpResponseBuilder::buildCookies()
{
	cookieMap->resetIterator();

	while (!cookieMap->isIteratorAtEnd())
	{
		output.append("Set-Cookie: ");
		std::pair<std::string, ValueWrapper> cookie = cookieMap->getNextPairFromIterator();
		output.append(cookie.first);
		output.append("=");
		output.append(cookie.second.getAsString());
		output.append("; HttpOnly\r\n");

		if (cookieMap->isIteratorReset())
			break;
	}
}

void HttpResponseBuilder::buildContentLengthAuto()
{
	output.append("Content-Length: ");
	output.append(std::to_string(body.length()));
	output.append("\r\n");
}

void HttpResponseBuilder::buildJsonBody()
{
	if (contentType.empty())
		setContentType(JSON);
	body.append("{");

	jsonMap->resetIterator();
	while (!jsonMap->isIteratorAtEnd())
	{
		std::pair<std::string, ValueWrapper> jsonPair = jsonMap->getNextPairFromIterator();
		body.append("\"");
		body.append(jsonPair.first);
		body.append("\":\"");

		std::string value = jsonPair.second.getAsString();

		const size_t jsonFieldOriginalLength = value.length();
		int noQuotesFirstIndex = 0;
		for (int i = 0; i < jsonFieldOriginalLength; i++)
		{
			if (value[i] == '\"')
			{
				if (i > noQuotesFirstIndex)
					body.append(value, noQuotesFirstIndex, i - noQuotesFirstIndex);
				noQuotesFirstIndex = i + 1;
				body.append("\\\"");
			}
		}

		if (noQuotesFirstIndex < jsonFieldOriginalLength )
			body.append(value, noQuotesFirstIndex, jsonFieldOriginalLength - noQuotesFirstIndex);

		body.append("\"");

		if (jsonMap->isIteratorReset())
			break;

		body.append(",");
	}

	body.append("}");
}

std::string& HttpResponseBuilder::build()
{
	output.clear();
	output.append("HTTP/1.1 ");
	output.append(mapStatusCode());
	output.append("\r\n");

	if (body.empty() && jsonMap != nullptr)
		buildJsonBody();

	if (!contentType.empty() && (headerMap == nullptr || !headerMap->hasKey("Content-Type")))
		buildContentType();

	if (headerMap != nullptr)
		buildHeaders();
	
	if (cookieMap != nullptr)
		buildCookies();

	if ((!body.empty() || jsonMap != nullptr) && (headerMap == nullptr || !headerMap->hasKey("Content-Length")))
		buildContentLengthAuto();

	output.append("\r\n");

	if (!body.empty())
		output.append(body);

	return output;
}

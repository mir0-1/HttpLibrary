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

void HttpResponseBuilder::buildContentTypeAuto()
{
	output.append("Content-Type: ");
	output.append(contentType);
	output.append("\r\n");
}

void HttpResponseBuilder::buildHeaders()
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

void HttpResponseBuilder::buildCookies()
{
	cookieMap->resetIterator();

	while (!cookieMap->isIteratorAtEnd())
	{
		output.append("Set-Cookie: ");
		std::pair<std::string, HttpValue> cookie = cookieMap->getNextPairFromIterator();
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
	body.append("{\r\n");

	jsonMap->resetIterator();
	while (!jsonMap->isIteratorAtEnd())
	{
		std::pair<std::string, HttpValue> jsonPair = jsonMap->getNextPairFromIterator();
		body.append("\t");
		body.append(jsonPair.first);
		body.append(": \"");

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

		if (noQuotesFirstIndex < jsonFieldOriginalLength - 1)
			body.append(value, noQuotesFirstIndex, jsonFieldOriginalLength - noQuotesFirstIndex);

		body.append("\"");

		if (jsonMap->isIteratorReset())
			break;

		body.append(",\r\n");
	}

	body.append("\r\n}");
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
		buildContentTypeAuto();

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

#include "../src/HttpResponseBuilder.h"
#include "../../SimpleTestingLibrary/assert.h"
#include "test-configuration.h"

void printResponse(const std::string& response, std::ostream* log)
{
	if (log != nullptr)
	{
		(*log) << "----------------------\nGenerating HTTP response" << std::endl;
		(*log) << response << std::endl;
	}
}

void initSomeHeadersCommon(HttpMutableMap &headers)
{
	headers.setValue("MyCustomHeader", ValueWrapper("customVal"));
	headers.setValue("Another", ValueWrapper("sth-else"));
}

void test_HttpResponseBuilder_noHeaders_noBody()
{
	HttpResponseBuilder httpResponseBuilder;

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::OK)
		.build();

	printResponse(result, testLogger);

	assertTrue(result == "HTTP/1.1 200 OK\r\n\r\n", exitOnFail, "Testing simple HTTP response", testLogger);
}

void test_HttpResponseBuilder_noHeaders_helloWorldHtml()
{
	HttpResponseBuilder httpResponseBuilder;

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::OK)
		.setContentType(HTML)
		.setRawBody("<html><body>Hello world!</body></html>")
		.build();

	printResponse(result, testLogger);

	assertTrue(result == "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: 38\r\n\r\n<html><body>Hello world!</body></html>", exitOnFail, "Testing simple HTTP response with HTML body", testLogger);
}

void test_HttpResponseBuilder_someHeaders_helloWorldHtml()
{
	HttpResponseBuilder httpResponseBuilder;
	HttpMutableMap headers;

	initSomeHeadersCommon(headers);

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::BAD_REQUEST)
		.setContentType(HTML)
		.setHeaderMap(&headers)
		.setRawBody("<html><body>Hello world!</body></html>")
		.build();

	printResponse(result, testLogger);

	assertTrue(result == "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html; charset=utf-8\r\nMyCustomHeader: customVal\r\nAnother: sth-else\r\nContent-Length: 38\r\n\r\n<html><body>Hello world!</body></html>", exitOnFail, "Testing HTTP response with HTML body (headers)", testLogger);
}

void test_HttpResponseBuilder_someHeaders_someCookies_helloWorldHtml()
{
	HttpResponseBuilder httpResponseBuilder;
	HttpMutableMap headers;

	initSomeHeadersCommon(headers);

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::NOT_FOUND)
		.setContentType(HTML)
		.setHeaderMap(&headers)
		.setCookieMap(&headers)
		.setRawBody("<html><body>Hello world!</body></html>")
		.build();

	printResponse(result, testLogger);

	assertTrue(result == "HTTP/1.1 404 Not Found\r\nContent-Type: text/html; charset=utf-8\r\nMyCustomHeader: customVal\r\nAnother: sth-else\r\nSet-Cookie: MyCustomHeader=customVal; HttpOnly\r\nSet-Cookie: Another=sth-else; HttpOnly\r\nContent-Length: 38\r\n\r\n<html><body>Hello world!</body></html>", exitOnFail, "Testing simple HTTP response with HTML body (headers & cookies)", testLogger);
}

void test_HttpResponseBuilder_someHeaders_someCookies_jsonBody()
{
	HttpResponseBuilder httpResponseBuilder;
	HttpMutableMap headers;

	initSomeHeadersCommon(headers);

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::UNAUTHORIZED)
		.setHeaderMap(&headers)
		.setCookieMap(&headers)
		.setJsonMap(&headers)
		.build();

	printResponse(result, testLogger);

	assertTrue(result == "HTTP/1.1 401 Unauthorized\r\nContent-Type: application/json; charset=utf-8\r\nMyCustomHeader: customVal\r\nAnother: sth-else\r\nSet-Cookie: MyCustomHeader=customVal; HttpOnly\r\nSet-Cookie: Another=sth-else; HttpOnly\r\nContent-Length: 57\r\n\r\n{\r\n\tMyCustomHeader: \"customVal\",\r\n\tAnother: \"sth-else\"\r\n}", exitOnFail, "Testing simple HTTP response with JSON body (headers & cookies)", testLogger);
}

void test_HttpResponseBuilder_someHeaders_someCookies_jsonBodyWithQuotes()
{
	HttpResponseBuilder httpResponseBuilder;
	HttpMutableMap headers, jsonMap;

	initSomeHeadersCommon(headers);

	jsonMap.setValue("Var1", ValueWrapper("random"));
	jsonMap.setValue("jsonvar2two", ValueWrapper("\"I quoted somebody\", said jsonvar2two"));

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::OK)
		.setHeaderMap(&headers)
		.setCookieMap(&headers)
		.setJsonMap(&jsonMap)
		.build();

	printResponse(result, testLogger);

	assertTrue(result == "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\nMyCustomHeader: customVal\r\nAnother: sth-else\r\nSet-Cookie: MyCustomHeader=customVal; HttpOnly\r\nSet-Cookie: Another=sth-else; HttpOnly\r\nContent-Length: 79\r\n\r\n{\r\n\tVar1: \"random\",\r\n\tjsonvar2two: \"\\\"I quoted somebody\\\", said jsonvar2two\"\r\n}", exitOnFail, "Testing simple HTTP response with JSON body (headers & cookies & json quotes)", testLogger);
}

void test_HttpResponseBuilder_someHeaders_helloWorldHtml_overrideContentTypeViaHeaderMap()
{
	HttpResponseBuilder httpResponseBuilder;
	HttpMutableMap headers;

	initSomeHeadersCommon(headers);
	headers.setValue("Content-Type", ValueWrapper("custom"));

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::BAD_REQUEST)
		.setContentType(HTML)
		.setHeaderMap(&headers)
		.setRawBody("<html><body>Hello world!</body></html>")
		.build();

	printResponse(result, testLogger);

	assertTrue(result == "HTTP/1.1 400 Bad Request\r\nAnother: sth-else\r\nMyCustomHeader: customVal\r\nContent-Type: custom\r\nContent-Length: 38\r\n\r\n<html><body>Hello world!</body></html>", exitOnFail, "Testing HTTP response with HTML body (headers, override Content-Type via header)", testLogger);
}

void test_HttpResponseBuilder_someHeaders_jsonBody_overrideContentTypeViaSetter()
{
	HttpResponseBuilder httpResponseBuilder;
	HttpMutableMap headers;

	initSomeHeadersCommon(headers);

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::UNAUTHORIZED)
		.setContentType("custom")
		.setHeaderMap(&headers)
		.setCookieMap(&headers)
		.setJsonMap(&headers)
		.build();

	printResponse(result, testLogger);

	assertTrue(result == "HTTP/1.1 401 Unauthorized\r\nContent-Type: custom; charset=utf-8\r\nMyCustomHeader: customVal\r\nAnother: sth-else\r\nSet-Cookie: MyCustomHeader=customVal; HttpOnly\r\nSet-Cookie: Another=sth-else; HttpOnly\r\nContent-Length: 57\r\n\r\n{\r\n\tMyCustomHeader: \"customVal\",\r\n\tAnother: \"sth-else\"\r\n}", exitOnFail, "Testing simple HTTP response with JSON body (headers & cookies, override Content-Type via setter)", testLogger);
}

void test_HttpResponseBuilder_someHeaders_noCookies_htmlBody_overrideContentLength()
{
	HttpResponseBuilder httpResponseBuilder;
	HttpMutableMap headers;

	initSomeHeadersCommon(headers);

	headers.setValue("Content-Length", ValueWrapper("1337"));

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::OK)
		.setHeaderMap(&headers)
		.setRawBody("<html>Hello world</html>")
		.build();

	printResponse(result, testLogger);

	assertTrue(result == "HTTP/1.1 200 OK\r\nAnother: sth-else\r\nMyCustomHeader: customVal\r\nContent-Length: 1337\r\n\r\n<html>Hello world</html>", exitOnFail, "Testing HTTP response with HTML body and some headers (check Content-Length override)", testLogger);
}

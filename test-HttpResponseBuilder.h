#include "HttpResponseBuilder.h"
#include "assert.h"

void test_HttpResponseBuilder_noHeaders_noBody()
{
	HttpResponseBuilder httpResponseBuilder;

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::OK)
		.build();

	std::cout << "----------------------\nGenerating HTTP response" << std::endl;
	std::cout << result << std::endl;

	assertTrue(result == "HTTP/1.1 200 OK\r\n\r\n", true, "Testing simple HTTP response", &std::cout);
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

	std::cout << "----------------------\nGenerating HTTP response" << std::endl;
	std::cout << result << std::endl;

	assertTrue(result == "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 38\r\n\r\n<html><body>Hello world!</body></html>", true, "Testing simple HTTP response with HTML body", &std::cout);
}

void test_HttpResponseBuilder_someHeaders_helloWorldHtml()
{
	HttpResponseBuilder httpResponseBuilder;
	HttpMap headers;

	headers.setValue("MyCustomHeader", HttpValue("customVal"));
	headers.setValue("Another", HttpValue("sth-else"));

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::BAD_REQUEST)
		.setContentType(HTML)
		.setHeaderMap(&headers)
		.setRawBody("<html><body>Hello world!</body></html>")
		.build();

	std::cout << "----------------------\nGenerating HTTP response" << std::endl;
	std::cout << result << std::endl;

	assertTrue(result == "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nMyCustomHeader: customVal\r\nAnother: sth-else\r\nContent-Length: 38\r\n\r\n<html><body>Hello world!</body></html>", true, "Testing HTTP response with HTML body (headers)", &std::cout);
}

void test_HttpResponseBuilder_someHeaders_someCookies_helloWorldHtml()
{
	HttpResponseBuilder httpResponseBuilder;
	HttpMap headers;

	headers.setValue("MyCustomHeader", HttpValue("customVal"));
	headers.setValue("Another", HttpValue("sth-else"));

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::NOT_FOUND)
		.setContentType(HTML)
		.setHeaderMap(&headers)
		.setCookieMap(&headers)
		.setRawBody("<html><body>Hello world!</body></html>")
		.build();

	std::cout << "----------------------\nGenerating HTTP response" << std::endl;
	std::cout << result << std::endl;

	assertTrue(result == "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nMyCustomHeader: customVal\r\nAnother: sth-else\r\nSet-Cookie: MyCustomHeader=customVal; HttpOnly\r\nSet-Cookie: Another=sth-else; HttpOnly\r\nContent-Length: 38\r\n\r\n<html><body>Hello world!</body></html>", true, "Testing simple HTTP response with HTML body (headers & cookies)", &std::cout);
}

void test_HttpResponseBuilder_someHeaders_someCookies_jsonBody()
{
	HttpResponseBuilder httpResponseBuilder;
	HttpMap headers;

	headers.setValue("MyCustomHeader", HttpValue("customVal"));
	headers.setValue("Another", HttpValue("sth-else"));

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::UNAUTHORIZED)
		.setHeaderMap(&headers)
		.setCookieMap(&headers)
		.setJsonMap(&headers)
		.build();

	std::cout << "----------------------\nGenerating HTTP response" << std::endl;
	std::cout << result << std::endl;

	assertTrue(result == "HTTP/1.1 401 Unauthorized\r\nContent-Type: application/json\r\nMyCustomHeader: customVal\r\nAnother: sth-else\r\nSet-Cookie: MyCustomHeader=customVal; HttpOnly\r\nSet-Cookie: Another=sth-else; HttpOnly\r\nContent-Length: 57\r\n\r\n{\r\n\tMyCustomHeader: \"customVal\",\r\n\tAnother: \"sth-else\"\r\n}", true, "Testing simple HTTP response with JSON body (headers & cookies)", &std::cout);
}

void test_HttpResponseBuilder_someHeaders_someCookies_jsonBodyWithQuotes()
{
	HttpResponseBuilder httpResponseBuilder;
	HttpMap headers, jsonMap;

	headers.setValue("MyCustomHeader", HttpValue("customVal"));
	headers.setValue("Another", HttpValue("sth-else"));

	jsonMap.setValue("Var1", HttpValue("random"));
	jsonMap.setValue("jsonvar2two", HttpValue("\"I quoted somebody\", said jsonvar2two"));

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::OK)
		.setHeaderMap(&headers)
		.setCookieMap(&headers)
		.setJsonMap(&jsonMap)
		.build();

	std::cout << "----------------------\nGenerating HTTP response" << std::endl;
	std::cout << result << std::endl;

	assertTrue(result == "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nMyCustomHeader: customVal\r\nAnother: sth-else\r\nSet-Cookie: MyCustomHeader=customVal; HttpOnly\r\nSet-Cookie: Another=sth-else; HttpOnly\r\nContent-Length: 79\r\n\r\n{\r\n\tVar1: \"random\",\r\n\tjsonvar2two: \"\\\"I quoted somebody\\\", said jsonvar2two\"\r\n}", true, "Testing simple HTTP response with JSON body (headers & cookies & json quotes)", &std::cout);
}

void test_HttpResponseBuilder_someHeaders_helloWorldHtml_overrideContentTypeViaHeaderMap()
{
	HttpResponseBuilder httpResponseBuilder;
	HttpMap headers;

	headers.setValue("MyCustomHeader", HttpValue("customVal"));
	headers.setValue("Another", HttpValue("sth-else"));
	headers.setValue("Content-Type", HttpValue("custom"));

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::BAD_REQUEST)
		.setContentType(HTML)
		.setHeaderMap(&headers)
		.setRawBody("<html><body>Hello world!</body></html>")
		.build();

	std::cout << "----------------------\nGenerating HTTP response" << std::endl;
	std::cout << result << std::endl;

	assertTrue(result == "HTTP/1.1 400 Bad Request\r\nAnother: sth-else\r\nMyCustomHeader: customVal\r\nContent-Type: custom\r\nContent-Length: 38\r\n\r\n<html><body>Hello world!</body></html>", true, "Testing HTTP response with HTML body (headers, override Content-Type via header)", &std::cout);
}

void test_HttpResponseBuilder_someHeaders_jsonBody_overrideContentTypeViaSetter()
{
	HttpResponseBuilder httpResponseBuilder;
	HttpMap headers;

	headers.setValue("MyCustomHeader", HttpValue("customVal"));
	headers.setValue("Another", HttpValue("sth-else"));

	std::string result = httpResponseBuilder
		.setProtocolVersion(1.1)
		.setStatusCode(HttpStatusCode::UNAUTHORIZED)
		.setContentType("custom")
		.setHeaderMap(&headers)
		.setCookieMap(&headers)
		.setJsonMap(&headers)
		.build();

	std::cout << "----------------------\nGenerating HTTP response" << std::endl;
	std::cout << result << std::endl;

	assertTrue(result == "HTTP/1.1 401 Unauthorized\r\nContent-Type: custom\r\nMyCustomHeader: customVal\r\nAnother: sth-else\r\nSet-Cookie: MyCustomHeader=customVal; HttpOnly\r\nSet-Cookie: Another=sth-else; HttpOnly\r\nContent-Length: 57\r\n\r\n{\r\n\tMyCustomHeader: \"customVal\",\r\n\tAnother: \"sth-else\"\r\n}", true, "Testing simple HTTP response with JSON body (headers & cookies, override Content-Type via setter)", &std::cout);
}
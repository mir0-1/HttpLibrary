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

	assertTrue(result == "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nMyCustomHeader: customVal\r\nAnother: sth-else\r\nSet-Cookie: MyCustomHeader=customVal;\r\nSet-Cookie: Another=sth-else;\r\nContent-Length: 38\r\n\r\n<html><body>Hello world!</body></html>", true, "Testing simple HTTP response with HTML body (headers & cookies)", &std::cout);
}

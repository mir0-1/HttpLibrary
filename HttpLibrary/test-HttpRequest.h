#include "HttpRequest.h"
#include "assert.h"

void test_HttpRequest_requestType()
{
	char request[] = "POST /example/path HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);


	assertTrue(httpRequest.isValid(), true, "Testing if simple GET request is valid", &std::cout);
	assertTrue(httpRequest.getRequestType() == RequestType::POST, true, "Testing RequestType", &std::cout);
}


void test_HttpRequest_pathToResource()
{
	char request[] = "GET /my/path?param1=key HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	assertTrue(httpRequest.isValid(), true, "Testing if simple POST request is valid with some parameters", &std::cout);
	assertTrue(httpRequest.getPathToResource() == "/my/path", true, "Testing PathToResource correctness", &std::cout);
}

void test_HttpRequest_query_empty()
{
	char request[] = "GET /my/path? HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is empty - only \"?\" and no params", &std::cout);
}

void test_HttpRequest_query_ampersandOnly()
{
	char request[] = "GET /my/path?& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?&\"", &std::cout);
}


void test_HttpRequest_query_noKeyValueSeperator()
{
	char request[] = "GET /my/path?param1 HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	const HttpMap& parametersMap = httpRequest.getParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when param has no value - \"?param1\"", &std::cout);
	assertTrue(!parametersMap.hasParameter("param1"), true, "Testing if valueless param1 is absent (not considered a map entry)", &std::cout);
}

void test_HttpRequest_query_singleEmptyValueParameter()
{
	char request[] = "GET /my/path?param1= HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	const HttpMap& parametersMap = httpRequest.getParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?param1=\"", &std::cout);
	assertTrue(parametersMap.hasParameter("param1"), true, "Testing if empty value param1 is present", &std::cout);
	assertTrue(parametersMap.getParameter("param1").getAsString() == "", true, "Testing if param1 actually has empty value", &std::cout);
}

void test_HttpRequest_query_singleEmptyValueParamAndAmpersand()
{
	char request[] = "GET /my/path?param1=& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	const HttpMap& parametersMap = httpRequest.getParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?param1=&\"", &std::cout);
	assertTrue(parametersMap.hasParameter("param1"), true, "Testing if empty value param1 is present (when there is &)", &std::cout);
	assertTrue(parametersMap.getParameter("param1").getAsString() == "", true, "Testing if param1 actually has empty value (when there is &)", &std::cout);
}

void test_HttpRequest_query_multipleEmptyValueParam()
{
	char request[] = "GET /my/path?param1=&param2= HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	const HttpMap& parametersMap = httpRequest.getParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?param1=&param2=\"", &std::cout);
	assertTrue(parametersMap.hasParameter("param1"), true, "Testing first if empty value param1 is present", &std::cout);
	assertTrue(parametersMap.getParameter("param1").getAsString() == "", true, "Testing first if param1 actually has empty value", &std::cout);
	assertTrue(parametersMap.hasParameter("param2"), true, "Testing second if empty value param2 is present", &std::cout);
	assertTrue(parametersMap.getParameter("param2").getAsString() == "", true, "Testing second if param2 actually has empty value", &std::cout);
}

void test_HttpRequest_query_mixedParams()
{
	char request[] = "GET /my/path?param1=&&param2&&&param3=& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	const HttpMap& parametersMap = httpRequest.getParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"param1=&&param2&&&param3=&\"", &std::cout);
	assertTrue(parametersMap.hasParameter("param1"), true, "Testing first if empty value param1 is present (when there is &/&&/&&&)", &std::cout);
	assertTrue(parametersMap.getParameter("param1").getAsString() == "", true, "Testing first if param1 actually has empty value (when there is &/&&/&&&)", &std::cout);
	assertTrue(!parametersMap.hasParameter("param2"), true, "Testing second if param2 is absent, not in key-value format (when there is &/&&/&&&)", &std::cout);
	assertTrue(parametersMap.hasParameter("param3"), true, "Testing third if empty value param3 is present (when there is &/&&/&&&)", &std::cout);
	assertTrue(parametersMap.getParameter("param3").getAsString() == "", true, "Testing third if param3 actually has empty value (when there is &/&&/&&&)", &std::cout);

}

void test_HttpRequest_query_multipleEmptyValueParamAndAmpersand()
{
	char request[] = "GET /my/path?param1=&param2=& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	const HttpMap& parametersMap = httpRequest.getParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?param1=&param2=&\"", &std::cout);
	assertTrue(parametersMap.hasParameter("param1"), true, "Testing first if empty value param1 is present (when there is &)", &std::cout);
	assertTrue(parametersMap.getParameter("param1").getAsString() == "", true, "Testing first if param1 actually has empty value (when there is &)", &std::cout);
	assertTrue(parametersMap.hasParameter("param2"), true, "Testing second if empty value param2 is present (when there is &)", &std::cout);
	assertTrue(parametersMap.getParameter("param2").getAsString() == "", true, "Testing second if param2 actually has empty value (when there is &)", &std::cout);
}

void test_HttpRequest_query_multipleExistingParameters()
{
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	const HttpMap& parameters = httpRequest.getParametersMap();

	HttpValue expectedFirst("value");
	HttpValue expectedSecond("343");

	const HttpValue& actualFirst = parameters.getParameter("firstParameter");
	const HttpValue& actualSecond = parameters.getParameter("secondParameter");

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?firstParameter=value&secondParameter=343\"", &std::cout);
	assertTrue(!actualFirst.isNull(), true, "Testing firstParameter not null", &std::cout);
	assertTrue(!actualSecond.isNull(), true, "Testing secondParameter not null", &std::cout);
	assertTrue(expectedFirst.getAsString() == actualFirst.getAsString(), true, "Testing first HttpParameter value", &std::cout);
	assertTrue(expectedSecond.getAsInt() == actualSecond.getAsInt(), true, "Testing second HttpParameter value (as int)", &std::cout);
}

void test_HttpRequest_query_multipleExistingParametersAndAmpersand()
{
	char request[] = "GET  /my/path?firstParameter=value&secondParameter=343&   HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	const HttpMap& parameters = httpRequest.getParametersMap();

	HttpValue expectedFirst("value");
	HttpValue expectedSecond("343");

	const HttpValue& actualFirst = parameters.getParameter("firstParameter");
	const HttpValue& actualSecond = parameters.getParameter("secondParameter");

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?firstParameter=value&secondParameter=343&\"", &std::cout);
	assertTrue(!actualFirst.isNull(), true, "Testing firstParameter not null (when there is &)", &std::cout);
	assertTrue(!actualSecond.isNull(), true, "Testing secondParameter not null (when there is &)", &std::cout);
	assertTrue(expectedFirst.getAsString() == actualFirst.getAsString(), true, "Testing first HttpParameter value (when there is &)", &std::cout);
	assertTrue(expectedSecond.getAsInt() == actualSecond.getAsInt(), true, "Testing second HttpParameter value (as int) (when there is &)", &std::cout);
}

void test_HttpRequest_query_multipleExistingParametersAndMultipleSeparator()
{
	char request[] = "GET  /my/path?firstParameter=value&&&secondParameter=343&&  HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	const HttpMap& parameters = httpRequest.getParametersMap();

	HttpValue expectedFirst("value");
	HttpValue expectedSecond("343");

	const HttpValue& actualFirst = parameters.getParameter("firstParameter");
	const HttpValue& actualSecond = parameters.getParameter("secondParameter");

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?firstParameter=value&&&secondParameter=343&&\"", &std::cout);
	assertTrue(!actualFirst.isNull(), true, "Testing firstParameter not null (when there is &&&)", &std::cout);
	assertTrue(!actualSecond.isNull(), true, "Testing secondParameter not null (when there is &&&)", &std::cout);
	assertTrue(expectedFirst.getAsString() == actualFirst.getAsString(), true, "Testing first HttpParameter value (when there is &&&)", &std::cout);
	assertTrue(expectedSecond.getAsInt() == actualSecond.getAsInt(), true, "Testing second HttpParameter value (as int) (when there is &&&)", &std::cout);
}

void test_HttpRequest_query_fictionalParameter()
{
	char request[] = "GET   /my/path?firstParameter=value&secondParameter=343   HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	const HttpMap& parameters = httpRequest.getParametersMap();

	assertTrue(parameters.getParameter("imaginaryParameter").isNull(), true, "Testing absence of imaginaryParameter", &std::cout);
}

void test_HttpRequest_protocolVersion_general()
{
	char request[] = "GET  /my/path  HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	assertTrue(httpRequest.isValid(), true, "Testing if HTTP/1.1 protocol request is valid", &std::cout);
	assertTrue(httpRequest.getProtocolVersion() == 1.1, true, "Testing if parsed protocol version is 1.1", &std::cout);
}

void test_HttpRequest_protocolVersion_uncommon()
{
	char request[] = "GET   /my/path?firstParameter=value&secondParameter=343   HTTP/2\r\n\r\n";

	HttpRequest httpRequest(request);

	assertTrue(httpRequest.isValid(), true, "Testing if HTTP/2 protocol request is valid", &std::cout);
	assertTrue(httpRequest.getProtocolVersion() == 2.0, true, "Testing if parsed protocol version is 2, with parameters", &std::cout);
}

void test_HttpRequest_protocolVersion_notANumber()
{
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343  HTTP/NaN\r\n\r\n";

	HttpRequest httpRequest(request);

	assertTrue(!httpRequest.isValid(), true, "Testing if HTTP/NaN protocol request is invalid", &std::cout);
}

void test_HttpRequest_protocolVersion_incorrectName()
{
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTPS/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	assertTrue(!httpRequest.isValid(), true, "Testing if HTTPS/1.1 protocol request is invalid", &std::cout);
}

void test_HttpRequest_headers_singleNonCookie()
{
	char request[] = "GET /my/path HTTP/1.1\r\nMyHeader: value\r\n";

	HttpRequest httpRequest(request);

	const HttpMap& headers = httpRequest.getHeadersMap();

	assertTrue(headers.hasParameter("MyHeader"), true, "Testing single header presence", &std::cout);
	assertTrue(headers.getParameter("MyHeader").getAsString() == "value", true, "Testing single header value correctness", &std::cout);
}

void test_HttpRequest_headers_multipleNonCookies()
{
	char request[] = "GET /my/path HTTP/1.1\r\nMyHeader: value\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);

	const HttpMap& headers = httpRequest.getHeadersMap();

	assertTrue(headers.hasParameter("MyHeader"), true, "Testing first header presence", &std::cout);
	assertTrue(headers.getParameter("MyHeader").getAsString() == "value", true, "Testing first header value correctness", &std::cout);
	assertTrue(headers.hasParameter("SecondHeader"), true, "Testing second header presence", &std::cout);
	assertTrue(headers.getParameter("SecondHeader").getAsString() == "anotherValue", true, "Testing second header value correctness", &std::cout);
}

void test_HttpRequest_headers_fictionalNonCookie()
{
	char request[] = "GET /my/path HTTP/1.1\r\nMyHeader: value\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);

	const HttpMap& headers = httpRequest.getHeadersMap();

	assertTrue(!headers.hasParameter("ThirdHeader"), true, "Testing non-existing header absence", &std::cout);;
}

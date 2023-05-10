#include "HttpRequest.h"
#include "assert.h"

void test_HttpRequest_requestType()
{
	// Given: valid POST request
	char request[] = "POST /example/path HTTP/1.1\r\n\r\n";

	// When: the request string is sent to HttpRequest constructor
	HttpRequest httpRequest(request);


	// Then:	the RequestType enum should correspond the value for POST
	//			and the httpRequest should be valid
	assertTrue(httpRequest.isValid(), true, "Testing if request is valid", &std::cout);
	assertTrue(httpRequest.getRequestType() == RequestType::POST, true, "Testing RequestType", &std::cout);
}


void test_HttpRequest_pathToResource()
{
	// Given: valid GET request
	char request[] = "GET /my/path?param1=key HTTP/1.1\r\n\r\n";

	// When: the request string is sent to HttpRequest constructor
	HttpRequest httpRequest(request);


	// Then:	the pathToResource should be "/my/path"
	//			and the httpRequest should be valid
	assertTrue(httpRequest.isValid(), true, "Testing if request is valid", &std::cout);
	assertTrue(httpRequest.getPathToResource() == "/my/path", true, "Testing PathToResource", &std::cout);
}


void test_HttpRequest_multipleExistingParameters()
{
	// Given: valid GET request with multiple params
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTP/1.1\r\n\r\n";

	// When: the request string is sent to HttpRequest constructor
	HttpRequest httpRequest(request);
	const HttpMap<HttpParameterValue>& parameters = httpRequest.getParametersMap();


	// Then:	both Parameters should be present in the map
	//			key "firstParameter" should be non-null have a value "value"
	//			key "secondParameter" should be non-null and have a value 343, obtainable as int
	HttpParameterValue expectedFirst("value");
	HttpParameterValue expectedSecond("343");

	const HttpParameterValue& actualFirst = parameters.getParameter("firstParameter");
	const HttpParameterValue& actualSecond = parameters.getParameter("secondParameter");

	assertTrue(!actualFirst.isNull(), true, "Testing first HttpParameter not null", &std::cout);
	assertTrue(!actualSecond.isNull(), true, "Testing second HttpParameter not null", &std::cout);
	assertTrue(expectedFirst.getAsString() == actualFirst.getAsString(), true, "Testing first HttpParameter value", &std::cout);
	assertTrue(expectedSecond.getAsInt() == actualSecond.getAsInt(), true, "Testing second HttpParameter value", &std::cout);
}

void test_HttpRequest_nullParameter()
{
	// Given: valid GET request with some params
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTP/1.1\r\n\r\n";

	// When: the request string is sent to HttpRequest constructor
	HttpRequest httpRequest(request);
	const HttpMap<HttpParameterValue>& parameters = httpRequest.getParametersMap();

	assertTrue(parameters.getParameter("imaginaryParameter").isNull(), true, "Testing null parameters", &std::cout);
}

void test_HttpRequest_protocolVersionGeneral()
{
	// Given: valid GET request
	char request[] = "GET /my/path HTTP/1.1\r\n\r\n";

	// When: the request string is sent to HttpRequest constructor
	HttpRequest httpRequest(request);

	assertTrue(httpRequest.getProtocolVersion() == 1.1, true, "Testing 1.1 protocol version", &std::cout);
}

void test_HttpRequest_protocolVersionUncommon()
{
	// Given: valid GET request
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTP/2\r\n\r\n";

	// When: the request string is sent to HttpRequest constructor
	HttpRequest httpRequest(request);

	assertTrue(httpRequest.getProtocolVersion() == 2.0, true, "Testing 2 protocol version with parameters", &std::cout);
}

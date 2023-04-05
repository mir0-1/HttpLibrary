#include "HttpRequest.h"
#include "assert.h"
#include "test-HttpRequest.h"

void test_HttpRequest_RequestType()
{
	// Given: valid POST request
	char request[] = "POST /example/path HTTP/1.1";

	// When: the request string is sent to HttpRequest constructor
	HttpRequest httpRequest(request);

	
	// Then:	the RequestType enum should correspond the value for POST
	//			and the httpRequest should be valid
	assertEquals<bool>(true, httpRequest.isValid(), true, "Testing if request is valid", &std::cout);
	assertEquals<RequestType>(RequestType::POST, httpRequest.getRequestType(), true, "Testing RequestType", &std::cout);
}


void test_HttpRequest_testPathToResource()
{
	// Given: valid GET request
	char request[] = "GET /my/path?param1=key HTTP/1.1";

	// When: the request string is sent to HttpRequest constructor
	HttpRequest httpRequest(request);


	// Then:	the pathToResource should be "/my/path"
	//			and the httpRequest should be valid
	assertEquals<bool>(true, httpRequest.isValid(), true, "Testing if request is valid", &std::cout);
	assertEquals<std::string>("/my/path", httpRequest.getPathToResource(), true, "Testing PathToResource", &std::cout);
}


void test_HttpRequest_testMultipleParameters()
{
	// Given: valid GET request with multiple params
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTP/1.1";

	// When: the request string is sent to HttpRequest constructor
	HttpRequest httpRequest(request);
	const HttpParametersMap& parameters = httpRequest.getParametersMap();


	// Then:	both Parameters should be present in the map
	//			key "firstParameter" should have a value "value"
	//			key "secondParameter" should have a value 343, obtainable as int
	HttpParameterValue expectedFirst("value");
	HttpParameterValue expectedSecond("343");

	assertEquals<std::string>(expectedFirst.getAsString(), parameters.getParameter("firstParameter").getAsString(), true, "Testing HttpParameters", &std::cout);
	assertEquals<int>(expectedSecond.getAsInt(), parameters.getParameter("secondParameter").getAsInt(), true, "Testing HttpParameters", &std::cout);
}
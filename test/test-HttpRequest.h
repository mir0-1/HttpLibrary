#pragma once
#include "../src/HttpRequest.h"
#include "../../SimpleTestingLibrary/assert.h"
#include "test-configuration.h"

void printRequest(char* request, std::ostream* testLogger)
{
	if (testLogger != nullptr)
	{
		(*testLogger) << "------------------------------------------------" << std::endl;
		(*testLogger) << "Given the following HTTP request" << std::endl;
		(*testLogger) << request << std::endl;
	}
}

void test_HttpRequest_requestType()
{
	char request[] = "POST /example/path HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if simple POST request is valid", testLogger);
	assertTrue(httpRequest.getRequestType() == RequestType::POST, exitOnFail, "Testing RequestType", testLogger);
}

void test_HttpRequest_requestType_extraSpaces()
{
	char request[] = "POST  /example/path   HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if simple POST request is valid (extra spaces)", testLogger);
	assertTrue(httpRequest.getRequestType() == RequestType::POST, exitOnFail, "Testing RequestType (extra spaces)", testLogger);
}

void test_HttpRequest_pathToResource_valid()
{
	char request[] = "GET /my/path?param1=key HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if simple GET request is valid with some parameters", testLogger);
	assertTrue(httpRequest.getPathToResource() == "/my/path", exitOnFail, "Testing PathToResource correctness", testLogger);
}

void test_HttpRequest_pathToResource_withSpaces()
{
	char request[] = "PUT /my folder/path?param1=key HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	assertTrue(!httpRequest.isValid(), exitOnFail, "Testing if simple PUT request is invalid when space in pathToResource", testLogger);
}

void test_HttpRequest_query_empty()
{
	char request[] = "GET /my/path? HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when query is empty - only \"?\" and no params", testLogger);
}

void test_HttpRequest_query_doubleQuestionMarkOnly()
{
	char request[] = "GET /my/path?? HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when query is only \"??\" and no params", testLogger);
}

void test_HttpRequest_query_ampersandOnly()
{
	char request[] = "GET /my/path?& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when query is \"?&\"", testLogger);
}


void test_HttpRequest_query_noKeyValueSeperator()
{
	char request[] = "DELETE /my/path?param1 HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when param has no value - \"?param1\"", testLogger);
	assertTrue(!parametersMap.hasKey("param1"), exitOnFail, "Testing if valueless param1 is absent (not considered a map entry)", testLogger);
}

void test_HttpRequest_query_singleParam_emptyValue()
{
	char request[] = "GET /my/path?param1= HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when query is \"?param1=\"", testLogger);
	assertTrue(parametersMap.hasKey("param1"), exitOnFail, "Testing if empty value param1 is present", testLogger);
	assertTrue(parametersMap.getValue("param1").getAsString() == "", exitOnFail, "Testing if param1 actually has empty value", testLogger);
}

void test_HttpRequest_query_singleParam_multipleKeyValueSeparatorAndAmpersandAfter()
{
	char request[] = "GET /my/path?param1====& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when query is \"?param1====&\"", testLogger);
	assertTrue(parametersMap.hasKey("param1==="), exitOnFail, "Testing if empty value param1 is present (in query \"?param1====&\")", testLogger);
	assertTrue(parametersMap.getValue("param1===").getAsString() == "", exitOnFail, "Testing if param1 actually has empty value (in query \"?param1====&\")", testLogger);
}

void test_HttpRequest_query_singleParam_multipleKeyValueSeparatorAndAmpersandBefore()
{
	char request[] = "TRACE /my/path?&param1==== HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when query is \"?&param1====\"", testLogger);
	assertTrue(parametersMap.hasKey("param1==="), exitOnFail, "Testing if empty value param1 is present (in query \"?&param1====&\")", testLogger);
	assertTrue(parametersMap.getValue("param1===").getAsString() == "", exitOnFail, "Testing if param1 actually has empty value (in query \"?&param1====&\")", testLogger);
}

void test_HttpRequest_query_singleParam_emptyValueAndAmpersand()
{
	char request[] = "GET /my/path?param1=& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when query is \"?param1=&\"", testLogger);
	assertTrue(parametersMap.hasKey("param1"), exitOnFail, "Testing if empty value param1 is present (when there is &)", testLogger);
	assertTrue(parametersMap.getValue("param1").getAsString() == "", exitOnFail, "Testing if param1 actually has empty value (when there is &)", testLogger);
}

void test_HttpRequest_query_multipleParams_emptyValue()
{
	char request[] = "POST /my/path?param1=&param2= HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when query is \"?param1=&param2=\"", testLogger);
	assertTrue(parametersMap.hasKey("param1"), exitOnFail, "Testing first if empty value param1 is present", testLogger);
	assertTrue(parametersMap.getValue("param1").getAsString() == "", exitOnFail, "Testing first if param1 actually has empty value", testLogger);
	assertTrue(parametersMap.hasKey("param2"), exitOnFail, "Testing second if empty value param2 is present", testLogger);
	assertTrue(parametersMap.getValue("param2").getAsString() == "", exitOnFail, "Testing second if param2 actually has empty value", testLogger);
}

void test_HttpRequest_query_multipleParams_allSeperatorChaos()
{
	char request[] = "GET /my/path?==&param1=&&param2&&&param3=& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when query is \"==&param1=&&param2&&&param3=&\"", testLogger);
	assertTrue(parametersMap.hasKey("param1"), exitOnFail, "Testing first if empty value param1 is present (when there is &/&&/&&&)", testLogger);
	assertTrue(parametersMap.getValue("param1").getAsString() == "", exitOnFail, "Testing first if param1 actually has empty value (when there is &/&&/&&&)", testLogger);
	assertTrue(!parametersMap.hasKey("param2"), exitOnFail, "Testing second if param2 is absent, not in key-value format (when there is &/&&/&&&)", testLogger);
	assertTrue(parametersMap.hasKey("param3"), exitOnFail, "Testing third if empty value param3 is present (when there is &/&&/&&&)", testLogger);
	assertTrue(parametersMap.getValue("param3").getAsString() == "", exitOnFail, "Testing third if param3 actually has empty value (when there is &/&&/&&&)", testLogger);

}

void test_HttpRequest_query_multipleParams_emptyValueAndAmpersand()
{
	char request[] = "GET /my/path?param1=&param2=& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when query is \"?param1=&param2=&\"", testLogger);
	assertTrue(parametersMap.hasKey("param1"), exitOnFail, "Testing first if empty value param1 is present (when there is &)", testLogger);
	assertTrue(parametersMap.getValue("param1").getAsString() == "", exitOnFail, "Testing first if param1 actually has empty value (when there is &)", testLogger);
	assertTrue(parametersMap.hasKey("param2"), exitOnFail, "Testing second if empty value param2 is present (when there is &)", testLogger);
	assertTrue(parametersMap.getValue("param2").getAsString() == "", exitOnFail, "Testing second if param2 actually has empty value (when there is &)", testLogger);
}

void test_HttpRequest_query_multipleParams()
{
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& parameters = httpRequest.getQueryParametersMap();

	ValueWrapper expectedFirst("value");
	ValueWrapper expectedSecond("343");

	const ValueWrapper& actualFirst = parameters.getValue("firstParameter");
	const ValueWrapper& actualSecond = parameters.getValue("secondParameter");

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when query is \"?firstParameter=value&secondParameter=343\"", testLogger);
	assertTrue(!actualFirst.isNull(), exitOnFail, "Testing firstParameter not null", testLogger);
	assertTrue(!actualSecond.isNull(), exitOnFail, "Testing secondParameter not null", testLogger);
	assertTrue(expectedFirst.getAsString() == actualFirst.getAsString(), exitOnFail, "Testing first HttpParameter value", testLogger);
	assertTrue(expectedSecond.getAsInt() == actualSecond.getAsInt(), exitOnFail, "Testing second HttpParameter value (as int)", testLogger);
}

void test_HttpRequest_query_multipleParams_ampersandAtEnd()
{
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& parameters = httpRequest.getQueryParametersMap();

	ValueWrapper expectedFirst("value");
	ValueWrapper expectedSecond("343");

	const ValueWrapper& actualFirst = parameters.getValue("firstParameter");
	const ValueWrapper& actualSecond = parameters.getValue("secondParameter");

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when query is \"?firstParameter=value&secondParameter=343&\"", testLogger);
	assertTrue(!actualFirst.isNull(), exitOnFail, "Testing firstParameter not null (when there is &)", testLogger);
	assertTrue(!actualSecond.isNull(), exitOnFail, "Testing secondParameter not null (when there is &)", testLogger);
	assertTrue(expectedFirst.getAsString() == actualFirst.getAsString(), exitOnFail, "Testing first HttpParameter value (when there is &)", testLogger);
	assertTrue(expectedSecond.getAsInt() == actualSecond.getAsInt(), exitOnFail, "Testing second HttpParameter value (as int) (when there is &)", testLogger);
}

void test_HttpRequest_query_multipleParams_multipleSeparator()
{
	char request[] = "GET /my/path?firstParameter=value&&&secondParameter=343&& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& parameters = httpRequest.getQueryParametersMap();

	ValueWrapper expectedFirst("value");
	ValueWrapper expectedSecond("343");

	const ValueWrapper& actualFirst = parameters.getValue("firstParameter");
	const ValueWrapper& actualSecond = parameters.getValue("secondParameter");

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when query is \"?firstParameter=value&&&secondParameter=343&&\"", testLogger);
	assertTrue(!actualFirst.isNull(), exitOnFail, "Testing firstParameter not null (when there is &&&)", testLogger);
	assertTrue(!actualSecond.isNull(), exitOnFail, "Testing secondParameter not null (when there is &&&)", testLogger);
	assertTrue(expectedFirst.getAsString() == actualFirst.getAsString(), exitOnFail, "Testing first HttpParameter value (when there is &&&)", testLogger);
	assertTrue(expectedSecond.getAsInt() == actualSecond.getAsInt(), exitOnFail, "Testing second HttpParameter value (as int) (when there is &&&)", testLogger);
}

void test_HttpRequest_query_multipleParams_fictionalParameter()
{
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& parameters = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request is valid when query is \"/my/path?firstParameter=value&secondParameter=343\" and extra spaces", testLogger);
	assertTrue(parameters.getValue("imaginaryParameter").isNull(), exitOnFail, "Testing absence of imaginaryParameter", testLogger);
}

void test_HttpRequest_protocolVersion_valid()
{
	char request[] = "GET /my/path HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if HTTP/1.1 protocol request is valid", testLogger);
	assertTrue(httpRequest.getProtocolVersion() == 1.1, exitOnFail, "Testing if parsed protocol version is 1.1", testLogger);
}

void test_HttpRequest_protocolVersion_valid_extraSpaces()
{
	char request[] = "GET   /my/path   HTTP/1.1  \r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if HTTP/1.1 protocol request is valid", testLogger);
	assertTrue(httpRequest.getProtocolVersion() == 1.1, exitOnFail, "Testing if parsed protocol version is 1.1", testLogger);
}

void test_HttpRequest_protocolVersion_valid_futureVersion()
{
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTP/2\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if HTTP/2 protocol request is valid", testLogger);
	assertTrue(httpRequest.getProtocolVersion() == 2.0, exitOnFail, "Testing if parsed protocol version is 2, with parameters", testLogger);
}

void test_HttpRequest_protocolVersion_notANumber()
{
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTP/NaN\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	assertTrue(!httpRequest.isValid(), exitOnFail, "Testing if HTTP/NaN protocol request is invalid", testLogger);
}

void test_HttpRequest_protocolVersion_incorrectName()
{
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTPS/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	assertTrue(!httpRequest.isValid(), exitOnFail, "Testing if HTTPS/1.1 protocol request is invalid", testLogger);
}

void test_HttpRequest_headers_singleNonCookie()
{
	char request[] = "GET /my/path HTTP/1.1\r\nMyHeader: value\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if single header request is valid", testLogger);
	assertTrue(headers.hasKey("MyHeader"), exitOnFail, "Testing single header presence", testLogger);
	assertTrue(headers.getValue("MyHeader").getAsString() == "value", exitOnFail, "Testing single header value correctness", testLogger);
}

void test_HttpRequest_headers_multipleNonCookies()
{
	char request[] = "GET /my/path HTTP/1.1\r\nMyHeader: value\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple header request is valid", testLogger);
	assertTrue(headers.hasKey("MyHeader"), exitOnFail, "Testing first header presence", testLogger);
	assertTrue(headers.getValue("MyHeader").getAsString() == "value", exitOnFail, "Testing first header value correctness", testLogger);
	assertTrue(headers.hasKey("SecondHeader"), exitOnFail, "Testing second header presence", testLogger);
	assertTrue(headers.getValue("SecondHeader").getAsString() == "anotherValue", exitOnFail, "Testing second header value correctness", testLogger);
}

void test_HttpRequest_headers_multipleNonCookies_spaceChaos()
{
	char request[] = "GET /my/path HTTP/1.1\r\nMyHe ader:va lu e\r\n  Sec ond Header:anoth erValue\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple header request is valid (\"space chaos\")", testLogger);
	assertTrue(headers.hasKey("MyHe ader"), exitOnFail, "Testing first header presence", testLogger);
	assertTrue(headers.getValue("MyHe ader").getAsString() == "va lu e", exitOnFail, "Testing first header value correctness (\"space chaos\")", testLogger);
	assertTrue(headers.hasKey("Sec ond Header"), exitOnFail, "Testing second header presence (\"space chaos\")", testLogger);
	assertTrue(headers.getValue("Sec ond Header").getAsString() == "anoth erValue", exitOnFail, "Testing second header value correctness (\"space chaos\")", testLogger);
}

void test_HttpRequest_headers_fictionalNonCookie()
{
	char request[] = "GET /my/path HTTP/1.1\r\nMyHeader: value\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple header request is valid, second time", testLogger);
	assertTrue(!headers.hasKey("ThirdHeader"), exitOnFail, "Testing non-existing header absence", testLogger);;
}

void test_HttpRequest_headers_singleCookie()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: key1=value1\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if single cookie request is valid", testLogger);
	assertTrue(cookies.hasKey("key1"), exitOnFail, "Testing single cookie presence", testLogger);
	assertTrue(cookies.getValue("key1").getAsString() == "value1", exitOnFail, "Testing single cookie value correctness", testLogger);
}

void test_HttpRequest_headers_singleCookie_emptyValue()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: key1=\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if single cookie request is valid (empty value)", testLogger);
	assertTrue(cookies.hasKey("key1"), exitOnFail, "Testing single cookie presence (empty value)", testLogger);
	assertTrue(cookies.getValue("key1").getAsString() == "", exitOnFail, "Testing single cookie empty value correctness", testLogger);
}

void test_HttpRequest_headers_singleCookie_oneKeyMultipleValues()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: key1=value1=value2\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if single cookie request is valid (one key two values)", testLogger);
	assertTrue(cookies.hasKey("key1=value1"), exitOnFail, "Testing single cookie presence (one key two values)", testLogger);
	assertTrue(cookies.getValue("key1=value1").getAsString() == "value2", exitOnFail, "Testing single cookie value correctness (one key two values)", testLogger);
}

void test_HttpRequest_headers_singleCookie_oneKeyMultipleValues_extraSeperators()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: key1====value1===value2\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if single cookie request is valid (one key two values)", testLogger);
	assertTrue(cookies.hasKey("key1====value1=="), exitOnFail, "Testing single cookie presence (one key two values)", testLogger);
	assertTrue(cookies.getValue("key1====value1==").getAsString() == "value2", exitOnFail, "Testing single cookie value correctness (one key two values)", testLogger);
}

void test_HttpRequest_headers_multipleCookies_sameHeader()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie=value; SecondCookie=anotherValue\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();
	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple cookie request is valid (same header)", testLogger);
	assertTrue(!headers.hasKey("Cookie"), exitOnFail, "Testing the absence of Cookie header in the header map (same header)", testLogger);
	assertTrue(cookies.hasKey("MyCookie"), exitOnFail, "Testing first cookie presence (same header)", testLogger);
	assertTrue(cookies.getValue("MyCookie").getAsString() == "value", exitOnFail, "Testing first cookie value correctness (same header)", testLogger);
	assertTrue(cookies.hasKey("SecondCookie"), exitOnFail, "Testing second cookie presence", testLogger);
	assertTrue(cookies.getValue("SecondCookie").getAsString() == "anotherValue", exitOnFail, "Testing second cookie value correctness (same header)", testLogger);
}

void test_HttpRequest_headers_multipleCookies_sameHeader_noValue()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie=; SecondCookie=\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();
	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple cookie request is valid (same header & no value)", testLogger);
	assertTrue(!headers.hasKey("Cookie"), exitOnFail, "Testing the absence of Cookie header in the header map (same header & no value)", testLogger);
	assertTrue(cookies.hasKey("MyCookie"), exitOnFail, "Testing first cookie presence (same header & no value)", testLogger);
	assertTrue(cookies.getValue("MyCookie").getAsString() == "", exitOnFail, "Testing first cookie value correctness (same header & no value)", testLogger);
	assertTrue(cookies.hasKey("SecondCookie"), exitOnFail, "Testing second cookie presence", testLogger);
	assertTrue(cookies.getValue("SecondCookie").getAsString() == "", exitOnFail, "Testing second cookie value correctness (same header & no value)", testLogger);
}

void test_HttpRequest_headers_multipleCookies_sameHeader_noValueNoSeparator()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie; SecondCookie\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();
	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple cookie request is valid (same header & no value, no \'=\')", testLogger);
	assertTrue(!headers.hasKey("Cookie"), exitOnFail, "Testing the absence of Cookie header in the header map (same header & no value, no \'=\')", testLogger);
	assertTrue(!cookies.hasKey("MyCookie"), exitOnFail, "Testing first cookie presence (same header & no value, no \'=\')", testLogger);
	assertTrue(!cookies.hasKey("SecondCookie"), exitOnFail, "Testing second cookie presence (same header & no value, no \'=\')", testLogger);
}

void test_HttpRequest_headers_multipleCookies_sameHeader_noValueNoSeparatorAndOneValid()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie; SecondCookie=test\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();
	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple cookie request is valid (same header & no value, no \'=\', 1 ok)", testLogger);
	assertTrue(!headers.hasKey("Cookie"), exitOnFail, "Testing the absence of Cookie header in the header map (same header & no value, no \'=\', 1 ok)", testLogger);
	assertTrue(!cookies.hasKey("MyCookie"), exitOnFail, "Testing first cookie presence (same header & no value, no \'=\', 1 ok)", testLogger);
	assertTrue(cookies.hasKey("SecondCookie"), exitOnFail, "Testing second cookie presence (same header & no value, no \'=\', 1 ok)", testLogger);
	assertTrue(cookies.getValue("SecondCookie").getAsString() == "test", exitOnFail, "Testing second cookie value correctness (same header & no value, no \'=\', 1 ok)", testLogger);
}

void test_HttpRequest_headers_multipleCookies_sameHeader_extraSpaces()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie=value;   SecondCookie=anotherValue\r\nSecondHeader: anotherValue;  \r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();
	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple cookie request is valid (same header with extra spaces)", testLogger);
	assertTrue(!headers.hasKey("Cookie"), exitOnFail, "Testing the absence of Cookie header in the header map (same header with extra spaces)", testLogger);
	assertTrue(cookies.hasKey("MyCookie"), exitOnFail, "Testing first cookie presence (same header with extra spaces)", testLogger);
	assertTrue(cookies.getValue("MyCookie").getAsString() == "value", exitOnFail, "Testing first cookie value correctness (same header with extra spaces)", testLogger);
	assertTrue(cookies.hasKey("SecondCookie"), exitOnFail, "Testing second cookie presence (same header with extra spaces)", testLogger);
	assertTrue(cookies.getValue("SecondCookie").getAsString() == "anotherValue", exitOnFail, "Testing second cookie value correctness (with extra spaces)", testLogger);
}

void test_HttpRequest_headers_multipleCookies_sameHeader_spaceInKey()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie=value; stray SecondCookie=anotherValue\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();
	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple cookie request is valid (space in key)", testLogger);
	assertTrue(!headers.hasKey("Cookie"), exitOnFail, "Testing the absence of Cookie header in the header map (space in key)", testLogger);
	assertTrue(cookies.hasKey("MyCookie"), exitOnFail, "Testing first cookie presence", testLogger);
	assertTrue(cookies.getValue("MyCookie").getAsString() == "value", exitOnFail, "Testing first cookie value correctness (space in key)", testLogger);
	assertTrue(cookies.hasKey("stray SecondCookie"), exitOnFail, "Testing second cookie presence (space in key)", testLogger);
	assertTrue(cookies.getValue("stray SecondCookie").getAsString() == "anotherValue", exitOnFail, "Testing second cookie value correctness (space in key)", testLogger);
}

void test_HttpRequest_headers_multipleCookies_differentHeader()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie=value;\r\nCookie: SecondCookie=anotherValue\r\nSecondHeader: anotherValue  \r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();
	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple cookie request is valid (different header)", testLogger);
	assertTrue(!headers.hasKey("Cookie"), exitOnFail, "Testing the absence of Cookie header in the header map (different header)", testLogger);
	assertTrue(cookies.hasKey("MyCookie"), exitOnFail, "Testing first cookie presence (different header)", testLogger);
	assertTrue(cookies.getValue("MyCookie").getAsString() == "value", exitOnFail, "Testing first cookie value correctness (different header)", testLogger);
	assertTrue(cookies.hasKey("SecondCookie"), exitOnFail, "Testing second cookie presence (different header)", testLogger);
	assertTrue(cookies.getValue("SecondCookie").getAsString() == "anotherValue", exitOnFail, "Testing second cookie value correctness (different header)", testLogger);
}

void test_HttpRequest_headers_multipleCookies_differentHeader_extraSpaces()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie=value;  \r\n  Cookie: SecondCookie=anotherValue ;   \r\nSecondHeader: anotherValue  \r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();
	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple cookie request is valid (different header & extra spaces)", testLogger);
	assertTrue(!headers.hasKey("Cookie"), exitOnFail, "Testing the absence of Cookie header in the header map (different header & extra spaces)", testLogger);
	assertTrue(cookies.hasKey("MyCookie"), exitOnFail, "Testing first cookie presence (different header & extra spaces)", testLogger);
	assertTrue(cookies.getValue("MyCookie").getAsString() == "value", exitOnFail, "Testing first cookie value correctness (different header & extra spaces)", testLogger);
	assertTrue(cookies.hasKey("SecondCookie"), exitOnFail, "Testing second cookie presence (different header)", testLogger);
	assertTrue(cookies.getValue("SecondCookie").getAsString() == "anotherValue ", exitOnFail, "Testing second cookie value correctness (different header & extra spaces)", testLogger);
}

void test_HttpRequest_headers_multipleCookies_differentHeader_spacesChaos()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie:MyCook ie=va l ue;  \r\n  Cookie: Secon dCookie=anoth erValue ;   \r\nSecondHeader: anotherValue  \r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();
	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple cookie request is valid (\"spaces chaos\")", testLogger);
	assertTrue(!headers.hasKey("MyCook ie"), exitOnFail, "Testing the absence of Cookie header in the header map (\"spaces chaos\")", testLogger);
	assertTrue(cookies.hasKey("MyCook ie"), exitOnFail, "Testing first cookie presence (\"spaces chaos\")", testLogger);
	assertTrue(cookies.getValue("MyCook ie").getAsString() == "va l ue", exitOnFail, "Testing first cookie value correctness (\"spaces chaos\")", testLogger);
	assertTrue(cookies.hasKey("Secon dCookie"), exitOnFail, "Testing second cookie presence (\"spaces chaos\")", testLogger);
	assertTrue(cookies.getValue("Secon dCookie").getAsString() == "anoth erValue ", exitOnFail, "Testing second cookie value correctness (\"spaces chaos\")", testLogger);
}

void test_HttpRequest_headers_multipleCookies_differentHeader_multipleSeparators()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie:MyCookie=value;;;SecondCookie=value;;\r\n  Cookie:;ThirdCookie=yetAnotherValue ;\r\nSecondHeader: anotherValue  \r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();
	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple cookie request is valid (different header & multiple \';\')", testLogger);
	assertTrue(!headers.hasKey("Cookie"), exitOnFail, "Testing the absence of Cookie header in the header map (different header & multiple \';\')", testLogger);
	assertTrue(cookies.hasKey("MyCookie"), exitOnFail, "Testing first cookie presence (different header & multiple \';\')", testLogger);
	assertTrue(cookies.getValue("MyCookie").getAsString() == "value", exitOnFail, "Testing first cookie value correctness (different header & multiple \';\')", testLogger);
	assertTrue(cookies.hasKey("SecondCookie"), exitOnFail, "Testing second cookie presence (different header)", testLogger);
	assertTrue(cookies.getValue("SecondCookie").getAsString() == "value", exitOnFail, "Testing second cookie value correctness (different header & multiple \';\')", testLogger);
	assertTrue(cookies.hasKey("ThirdCookie"), exitOnFail, "Testing third cookie presence (different header)", testLogger);
	assertTrue(cookies.getValue("ThirdCookie").getAsString() == "yetAnotherValue ", exitOnFail, "Testing third cookie value correctness (different header & multiple \';\')", testLogger);
}

void test_HttpRequest_headers_multipleCookies_differentHeader_allMultipleSeparators()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie:MyCookie===;;;SecondCookie==value;;\r\n  Cookie:;ThirdCookie====yetAnotherValue ;\r\nSecondHeader: anotherValue  \r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();
	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple cookie request is valid (different header & multiple \';\' and \'=\')", testLogger);
	assertTrue(!headers.hasKey("Cookie"), exitOnFail, "Testing the absence of Cookie header in the header map (different header & multiple \';\' and \'=\')", testLogger);
	assertTrue(cookies.hasKey("MyCookie=="), exitOnFail, "Testing first cookie presence (different header & multiple \';\')", testLogger);
	assertTrue(cookies.getValue("MyCookie==").getAsString() == "", exitOnFail, "Testing first cookie value correctness (different header & multiple \';\' and \'=\')", testLogger);
	assertTrue(cookies.hasKey("SecondCookie="), exitOnFail, "Testing second cookie presence (different header)", testLogger);
	assertTrue(cookies.getValue("SecondCookie=").getAsString() == "value", exitOnFail, "Testing second cookie value correctness (different header & multiple \';\' and \'=\')", testLogger);
	assertTrue(cookies.hasKey("ThirdCookie==="), exitOnFail, "Testing third cookie presence (different header)", testLogger);
	assertTrue(cookies.getValue("ThirdCookie===").getAsString() == "yetAnotherValue ", exitOnFail, "Testing third cookie value correctness (different header & multiple \';\' and \'=\')", testLogger);
}

void test_HttpRequest_headers_multipleCookies_differentHeader_allMultipleSeparators_missingCRs()
{
	char request[] = "GET /my/path HTTP/1.1\nCookie:MyCookie===;;;SecondCookie==value;;\n  Cookie:;ThirdCookie====yetAnotherValue ;\nSecondHeader: anotherValue  \n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& cookies = httpRequest.getCookiesMap();
	const CommonImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if multiple cookie request is valid (different header & multiple \';\' and \'=\')", testLogger);
	assertTrue(!headers.hasKey("Cookie"), exitOnFail, "Testing the absence of Cookie header in the header map (different header & multiple \';\' and \'=\')", testLogger);
	assertTrue(cookies.hasKey("MyCookie=="), exitOnFail, "Testing first cookie presence (different header & multiple \';\')", testLogger);
	assertTrue(cookies.getValue("MyCookie==").getAsString() == "", exitOnFail, "Testing first cookie value correctness (different header & multiple \';\' and \'=\')", testLogger);
	assertTrue(cookies.hasKey("SecondCookie="), exitOnFail, "Testing second cookie presence (different header)", testLogger);
	assertTrue(cookies.getValue("SecondCookie=").getAsString() == "value", exitOnFail, "Testing second cookie value correctness (different header & multiple \';\' and \'=\')", testLogger);
	assertTrue(cookies.hasKey("ThirdCookie==="), exitOnFail, "Testing third cookie presence (different header)", testLogger);
	assertTrue(cookies.getValue("ThirdCookie===").getAsString() == "yetAnotherValue ", exitOnFail, "Testing third cookie value correctness (different header & multiple \';\' and \'=\')", testLogger);
}

void test_HttpRequest_body_noHeaders()
{
	char request[] = "GET /my/path HTTP/1.1\r\n\r\nMyBody=value";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request with body is valid (no headers)", testLogger);
	assertTrue(bodyParams.hasKey("MyBody"), exitOnFail, "Testing presence of parameter MyBody (no headers)", testLogger);
	assertTrue(bodyParams.getValue("MyBody").getAsString() == "value", exitOnFail, "Testing MyBody value correctness (no headers)", testLogger);
}

void test_HttpRequest_body_noHeaders_extraNewlineMissing()
{
	char request[] = "GET /my/path HTTP/1.1\r\nMyBody=value\r\n";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(!httpRequest.isValid(), exitOnFail, "Testing if request with body is invalid (no headers & extra LF missing)", testLogger);
}

void test_HttpRequest_body_singleNonCookie()
{
	char request[] = "GET /my/path HTTP/1.1\r\nMyHeader: value\r\n\r\nMyBody=value";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request with body is valid (single non-cookie)", testLogger);
	assertTrue(bodyParams.hasKey("MyBody"), exitOnFail, "Testing presence of parameter MyBody (single non-cookie)", testLogger);
	assertTrue(bodyParams.getValue("MyBody").getAsString() == "value", exitOnFail, "Testing MyBody value correctness (single non-cookie)", testLogger);
}

void test_HttpRequest_body_singleCookie()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: key=value\r\n\r\nMyBody=value";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request with body is valid (single cookie)", testLogger);
	assertTrue(bodyParams.hasKey("MyBody"), exitOnFail, "Testing presence of parameter MyBody (single cookie)", testLogger);
	assertTrue(bodyParams.getValue("MyBody").getAsString() == "value", exitOnFail, "Testing MyBody value correctness (single cookie)", testLogger);
}

void test_HttpRequest_body_multipleCookiesSingleHeader()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: key=value; key2=value2\r\n\r\nMyBody=value";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request with body is valid (multiple cookie single header)", testLogger);
	assertTrue(bodyParams.hasKey("MyBody"), exitOnFail, "Testing presence of parameter MyBody (multiple cookie single header)", testLogger);
	assertTrue(bodyParams.getValue("MyBody").getAsString() == "value", exitOnFail, "Testing MyBody value correctness (multiple cookie single header)", testLogger);
}

void test_HttpRequest_body_noHeaders_multipleParameters()
{
	char request[] = "GET /my/path HTTP/1.1\r\n\r\nMyBody=value&AnotherKey=test";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request with body is valid (no headers & multiple params)", testLogger);
	assertTrue(bodyParams.hasKey("MyBody"), exitOnFail, "Testing presence of parameter MyBody (no headers & multiple params)", testLogger);
	assertTrue(bodyParams.getValue("MyBody").getAsString() == "value", exitOnFail, "Testing MyBody value correctness (no headers & multiple params)", testLogger);
	assertTrue(bodyParams.hasKey("AnotherKey"), exitOnFail, "Testing presence of parameter AnotherKey (no headers & multiple params)", testLogger);
	assertTrue(bodyParams.getValue("AnotherKey").getAsString() == "test", exitOnFail, "Testing AnotherKey value correctness (no headers & multiple params)", testLogger);
}

void test_HttpRequest_body_multipleHeaders_multipleParameters()
{
	char request[] = "GET /my/path HTTP/1.1\r\nFirstHeader: value\r\nSecondHeader: value\r\n\r\nMyBody=value&AnotherKey=test";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request with body is valid (multi headers & multiple params)", testLogger);
	assertTrue(bodyParams.hasKey("MyBody"), exitOnFail, "Testing presence of parameter MyBody (multi headers & multiple params)", testLogger);
	assertTrue(bodyParams.getValue("MyBody").getAsString() == "value", exitOnFail, "Testing MyBody value correctness (multi headers & multiple params)", testLogger);
	assertTrue(bodyParams.hasKey("AnotherKey"), exitOnFail, "Testing presence of parameter AnotherKey (multi headers & multiple params)", testLogger);
	assertTrue(bodyParams.getValue("AnotherKey").getAsString() == "test", exitOnFail, "Testing AnotherKey value correctness (multi headers & multiple params)", testLogger);
}

void test_HttpRequest_body_multipleHeaders_multipleParameters_spacesChaos()
{
	char request[] = "GET /my/path HTTP/1.1\r\nFir st Hea der:va lue\r\nSeco ndHea der : va lue\r\n\r\nMyB od y=va lu e&Anothe rKe y=te st ";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request with body is valid (multi headers & multiple params, \"space chaos\")", testLogger);
	assertTrue(bodyParams.hasKey("MyB od y"), exitOnFail, "Testing presence of parameter MyBody (multi headers & multiple params, \"space chaos\")", testLogger);
	assertTrue(bodyParams.getValue("MyB od y").getAsString() == "va lu e", exitOnFail, "Testing MyBody value correctness (multi headers & multiple params, \"space chaos\")", testLogger);
	assertTrue(bodyParams.hasKey("Anothe rKe y"), exitOnFail, "Testing presence of parameter AnotherKey (multi headers & multiple params, \"space chaos\")", testLogger);
	assertTrue(bodyParams.getValue("Anothe rKe y").getAsString() == "te st ", exitOnFail, "Testing AnotherKey value correctness (multi headers & multiple params, \"space chaos\")", testLogger);
}

void test_HttpRequest_body_multipleHeaders_multipleParameters_spacesChaos_missingCRs()
{
	char request[] = "GET /my/path HTTP/1.1\nFir st Hea der:va lue\nSeco ndHea der : va lue\n\nMyB od y=va lu e&Anothe rKe y=te st ";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request with body is valid (multi headers & multiple params, \"space chaos\", no \\r)", testLogger);
	assertTrue(bodyParams.hasKey("MyB od y"), exitOnFail, "Testing presence of parameter MyBody (multi headers & multiple params, \"space chaos\", no \\r)", testLogger);
	assertTrue(bodyParams.getValue("MyB od y").getAsString() == "va lu e", exitOnFail, "Testing MyBody value correctness (multi headers & multiple params, \"space chaos\", no \\r)", testLogger);
	assertTrue(bodyParams.hasKey("Anothe rKe y"), exitOnFail, "Testing presence of parameter AnotherKey (multi headers & multiple params, \"space chaos\", no \\r)", testLogger);
	assertTrue(bodyParams.getValue("Anothe rKe y").getAsString() == "te st ", exitOnFail, "Testing AnotherKey value correctness (multi headers & multiple params, \"space chaos\", no \\r)", testLogger);
}

void test_HttpRequest_body_multipleHeaders_multipleParameters_spacesChaos_missingCRs_extraSeparators()
{
	char request[] = "GET /my/path HTTP/1.1\nFir st Hea der:va lue\nSeco ndHea der : va lue\n\n&&MyB od y=va lu e&&&Anothe rKe y=te st &&";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request with body is valid (multi headers & multiple params, \"space chaos\", no \\r, more \'&\'')", testLogger);
	assertTrue(bodyParams.hasKey("MyB od y"), exitOnFail, "Testing presence of parameter MyBody (multi headers & multiple params, \"space chaos\", no \\r, more \'&\'')", testLogger);
	assertTrue(bodyParams.getValue("MyB od y").getAsString() == "va lu e", exitOnFail, "Testing MyBody value correctness (multi headers & multiple params, \"space chaos\", no \\r, , more \'&\')", testLogger);
	assertTrue(bodyParams.hasKey("Anothe rKe y"), exitOnFail, "Testing presence of parameter AnotherKey (multi headers & multiple params, \"space chaos\", no \\r, , more \'&\')", testLogger);
	assertTrue(bodyParams.getValue("Anothe rKe y").getAsString() == "te st ", exitOnFail, "Testing AnotherKey value correctness (multi headers & multiple params, \"space chaos\", no \\r, more \'&\')", testLogger);
}

void test_HttpRequest_body_multipleHeaders_multipleParameters_noValue()
{
	char request[] = "GET /my/path HTTP/1.1\r\nFirstHeader: value\r\nSecondHeader: value\r\n\r\nMyBody=value&keywnovalue&AnotherKey=test";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request with body is valid (param /w no value)", testLogger);
	assertTrue(bodyParams.hasKey("MyBody"), exitOnFail, "Testing presence of parameter MyBody (param /w no value)", testLogger);
	assertTrue(!bodyParams.hasKey("keywnovalue"), exitOnFail, "Testing absence of parameter keywnovalue (has no value)(param /w no value)", testLogger);
	assertTrue(bodyParams.getValue("MyBody").getAsString() == "value", exitOnFail, "Testing MyBody value correctness (param /w no value)", testLogger);
	assertTrue(bodyParams.hasKey("AnotherKey"), exitOnFail, "Testing presence of parameter AnotherKey (param /w no value)", testLogger);
	assertTrue(bodyParams.getValue("AnotherKey").getAsString() == "test", exitOnFail, "Testing AnotherKey value correctness (param /w no value)", testLogger);
}

void test_HttpRequest_body_multipleHeaders_multipleParameters_noValue_spacesChaos_extraSeparators()
{
	char request[] = "GET /my/path HTTP/1.1\r\nFirstHeader: value\r\nSecon dHe ader: v alu e\r\n\r\n&&&&MyBod y=v alue&&&keywno val ue&&&AnotherK ey=t est&";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request with body is valid (param /w no value, \"space chaos\", \'&\'')", testLogger);
	assertTrue(bodyParams.hasKey("MyBod y"), exitOnFail, "Testing presence of parameter MyBody (param /w no value, \"space chaos\", \'&\')", testLogger);
	assertTrue(!bodyParams.hasKey("keywno val ue"), exitOnFail, "Testing absence of parameter keywnovalue (has no value)(param /w no value, \"space chaos\", \'&\')", testLogger);
	assertTrue(bodyParams.getValue("MyBod y").getAsString() == "v alue", exitOnFail, "Testing MyBody value correctness (param /w no value, \"space chaos\", \'&\')", testLogger);
	assertTrue(bodyParams.hasKey("AnotherK ey"), exitOnFail, "Testing presence of parameter AnotherKey (param /w no value, \"space chaos\", \'&\')", testLogger);
	assertTrue(bodyParams.getValue("AnotherK ey").getAsString() == "t est", exitOnFail, "Testing AnotherKey value correctness (param /w no value, \"space chaos\", \'&\')", testLogger);
}

void test_HttpRequest_body_multipleHeaders_multipleParameters_keyWithEmptyValue_extraSeparators()
{
	char request[] = "GET /my/path HTTP/1.1\r\nFirstHeader: value\r\nSecon dHe ader: v alu e\r\n\r\n&&&&somekey=";

	HttpRequest httpRequest(request);
	printRequest(request, testLogger);

	const CommonImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), exitOnFail, "Testing if request with body is valid (param /w no value, \"space chaos\", \'&\'')", testLogger);
	assertTrue(bodyParams.hasKey("MyBod y"), exitOnFail, "Testing presence of parameter MyBody (param /w no value, \"space chaos\", \'&\')", testLogger);
	assertTrue(!bodyParams.hasKey("keywno val ue"), exitOnFail, "Testing absence of parameter keywnovalue (has no value)(param /w no value, \"space chaos\", \'&\')", testLogger);
	assertTrue(bodyParams.getValue("MyBod y").getAsString() == "v alue", exitOnFail, "Testing MyBody value correctness (param /w no value, \"space chaos\", \'&\')", testLogger);
	assertTrue(bodyParams.hasKey("AnotherK ey"), exitOnFail, "Testing presence of parameter AnotherKey (param /w no value, \"space chaos\", \'&\')", testLogger);
	assertTrue(bodyParams.getValue("AnotherK ey").getAsString() == "t est", exitOnFail, "Testing AnotherKey value correctness (param /w no value, \"space chaos\", \'&\')", testLogger);
}

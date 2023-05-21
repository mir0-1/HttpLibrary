#include "HttpRequest.h"
#include "assert.h"

void test_HttpRequest_requestType()
{
	char request[] = "POST /example/path HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);


	assertTrue(httpRequest.isValid(), true, "Testing if simple POST request is valid", &std::cout);
	assertTrue(httpRequest.getRequestType() == RequestType::POST, true, "Testing RequestType", &std::cout);
}

void test_HttpRequest_requestType_extraSpaces()
{
	char request[] = "POST  /example/path   HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);


	assertTrue(httpRequest.isValid(), true, "Testing if simple POST request is valid (extra spaces)", &std::cout);
	assertTrue(httpRequest.getRequestType() == RequestType::POST, true, "Testing RequestType (extra spaces)", &std::cout);
}

void test_HttpRequest_pathToResource_valid()
{
	char request[] = "GET /my/path?param1=key HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	assertTrue(httpRequest.isValid(), true, "Testing if simple GET request is valid with some parameters", &std::cout);
	assertTrue(httpRequest.getPathToResource() == "/my/path", true, "Testing PathToResource correctness", &std::cout);
}

void test_HttpRequest_pathToResource_withSpaces()
{
	char request[] = "GET /my folder/path?param1=key HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	assertTrue(!httpRequest.isValid(), true, "Testing if simple GET request is invalid when space in pathToResource", &std::cout);
}

void test_HttpRequest_query_empty()
{
	char request[] = "GET /my/path? HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is empty - only \"?\" and no params", &std::cout);
}

void test_HttpRequest_query_doubleQuestionMarkOnly()
{
	char request[] = "GET /my/path?? HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is only \"??\" and no params", &std::cout);
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

	const HttpImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when param has no value - \"?param1\"", &std::cout);
	assertTrue(!parametersMap.hasKey("param1"), true, "Testing if valueless param1 is absent (not considered a map entry)", &std::cout);
}

void test_HttpRequest_query_singleParam_emptyValue()
{
	char request[] = "GET /my/path?param1= HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?param1=\"", &std::cout);
	assertTrue(parametersMap.hasKey("param1"), true, "Testing if empty value param1 is present", &std::cout);
	assertTrue(parametersMap.getValue("param1").getAsString() == "", true, "Testing if param1 actually has empty value", &std::cout);
}

void test_HttpRequest_query_singleParam_multipleKeyValueSeparatorAndAmpersandAfter()
{
	char request[] = "GET /my/path?param1====& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?param1====&\"", &std::cout);
	assertTrue(parametersMap.hasKey("param1==="), true, "Testing if empty value param1 is present (in query \"?param1====&\")", &std::cout);
	assertTrue(parametersMap.getValue("param1===").getAsString() == "", true, "Testing if param1 actually has empty value (in query \"?param1====&\")", &std::cout);
}

void test_HttpRequest_query_singleParam_multipleKeyValueSeparatorAndAmpersandBefore()
{
	char request[] = "GET /my/path?&param1==== HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?&param1====\"", &std::cout);
	assertTrue(parametersMap.hasKey("param1==="), true, "Testing if empty value param1 is present (in query \"?&param1====&\")", &std::cout);
	assertTrue(parametersMap.getValue("param1===").getAsString() == "", true, "Testing if param1 actually has empty value (in query \"?&param1====&\")", &std::cout);
}

void test_HttpRequest_query_singleParam_emptyValueAndAmpersand()
{
	char request[] = "GET /my/path?param1=& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?param1=&\"", &std::cout);
	assertTrue(parametersMap.hasKey("param1"), true, "Testing if empty value param1 is present (when there is &)", &std::cout);
	assertTrue(parametersMap.getValue("param1").getAsString() == "", true, "Testing if param1 actually has empty value (when there is &)", &std::cout);
}

void test_HttpRequest_query_multipleParams_emptyValue()
{
	char request[] = "GET /my/path?param1=&param2= HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?param1=&param2=\"", &std::cout);
	assertTrue(parametersMap.hasKey("param1"), true, "Testing first if empty value param1 is present", &std::cout);
	assertTrue(parametersMap.getValue("param1").getAsString() == "", true, "Testing first if param1 actually has empty value", &std::cout);
	assertTrue(parametersMap.hasKey("param2"), true, "Testing second if empty value param2 is present", &std::cout);
	assertTrue(parametersMap.getValue("param2").getAsString() == "", true, "Testing second if param2 actually has empty value", &std::cout);
}

void test_HttpRequest_query_multipleParams_allSeperatorChaos()
{
	char request[] = "GET /my/path?==&param1=&&param2&&&param3=& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"==&param1=&&param2&&&param3=&\"", &std::cout);
	assertTrue(parametersMap.hasKey("param1"), true, "Testing first if empty value param1 is present (when there is &/&&/&&&)", &std::cout);
	assertTrue(parametersMap.getValue("param1").getAsString() == "", true, "Testing first if param1 actually has empty value (when there is &/&&/&&&)", &std::cout);
	assertTrue(!parametersMap.hasKey("param2"), true, "Testing second if param2 is absent, not in key-value format (when there is &/&&/&&&)", &std::cout);
	assertTrue(parametersMap.hasKey("param3"), true, "Testing third if empty value param3 is present (when there is &/&&/&&&)", &std::cout);
	assertTrue(parametersMap.getValue("param3").getAsString() == "", true, "Testing third if param3 actually has empty value (when there is &/&&/&&&)", &std::cout);

}

void test_HttpRequest_query_multipleParams_emptyValueAndAmpersand()
{
	char request[] = "GET /my/path?param1=&param2=& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& parametersMap = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?param1=&param2=&\"", &std::cout);
	assertTrue(parametersMap.hasKey("param1"), true, "Testing first if empty value param1 is present (when there is &)", &std::cout);
	assertTrue(parametersMap.getValue("param1").getAsString() == "", true, "Testing first if param1 actually has empty value (when there is &)", &std::cout);
	assertTrue(parametersMap.hasKey("param2"), true, "Testing second if empty value param2 is present (when there is &)", &std::cout);
	assertTrue(parametersMap.getValue("param2").getAsString() == "", true, "Testing second if param2 actually has empty value (when there is &)", &std::cout);
}

void test_HttpRequest_query_multipleParams()
{
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	const HttpImmutableMap& parameters = httpRequest.getQueryParametersMap();

	HttpValue expectedFirst("value");
	HttpValue expectedSecond("343");

	const HttpValue& actualFirst = parameters.getValue("firstParameter");
	const HttpValue& actualSecond = parameters.getValue("secondParameter");

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?firstParameter=value&secondParameter=343\"", &std::cout);
	assertTrue(!actualFirst.isNull(), true, "Testing firstParameter not null", &std::cout);
	assertTrue(!actualSecond.isNull(), true, "Testing secondParameter not null", &std::cout);
	assertTrue(expectedFirst.getAsString() == actualFirst.getAsString(), true, "Testing first HttpParameter value", &std::cout);
	assertTrue(expectedSecond.getAsInt() == actualSecond.getAsInt(), true, "Testing second HttpParameter value (as int)", &std::cout);
}

void test_HttpRequest_query_multipleParams_ampersandAtEnd()
{
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	const HttpImmutableMap& parameters = httpRequest.getQueryParametersMap();

	HttpValue expectedFirst("value");
	HttpValue expectedSecond("343");

	const HttpValue& actualFirst = parameters.getValue("firstParameter");
	const HttpValue& actualSecond = parameters.getValue("secondParameter");

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?firstParameter=value&secondParameter=343&\"", &std::cout);
	assertTrue(!actualFirst.isNull(), true, "Testing firstParameter not null (when there is &)", &std::cout);
	assertTrue(!actualSecond.isNull(), true, "Testing secondParameter not null (when there is &)", &std::cout);
	assertTrue(expectedFirst.getAsString() == actualFirst.getAsString(), true, "Testing first HttpParameter value (when there is &)", &std::cout);
	assertTrue(expectedSecond.getAsInt() == actualSecond.getAsInt(), true, "Testing second HttpParameter value (as int) (when there is &)", &std::cout);
}

void test_HttpRequest_query_multipleParams_multipleSeparator()
{
	char request[] = "GET /my/path?firstParameter=value&&&secondParameter=343&& HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	const HttpImmutableMap& parameters = httpRequest.getQueryParametersMap();

	HttpValue expectedFirst("value");
	HttpValue expectedSecond("343");

	const HttpValue& actualFirst = parameters.getValue("firstParameter");
	const HttpValue& actualSecond = parameters.getValue("secondParameter");

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"?firstParameter=value&&&secondParameter=343&&\"", &std::cout);
	assertTrue(!actualFirst.isNull(), true, "Testing firstParameter not null (when there is &&&)", &std::cout);
	assertTrue(!actualSecond.isNull(), true, "Testing secondParameter not null (when there is &&&)", &std::cout);
	assertTrue(expectedFirst.getAsString() == actualFirst.getAsString(), true, "Testing first HttpParameter value (when there is &&&)", &std::cout);
	assertTrue(expectedSecond.getAsInt() == actualSecond.getAsInt(), true, "Testing second HttpParameter value (as int) (when there is &&&)", &std::cout);
}

void test_HttpRequest_query_multipleParams_fictionalParameter()
{
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);
	const HttpImmutableMap& parameters = httpRequest.getQueryParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request is valid when query is \"/my/path?firstParameter=value&secondParameter=343\" and extra spaces", &std::cout);
	assertTrue(parameters.getValue("imaginaryParameter").isNull(), true, "Testing absence of imaginaryParameter", &std::cout);
}

void test_HttpRequest_protocolVersion_valid()
{
	char request[] = "GET /my/path HTTP/1.1\r\n\r\n";

	HttpRequest httpRequest(request);

	assertTrue(httpRequest.isValid(), true, "Testing if HTTP/1.1 protocol request is valid", &std::cout);
	assertTrue(httpRequest.getProtocolVersion() == 1.1, true, "Testing if parsed protocol version is 1.1", &std::cout);
}

void test_HttpRequest_protocolVersion_valid_extraSpaces()
{
	char request[] = "GET   /my/path   HTTP/1.1  \r\n\r\n";

	HttpRequest httpRequest(request);

	assertTrue(httpRequest.isValid(), true, "Testing if HTTP/1.1 protocol request is valid", &std::cout);
	assertTrue(httpRequest.getProtocolVersion() == 1.1, true, "Testing if parsed protocol version is 1.1", &std::cout);
}

void test_HttpRequest_protocolVersion_valid_futureVersion()
{
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTP/2\r\n\r\n";

	HttpRequest httpRequest(request);

	assertTrue(httpRequest.isValid(), true, "Testing if HTTP/2 protocol request is valid", &std::cout);
	assertTrue(httpRequest.getProtocolVersion() == 2.0, true, "Testing if parsed protocol version is 2, with parameters", &std::cout);
}

void test_HttpRequest_protocolVersion_notANumber()
{
	char request[] = "GET /my/path?firstParameter=value&secondParameter=343 HTTP/NaN\r\n\r\n";

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

	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if single header request is valid", &std::cout);
	assertTrue(headers.hasKey("MyHeader"), true, "Testing single header presence", &std::cout);
	assertTrue(headers.getValue("MyHeader").getAsString() == "value", true, "Testing single header value correctness", &std::cout);
}

void test_HttpRequest_headers_multipleNonCookies()
{
	char request[] = "GET /my/path HTTP/1.1\r\nMyHeader: value\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple header request is valid", &std::cout);
	assertTrue(headers.hasKey("MyHeader"), true, "Testing first header presence", &std::cout);
	assertTrue(headers.getValue("MyHeader").getAsString() == "value", true, "Testing first header value correctness", &std::cout);
	assertTrue(headers.hasKey("SecondHeader"), true, "Testing second header presence", &std::cout);
	assertTrue(headers.getValue("SecondHeader").getAsString() == "anotherValue", true, "Testing second header value correctness", &std::cout);
}

void test_HttpRequest_headers_multipleNonCookies_spaceChaos()
{
	char request[] = "GET /my/path HTTP/1.1\r\nMyHe ader:va lu e\r\n  Sec ond Header:anoth erValue\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple header request is valid (\"space chaos\")", &std::cout);
	assertTrue(headers.hasKey("MyHe ader"), true, "Testing first header presence", &std::cout);
	assertTrue(headers.getValue("MyHe ader").getAsString() == "va lu e", true, "Testing first header value correctness (\"space chaos\")", &std::cout);
	assertTrue(headers.hasKey("Sec ond Header"), true, "Testing second header presence (\"space chaos\")", &std::cout);
	assertTrue(headers.getValue("Sec ond Header").getAsString() == "anoth erValue", true, "Testing second header value correctness (\"space chaos\")", &std::cout);
}

void test_HttpRequest_headers_fictionalNonCookie()
{
	char request[] = "GET /my/path HTTP/1.1\r\nMyHeader: value\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple header request is valid, second time", &std::cout);
	assertTrue(!headers.hasKey("ThirdHeader"), true, "Testing non-existing header absence", &std::cout);;
}

void test_HttpRequest_headers_singleCookie()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: key1=value1\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();

	assertTrue(httpRequest.isValid(), true, "Testing if single cookie request is valid", &std::cout);
	assertTrue(cookies.hasKey("key1"), true, "Testing single cookie presence", &std::cout);
	assertTrue(cookies.getValue("key1").getAsString() == "value1", true, "Testing single cookie value correctness", &std::cout);
}

void test_HttpRequest_headers_singleCookie_emptyValue()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: key1=\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();

	assertTrue(httpRequest.isValid(), true, "Testing if single cookie request is valid (empty value)", &std::cout);
	assertTrue(cookies.hasKey("key1"), true, "Testing single cookie presence (empty value)", &std::cout);
	assertTrue(cookies.getValue("key1").getAsString() == "", true, "Testing single cookie empty value correctness", &std::cout);
}

void test_HttpRequest_headers_singleCookie_oneKeyMultipleValues()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: key1=value1=value2\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();

	assertTrue(httpRequest.isValid(), true, "Testing if single cookie request is valid (one key two values)", &std::cout);
	assertTrue(cookies.hasKey("key1=value1"), true, "Testing single cookie presence (one key two values)", &std::cout);
	assertTrue(cookies.getValue("key1=value1").getAsString() == "value2", true, "Testing single cookie value correctness (one key two values)", &std::cout);
}

void test_HttpRequest_headers_singleCookie_oneKeyMultipleValues_extraSeperators()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: key1====value1===value2\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();

	assertTrue(httpRequest.isValid(), true, "Testing if single cookie request is valid (one key two values)", &std::cout);
	assertTrue(cookies.hasKey("key1====value1=="), true, "Testing single cookie presence (one key two values)", &std::cout);
	assertTrue(cookies.getValue("key1====value1==").getAsString() == "value2", true, "Testing single cookie value correctness (one key two values)", &std::cout);
}

void test_HttpRequest_headers_multipleCookies_sameHeader()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie=value; SecondCookie=anotherValue\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();
	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple cookie request is valid (same header)", &std::cout);
	assertTrue(!headers.hasKey("Cookie"), true, "Testing the absence of Cookie header in the header map (same header)", &std::cout);
	assertTrue(cookies.hasKey("MyCookie"), true, "Testing first cookie presence (same header)", &std::cout);
	assertTrue(cookies.getValue("MyCookie").getAsString() == "value", true, "Testing first cookie value correctness (same header)", &std::cout);
	assertTrue(cookies.hasKey("SecondCookie"), true, "Testing second cookie presence", &std::cout);
	assertTrue(cookies.getValue("SecondCookie").getAsString() == "anotherValue", true, "Testing second cookie value correctness (same header)", &std::cout);
}

void test_HttpRequest_headers_multipleCookies_sameHeader_noValue()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie=; SecondCookie=\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();
	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple cookie request is valid (same header & no value)", &std::cout);
	assertTrue(!headers.hasKey("Cookie"), true, "Testing the absence of Cookie header in the header map (same header & no value)", &std::cout);
	assertTrue(cookies.hasKey("MyCookie"), true, "Testing first cookie presence (same header & no value)", &std::cout);
	assertTrue(cookies.getValue("MyCookie").getAsString() == "", true, "Testing first cookie value correctness (same header & no value)", &std::cout);
	assertTrue(cookies.hasKey("SecondCookie"), true, "Testing second cookie presence", &std::cout);
	assertTrue(cookies.getValue("SecondCookie").getAsString() == "", true, "Testing second cookie value correctness (same header & no value)", &std::cout);
}

void test_HttpRequest_headers_multipleCookies_sameHeader_noValueNoSeparator()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie; SecondCookie\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();
	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple cookie request is valid (same header & no value, no \'=\')", &std::cout);
	assertTrue(!headers.hasKey("Cookie"), true, "Testing the absence of Cookie header in the header map (same header & no value, no \'=\')", &std::cout);
	assertTrue(!cookies.hasKey("MyCookie"), true, "Testing first cookie presence (same header & no value, no \'=\')", &std::cout);
	assertTrue(!cookies.hasKey("SecondCookie"), true, "Testing second cookie presence (same header & no value, no \'=\')", &std::cout);
}

void test_HttpRequest_headers_multipleCookies_sameHeader_noValueNoSeparatorAndOneValid()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie; SecondCookie=test\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();
	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple cookie request is valid (same header & no value, no \'=\', 1 ok)", &std::cout);
	assertTrue(!headers.hasKey("Cookie"), true, "Testing the absence of Cookie header in the header map (same header & no value, no \'=\', 1 ok)", &std::cout);
	assertTrue(!cookies.hasKey("MyCookie"), true, "Testing first cookie presence (same header & no value, no \'=\', 1 ok)", &std::cout);
	assertTrue(cookies.hasKey("SecondCookie"), true, "Testing second cookie presence (same header & no value, no \'=\', 1 ok)", &std::cout);
	assertTrue(cookies.getValue("SecondCookie").getAsString() == "test", true, "Testing second cookie value correctness (same header & no value, no \'=\', 1 ok)", &std::cout);
}

void test_HttpRequest_headers_multipleCookies_sameHeader_extraSpaces()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie=value;   SecondCookie=anotherValue\r\nSecondHeader: anotherValue;  \r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();
	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple cookie request is valid (same header with extra spaces)", &std::cout);
	assertTrue(!headers.hasKey("Cookie"), true, "Testing the absence of Cookie header in the header map (same header with extra spaces)", &std::cout);
	assertTrue(cookies.hasKey("MyCookie"), true, "Testing first cookie presence (same header with extra spaces)", &std::cout);
	assertTrue(cookies.getValue("MyCookie").getAsString() == "value", true, "Testing first cookie value correctness (same header with extra spaces)", &std::cout);
	assertTrue(cookies.hasKey("SecondCookie"), true, "Testing second cookie presence (same header with extra spaces)", &std::cout);
	assertTrue(cookies.getValue("SecondCookie").getAsString() == "anotherValue", true, "Testing second cookie value correctness (with extra spaces)", &std::cout);
}

void test_HttpRequest_headers_multipleCookies_sameHeader_spaceInKey()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie=value; stray SecondCookie=anotherValue\r\nSecondHeader: anotherValue\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();
	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple cookie request is valid (space in key)", &std::cout);
	assertTrue(!headers.hasKey("Cookie"), true, "Testing the absence of Cookie header in the header map (space in key)", &std::cout);
	assertTrue(cookies.hasKey("MyCookie"), true, "Testing first cookie presence", &std::cout);
	assertTrue(cookies.getValue("MyCookie").getAsString() == "value", true, "Testing first cookie value correctness (space in key)", &std::cout);
	assertTrue(cookies.hasKey("stray SecondCookie"), true, "Testing second cookie presence (space in key)", &std::cout);
	assertTrue(cookies.getValue("stray SecondCookie").getAsString() == "anotherValue", true, "Testing second cookie value correctness (space in key)", &std::cout);
}

void test_HttpRequest_headers_multipleCookies_differentHeader()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie=value;\r\nCookie: SecondCookie=anotherValue\r\nSecondHeader: anotherValue  \r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();
	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple cookie request is valid (different header)", &std::cout);
	assertTrue(!headers.hasKey("Cookie"), true, "Testing the absence of Cookie header in the header map (different header)", &std::cout);
	assertTrue(cookies.hasKey("MyCookie"), true, "Testing first cookie presence (different header)", &std::cout);
	assertTrue(cookies.getValue("MyCookie").getAsString() == "value", true, "Testing first cookie value correctness (different header)", &std::cout);
	assertTrue(cookies.hasKey("SecondCookie"), true, "Testing second cookie presence (different header)", &std::cout);
	assertTrue(cookies.getValue("SecondCookie").getAsString() == "anotherValue", true, "Testing second cookie value correctness (different header)", &std::cout);
}

void test_HttpRequest_headers_multipleCookies_differentHeader_extraSpaces()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: MyCookie=value;  \r\n  Cookie: SecondCookie=anotherValue ;   \r\nSecondHeader: anotherValue  \r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();
	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple cookie request is valid (different header & extra spaces)", &std::cout);
	assertTrue(!headers.hasKey("Cookie"), true, "Testing the absence of Cookie header in the header map (different header & extra spaces)", &std::cout);
	assertTrue(cookies.hasKey("MyCookie"), true, "Testing first cookie presence (different header & extra spaces)", &std::cout);
	assertTrue(cookies.getValue("MyCookie").getAsString() == "value", true, "Testing first cookie value correctness (different header & extra spaces)", &std::cout);
	assertTrue(cookies.hasKey("SecondCookie"), true, "Testing second cookie presence (different header)", &std::cout);
	assertTrue(cookies.getValue("SecondCookie").getAsString() == "anotherValue ", true, "Testing second cookie value correctness (different header & extra spaces)", &std::cout);
}

void test_HttpRequest_headers_multipleCookies_differentHeader_spacesChaos()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie:MyCook ie=va l ue;  \r\n  Cookie: Secon dCookie=anoth erValue ;   \r\nSecondHeader: anotherValue  \r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();
	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple cookie request is valid (\"spaces chaos\")", &std::cout);
	assertTrue(!headers.hasKey("MyCook ie"), true, "Testing the absence of Cookie header in the header map (\"spaces chaos\")", &std::cout);
	assertTrue(cookies.hasKey("MyCook ie"), true, "Testing first cookie presence (\"spaces chaos\")", &std::cout);
	assertTrue(cookies.getValue("MyCook ie").getAsString() == "va l ue", true, "Testing first cookie value correctness (\"spaces chaos\")", &std::cout);
	assertTrue(cookies.hasKey("Secon dCookie"), true, "Testing second cookie presence (\"spaces chaos\")", &std::cout);
	assertTrue(cookies.getValue("Secon dCookie").getAsString() == "anoth erValue ", true, "Testing second cookie value correctness (\"spaces chaos\")", &std::cout);
}

void test_HttpRequest_headers_multipleCookies_differentHeader_multipleSeparators()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie:MyCookie=value;;;SecondCookie=value;;\r\n  Cookie:;ThirdCookie=yetAnotherValue ;\r\nSecondHeader: anotherValue  \r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();
	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple cookie request is valid (different header & multiple \';\')", &std::cout);
	assertTrue(!headers.hasKey("Cookie"), true, "Testing the absence of Cookie header in the header map (different header & multiple \';\')", &std::cout);
	assertTrue(cookies.hasKey("MyCookie"), true, "Testing first cookie presence (different header & multiple \';\')", &std::cout);
	assertTrue(cookies.getValue("MyCookie").getAsString() == "value", true, "Testing first cookie value correctness (different header & multiple \';\')", &std::cout);
	assertTrue(cookies.hasKey("SecondCookie"), true, "Testing second cookie presence (different header)", &std::cout);
	assertTrue(cookies.getValue("SecondCookie").getAsString() == "value", true, "Testing second cookie value correctness (different header & multiple \';\')", &std::cout);
	assertTrue(cookies.hasKey("ThirdCookie"), true, "Testing third cookie presence (different header)", &std::cout);
	assertTrue(cookies.getValue("ThirdCookie").getAsString() == "yetAnotherValue ", true, "Testing third cookie value correctness (different header & multiple \';\')", &std::cout);
}

void test_HttpRequest_headers_multipleCookies_differentHeader_allMultipleSeparators()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie:MyCookie===;;;SecondCookie==value;;\r\n  Cookie:;ThirdCookie====yetAnotherValue ;\r\nSecondHeader: anotherValue  \r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();
	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple cookie request is valid (different header & multiple \';\' and \'=\')", &std::cout);
	assertTrue(!headers.hasKey("Cookie"), true, "Testing the absence of Cookie header in the header map (different header & multiple \';\' and \'=\')", &std::cout);
	assertTrue(cookies.hasKey("MyCookie=="), true, "Testing first cookie presence (different header & multiple \';\')", &std::cout);
	assertTrue(cookies.getValue("MyCookie==").getAsString() == "", true, "Testing first cookie value correctness (different header & multiple \';\' and \'=\')", &std::cout);
	assertTrue(cookies.hasKey("SecondCookie="), true, "Testing second cookie presence (different header)", &std::cout);
	assertTrue(cookies.getValue("SecondCookie=").getAsString() == "value", true, "Testing second cookie value correctness (different header & multiple \';\' and \'=\')", &std::cout);
	assertTrue(cookies.hasKey("ThirdCookie==="), true, "Testing third cookie presence (different header)", &std::cout);
	assertTrue(cookies.getValue("ThirdCookie===").getAsString() == "yetAnotherValue ", true, "Testing third cookie value correctness (different header & multiple \';\' and \'=\')", &std::cout);
}

void test_HttpRequest_headers_multipleCookies_differentHeader_allMultipleSeparators_missingCRs()
{
	char request[] = "GET /my/path HTTP/1.1\nCookie:MyCookie===;;;SecondCookie==value;;\n  Cookie:;ThirdCookie====yetAnotherValue ;\nSecondHeader: anotherValue  \n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& cookies = httpRequest.getCookiesMap();
	const HttpImmutableMap& headers = httpRequest.getHeadersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if multiple cookie request is valid (different header & multiple \';\' and \'=\')", &std::cout);
	assertTrue(!headers.hasKey("Cookie"), true, "Testing the absence of Cookie header in the header map (different header & multiple \';\' and \'=\')", &std::cout);
	assertTrue(cookies.hasKey("MyCookie=="), true, "Testing first cookie presence (different header & multiple \';\')", &std::cout);
	assertTrue(cookies.getValue("MyCookie==").getAsString() == "", true, "Testing first cookie value correctness (different header & multiple \';\' and \'=\')", &std::cout);
	assertTrue(cookies.hasKey("SecondCookie="), true, "Testing second cookie presence (different header)", &std::cout);
	assertTrue(cookies.getValue("SecondCookie=").getAsString() == "value", true, "Testing second cookie value correctness (different header & multiple \';\' and \'=\')", &std::cout);
	assertTrue(cookies.hasKey("ThirdCookie==="), true, "Testing third cookie presence (different header)", &std::cout);
	assertTrue(cookies.getValue("ThirdCookie===").getAsString() == "yetAnotherValue ", true, "Testing third cookie value correctness (different header & multiple \';\' and \'=\')", &std::cout);
}

void test_HttpRequest_body_noHeaders()
{
	char request[] = "GET /my/path HTTP/1.1\r\n\r\nMyBody=value\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request with body is valid (no headers)", &std::cout);
	assertTrue(bodyParams.hasKey("MyBody"), true, "Testing presence of parameter MyBody (no headers)", &std::cout);
	assertTrue(bodyParams.getValue("MyBody").getAsString() == "value", true, "Testing MyBody value correctness (no headers)", &std::cout);
}

void test_HttpRequest_body_noHeaders_extraNewlineMissing()
{
	char request[] = "GET /my/path HTTP/1.1\r\nMyBody=value\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(!httpRequest.isValid(), true, "Testing if request with body is invalid (no headers & extra LF missing)", &std::cout);
}

void test_HttpRequest_body_singleNonCookie()
{
	char request[] = "GET /my/path HTTP/1.1\r\nMyHeader: value\r\n\r\nMyBody=value\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request with body is valid (single non-cookie)", &std::cout);
	assertTrue(bodyParams.hasKey("MyBody"), true, "Testing presence of parameter MyBody (single non-cookie)", &std::cout);
	assertTrue(bodyParams.getValue("MyBody").getAsString() == "value", true, "Testing MyBody value correctness (single non-cookie)", &std::cout);
}

void test_HttpRequest_body_singleCookie()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: key=value\r\n\r\nMyBody=value\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request with body is valid (single cookie)", &std::cout);
	assertTrue(bodyParams.hasKey("MyBody"), true, "Testing presence of parameter MyBody (single cookie)", &std::cout);
	assertTrue(bodyParams.getValue("MyBody").getAsString() == "value", true, "Testing MyBody value correctness (single cookie)", &std::cout);
}

void test_HttpRequest_body_multipleCookiesSingleHeader()
{
	char request[] = "GET /my/path HTTP/1.1\r\nCookie: key=value; key2=value2\r\n\r\nMyBody=value\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request with body is valid (multiple cookie single header)", &std::cout);
	assertTrue(bodyParams.hasKey("MyBody"), true, "Testing presence of parameter MyBody (multiple cookie single header)", &std::cout);
	assertTrue(bodyParams.getValue("MyBody").getAsString() == "value", true, "Testing MyBody value correctness (multiple cookie single header)", &std::cout);
}

void test_HttpRequest_body_noHeaders_multipleParameters()
{
	char request[] = "GET /my/path HTTP/1.1\r\n\r\nMyBody=value&AnotherKey=test\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request with body is valid (no headers & multiple params)", &std::cout);
	assertTrue(bodyParams.hasKey("MyBody"), true, "Testing presence of parameter MyBody (no headers & multiple params)", &std::cout);
	assertTrue(bodyParams.getValue("MyBody").getAsString() == "value", true, "Testing MyBody value correctness (no headers & multiple params)", &std::cout);
	assertTrue(bodyParams.hasKey("AnotherKey"), true, "Testing presence of parameter AnotherKey (no headers & multiple params)", &std::cout);
	assertTrue(bodyParams.getValue("AnotherKey").getAsString() == "test", true, "Testing AnotherKey value correctness (no headers & multiple params)", &std::cout);
}

void test_HttpRequest_body_multipleHeaders_multipleParameters()
{
	char request[] = "GET /my/path HTTP/1.1\r\nFirstHeader: value\r\nSecondHeader: value\r\n\r\nMyBody=value&AnotherKey=test\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request with body is valid (multi headers & multiple params)", &std::cout);
	assertTrue(bodyParams.hasKey("MyBody"), true, "Testing presence of parameter MyBody (multi headers & multiple params)", &std::cout);
	assertTrue(bodyParams.getValue("MyBody").getAsString() == "value", true, "Testing MyBody value correctness (multi headers & multiple params)", &std::cout);
	assertTrue(bodyParams.hasKey("AnotherKey"), true, "Testing presence of parameter AnotherKey (multi headers & multiple params)", &std::cout);
	assertTrue(bodyParams.getValue("AnotherKey").getAsString() == "test", true, "Testing AnotherKey value correctness (multi headers & multiple params)", &std::cout);
}

void test_HttpRequest_body_multipleHeaders_multipleParameters_spacesChaos()
{
	char request[] = "GET /my/path HTTP/1.1\r\nFir st Hea der:va lue\r\nSeco ndHea der : va lue\r\n\r\nMyB od y=va lu e&Anothe rKe y=te st \r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request with body is valid (multi headers & multiple params, \"space chaos\")", &std::cout);
	assertTrue(bodyParams.hasKey("MyB od y"), true, "Testing presence of parameter MyBody (multi headers & multiple params, \"space chaos\")", &std::cout);
	assertTrue(bodyParams.getValue("MyB od y").getAsString() == "va lu e", true, "Testing MyBody value correctness (multi headers & multiple params, \"space chaos\")", &std::cout);
	assertTrue(bodyParams.hasKey("Anothe rKe y"), true, "Testing presence of parameter AnotherKey (multi headers & multiple params, \"space chaos\")", &std::cout);
	assertTrue(bodyParams.getValue("Anothe rKe y").getAsString() == "te st ", true, "Testing AnotherKey value correctness (multi headers & multiple params, \"space chaos\")", &std::cout);
}

void test_HttpRequest_body_multipleHeaders_multipleParameters_spacesChaos_missingCRs()
{
	char request[] = "GET /my/path HTTP/1.1\nFir st Hea der:va lue\nSeco ndHea der : va lue\n\nMyB od y=va lu e&Anothe rKe y=te st \n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request with body is valid (multi headers & multiple params, \"space chaos\", no \\r)", &std::cout);
	assertTrue(bodyParams.hasKey("MyB od y"), true, "Testing presence of parameter MyBody (multi headers & multiple params, \"space chaos\", no \\r)", &std::cout);
	assertTrue(bodyParams.getValue("MyB od y").getAsString() == "va lu e", true, "Testing MyBody value correctness (multi headers & multiple params, \"space chaos\", no \\r)", &std::cout);
	assertTrue(bodyParams.hasKey("Anothe rKe y"), true, "Testing presence of parameter AnotherKey (multi headers & multiple params, \"space chaos\", no \\r)", &std::cout);
	assertTrue(bodyParams.getValue("Anothe rKe y").getAsString() == "te st ", true, "Testing AnotherKey value correctness (multi headers & multiple params, \"space chaos\", no \\r)", &std::cout);
}

void test_HttpRequest_body_multipleHeaders_multipleParameters_spacesChaos_missingCRs_extraSeparators()
{
	char request[] = "GET /my/path HTTP/1.1\nFir st Hea der:va lue\nSeco ndHea der : va lue\n\n&&MyB od y=va lu e&&&Anothe rKe y=te st &&\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request with body is valid (multi headers & multiple params, \"space chaos\", no \\r, more \'&\'')", &std::cout);
	assertTrue(bodyParams.hasKey("MyB od y"), true, "Testing presence of parameter MyBody (multi headers & multiple params, \"space chaos\", no \\r, more \'&\'')", &std::cout);
	assertTrue(bodyParams.getValue("MyB od y").getAsString() == "va lu e", true, "Testing MyBody value correctness (multi headers & multiple params, \"space chaos\", no \\r, , more \'&\')", &std::cout);
	assertTrue(bodyParams.hasKey("Anothe rKe y"), true, "Testing presence of parameter AnotherKey (multi headers & multiple params, \"space chaos\", no \\r, , more \'&\')", &std::cout);
	assertTrue(bodyParams.getValue("Anothe rKe y").getAsString() == "te st ", true, "Testing AnotherKey value correctness (multi headers & multiple params, \"space chaos\", no \\r, more \'&\')", &std::cout);
}

void test_HttpRequest_body_multipleHeaders_multipleParameters_noValue()
{
	char request[] = "GET /my/path HTTP/1.1\r\nFirstHeader: value\r\nSecondHeader: value\r\n\r\nMyBody=value&keywnovalue&AnotherKey=test\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request with body is valid (param /w no value)", &std::cout);
	assertTrue(bodyParams.hasKey("MyBody"), true, "Testing presence of parameter MyBody (param /w no value)", &std::cout);
	assertTrue(!bodyParams.hasKey("keywnovalue"), true, "Testing absence of parameter keywnovalue (has no value)(param /w no value)", &std::cout);
	assertTrue(bodyParams.getValue("MyBody").getAsString() == "value", true, "Testing MyBody value correctness (param /w no value)", &std::cout);
	assertTrue(bodyParams.hasKey("AnotherKey"), true, "Testing presence of parameter AnotherKey (param /w no value)", &std::cout);
	assertTrue(bodyParams.getValue("AnotherKey").getAsString() == "test", true, "Testing AnotherKey value correctness (param /w no value)", &std::cout);
}

void test_HttpRequest_body_multipleHeaders_multipleParameters_noValue_spacesChaos_extraSeparators()
{
	char request[] = "GET /my/path HTTP/1.1\r\nFirstHeader: value\r\nSecon dHe ader: v alu e\r\n\r\n&&&&MyBod y=v alue&&&keywno val ue&&&AnotherK ey=t est&\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request with body is valid (param /w no value, \"space chaos\", \'&\'')", &std::cout);
	assertTrue(bodyParams.hasKey("MyBod y"), true, "Testing presence of parameter MyBody (param /w no value, \"space chaos\", \'&\')", &std::cout);
	assertTrue(!bodyParams.hasKey("keywno val ue"), true, "Testing absence of parameter keywnovalue (has no value)(param /w no value, \"space chaos\", \'&\')", &std::cout);
	assertTrue(bodyParams.getValue("MyBod y").getAsString() == "v alue", true, "Testing MyBody value correctness (param /w no value, \"space chaos\", \'&\')", &std::cout);
	assertTrue(bodyParams.hasKey("AnotherK ey"), true, "Testing presence of parameter AnotherKey (param /w no value, \"space chaos\", \'&\')", &std::cout);
	assertTrue(bodyParams.getValue("AnotherK ey").getAsString() == "t est", true, "Testing AnotherKey value correctness (param /w no value, \"space chaos\", \'&\')", &std::cout);
}

void test_HttpRequest_body_multipleHeaders_multipleParameters_keyWithEmptyValue_extraSeparators()
{
	char request[] = "GET /my/path HTTP/1.1\r\nFirstHeader: value\r\nSecon dHe ader: v alu e\r\n\r\n&&&&somekey=\r\n";

	HttpRequest httpRequest(request);

	const HttpImmutableMap& bodyParams = httpRequest.getBodyParametersMap();

	assertTrue(httpRequest.isValid(), true, "Testing if request with body is valid (param /w no value, \"space chaos\", \'&\'')", &std::cout);
	assertTrue(bodyParams.hasKey("MyBod y"), true, "Testing presence of parameter MyBody (param /w no value, \"space chaos\", \'&\')", &std::cout);
	assertTrue(!bodyParams.hasKey("keywno val ue"), true, "Testing absence of parameter keywnovalue (has no value)(param /w no value, \"space chaos\", \'&\')", &std::cout);
	assertTrue(bodyParams.getValue("MyBod y").getAsString() == "v alue", true, "Testing MyBody value correctness (param /w no value, \"space chaos\", \'&\')", &std::cout);
	assertTrue(bodyParams.hasKey("AnotherK ey"), true, "Testing presence of parameter AnotherKey (param /w no value, \"space chaos\", \'&\')", &std::cout);
	assertTrue(bodyParams.getValue("AnotherK ey").getAsString() == "t est", true, "Testing AnotherKey value correctness (param /w no value, \"space chaos\", \'&\')", &std::cout);
}

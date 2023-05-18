#include "test-HttpRequest.h"

int main()
{
	test_HttpRequest_requestType();
	test_HttpRequest_requestType_extraSpaces();
	test_HttpRequest_pathToResource_valid();
	test_HttpRequest_pathToResource_withSpaces();
	test_HttpRequest_query_empty();
	test_HttpRequest_query_doubleQuestionMarkOnly();
	test_HttpRequest_query_ampersandOnly();
	test_HttpRequest_query_noKeyValueSeperator();
	test_HttpRequest_query_singleEmptyValueParameter();
	test_HttpRequest_query_singleEmptyValueParamAndAmpersand();
	test_HttpRequest_query_multipleEmptyValueParam();
	test_HttpRequest_query_multipleEmptyValueParamAndAmpersand();
	test_HttpRequest_query_multipleExistingParameters();
	test_HttpRequest_query_multipleExistingParametersAndAmpersand();
	test_HttpRequest_query_multipleExistingParametersAndMultipleSeparator();
	test_HttpRequest_query_fictionalParameter();
	test_HttpRequest_query_mixedParams();
	test_HttpRequest_protocolVersion_valid();
	test_HttpRequest_protocolVersion_valid_extraSpaces();
	test_HttpRequest_protocolVersion_valid_futureVersion();
	test_HttpRequest_protocolVersion_notANumber();
	test_HttpRequest_protocolVersion_incorrectName();
	test_HttpRequest_headers_singleNonCookie();
	test_HttpRequest_headers_multipleNonCookies();
	test_HttpRequest_headers_multipleNonCookies_spaceChaos();
	test_HttpRequest_headers_fictionalNonCookie();
	test_HttpRequest_headers_singleCookie();
	test_HttpRequest_headers_multipleCookies_sameHeader();
	test_HttpRequest_headers_multipleCookies_sameHeader_extraSpaces();
	test_HttpRequest_headers_multipleCookies_sameHeader_spaceInKey();
	test_HttpRequest_headers_multipleCookies_differentHeader();
	test_HttpRequest_headers_multipleCookies_differentHeader_extraSpaces();
	test_HttpRequest_headers_multipleCookies_differentHeader_spacesChaos();

	return 0;
}

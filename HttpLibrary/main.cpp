#include "test-HttpRequest.h"

int main()
{
	test_HttpRequest_requestType();
	test_HttpRequest_pathToResource();
	test_HttpRequest_query_empty();
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
	test_HttpRequest_protocolVersion_general();
	test_HttpRequest_protocolVersion_uncommon();
	test_HttpRequest_protocolVersion_notANumber();
	test_HttpRequest_headers_singleNonCookie();
	test_HttpRequest_headers_multipleNonCookies();
	test_HttpRequest_headers_fictionalNonCookie();

	return 0;
}

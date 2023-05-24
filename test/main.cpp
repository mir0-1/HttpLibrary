#include "test-HttpRequest.h"
#include "test-HttpResponseBuilder.h"

int main()
{
	std::cout << "[--------------------TESTING HttpRequest now--------------------]" << std::endl;
	test_HttpRequest_requestType();
	test_HttpRequest_requestType_extraSpaces();
	test_HttpRequest_pathToResource_valid_noExtension();
	test_HttpRequest_pathToResource_valid_extension();
	test_HttpRequest_pathToResource_valid_emptyExtension();
	test_HttpRequest_pathToResource_doubleSlash();
	test_HttpRequest_pathToResource_extensionWithSlash();
	test_HttpRequest_pathToResource_forbiddenChar();
	test_HttpRequest_pathToResource_withSpaces();
	test_HttpRequest_query_empty();
	test_HttpRequest_query_doubleQuestionMarkOnly();
	test_HttpRequest_query_ampersandOnly();
	test_HttpRequest_query_noKeyValueSeperator();
	test_HttpRequest_query_singleParam_emptyValue();
	test_HttpRequest_query_singleParam_multipleKeyValueSeparatorAndAmpersandAfter();
	test_HttpRequest_query_singleParam_multipleKeyValueSeparatorAndAmpersandBefore();
	test_HttpRequest_query_singleParam_emptyValueAndAmpersand();
	test_HttpRequest_query_multipleParams();
	test_HttpRequest_query_multipleParams_emptyValue();
	test_HttpRequest_query_multipleParams_emptyValueAndAmpersand();
	test_HttpRequest_query_multipleParams_ampersandAtEnd();
	test_HttpRequest_query_multipleParams_multipleSeparator();
	test_HttpRequest_query_multipleParams_fictionalParameter();
	test_HttpRequest_query_multipleParams_allSeperatorChaos();
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
	test_HttpRequest_headers_singleCookie_emptyValue();
	test_HttpRequest_headers_singleCookie_oneKeyMultipleValues();
	test_HttpRequest_headers_singleCookie_oneKeyMultipleValues_extraSeperators();
	test_HttpRequest_headers_multipleCookies_sameHeader();
	test_HttpRequest_headers_multipleCookies_sameHeader_noValue();
	test_HttpRequest_headers_multipleCookies_sameHeader_noValueNoSeparator();
	test_HttpRequest_headers_multipleCookies_sameHeader_noValueNoSeparatorAndOneValid();
	test_HttpRequest_headers_multipleCookies_sameHeader_extraSpaces();
	test_HttpRequest_headers_multipleCookies_sameHeader_spaceInKey();
	test_HttpRequest_headers_multipleCookies_differentHeader();
	test_HttpRequest_headers_multipleCookies_differentHeader_extraSpaces();
	test_HttpRequest_headers_multipleCookies_differentHeader_spacesChaos();
	test_HttpRequest_headers_multipleCookies_differentHeader_multipleSeparators();
	test_HttpRequest_headers_multipleCookies_differentHeader_allMultipleSeparators();
	test_HttpRequest_headers_multipleCookies_differentHeader_allMultipleSeparators_missingCRs();
	test_HttpRequest_body_noHeaders();
	test_HttpRequest_body_noHeaders_extraNewlineMissing();
	test_HttpRequest_body_singleNonCookie();
	test_HttpRequest_body_singleCookie();
	test_HttpRequest_body_multipleCookiesSingleHeader();
	test_HttpRequest_body_noHeaders_multipleParameters();
	test_HttpRequest_body_multipleHeaders_multipleParameters();
	test_HttpRequest_body_multipleHeaders_multipleParameters_spacesChaos();
	test_HttpRequest_body_multipleHeaders_multipleParameters_spacesChaos_missingCRs();
	test_HttpRequest_body_multipleHeaders_multipleParameters_spacesChaos_missingCRs_extraSeparators();
	test_HttpRequest_body_multipleHeaders_multipleParameters_noValue();
	test_HttpRequest_body_multipleHeaders_multipleParameters_noValue_spacesChaos_extraSeparators();

	std::cout << "[--------------------TESTING HttpResponseBuilder now--------------------]" << std::endl;
	test_HttpResponseBuilder_noHeaders_noBody();
	test_HttpResponseBuilder_noHeaders_helloWorldHtml();
	test_HttpResponseBuilder_someHeaders_helloWorldHtml();
	test_HttpResponseBuilder_someHeaders_someCookies_helloWorldHtml();
	test_HttpResponseBuilder_someHeaders_someCookies_jsonBody();
	test_HttpResponseBuilder_someHeaders_someCookies_jsonBodySingleQuotes();
	test_HttpResponseBuilder_someHeaders_someCookies_jsonBodyWithQuotes();
	test_HttpResponseBuilder_someHeaders_helloWorldHtml_overrideContentTypeViaHeaderMap();
	test_HttpResponseBuilder_someHeaders_jsonBody_overrideContentTypeViaSetter();
	test_HttpResponseBuilder_someHeaders_noCookies_htmlBody_overrideContentLength();

	return 0;
}

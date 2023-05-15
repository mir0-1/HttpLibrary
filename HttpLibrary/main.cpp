#include "test-HttpRequest.h"

int main()
{
	test_HttpRequest_requestType();
	test_HttpRequest_pathToResource();
	test_HttpRequest_multipleExistingParameters();
	test_HttpRequest_nullParameter();
	test_HttpRequest_protocolVersionGeneral();
	test_HttpRequest_protocolVersionUncommon();
	test_HttpRequest_nonCookieHeader();

	return 0;
}

#pragma once
#include "RequestType.h"
#include <iostream>

class HttpRequest
{
	private:
		static const char* recognizedRequests[];
		static const int numberOfRecognizedRequests;
		static const char* commonProtocolSubstring;

		bool valid;
		RequestType requestType;
		char* pathToResource;
		double protocolVersion;

	private:
		char* parseRequestType(char* src);
		char* copyPathToResource(char* src);
		char* parseProtocolVersion(char* src);
		char* parseParametersFromResourcePath(char* src);

	public:
		bool isValid() const;
		RequestType getRequestType() const;
		char* const getPathToResource() const;
		double getProtocolVersion() const;

		friend std::ostream& operator<<(std::ostream& ostream, const HttpRequest& httpRequest);

		HttpRequest(char* src);
};
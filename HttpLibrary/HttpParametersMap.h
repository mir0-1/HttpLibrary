#pragma once
#include <unordered_map>
#include <string>
#include "HttpParameterValue.h"

class HttpParametersBuilder;

class HttpParametersMap 
{
	private:
		std::unordered_map<std::string, HttpParameterValue> params;

	public:
		const HttpParameterValue& getParameter(const std::string& key) const;
		bool hasParameter(const std::string& key) const;

		friend class HttpParametersBuilder;
};
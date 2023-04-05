#include "HttpParametersMap.h"

const HttpParameterValue& HttpParametersMap::getParameter(const std::string& key) const
{
	static HttpParameterValue nullParameter;

	auto it = params.find(key);
	if (it != params.end())
		return it->second;
	return nullParameter;
}

bool HttpParametersMap::hasParameter(const std::string& key) const
{
	return params.find(key) != params.end();
}
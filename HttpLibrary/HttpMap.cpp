#include "HttpMap.h"

const HttpValue& HttpMap::getParameter(const std::string& key) const
{
	static HttpValue nullParameter;

	auto it = params.find(key);
	if (it != params.end())
		return it->second;
	return nullParameter;
}

bool HttpMap::hasParameter(const std::string& key) const
{
	return params.find(key) != params.end();
}

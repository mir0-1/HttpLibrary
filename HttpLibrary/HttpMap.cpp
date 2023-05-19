#include "HttpMap.h"

const HttpValue& HttpMap::getValue(const std::string& key) const
{
	static HttpValue nullKey;

	auto it = keys.find(key);
	if (it != keys.end())
		return it->second;
	return nullKey;
}

void HttpMap::setValue(const std::string& key, const HttpValue& value)
{
	keys[key] = value;
}

bool HttpMap::hasKey(const std::string& key) const
{
	return keys.find(key) != keys.end();
}

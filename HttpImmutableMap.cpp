#include "HttpImmutableMap.h"

bool HttpImmutableMap::hasKey(const std::string& key) const
{
	return map.hasKey(key);
}

const HttpValue& HttpImmutableMap::getValue(const std::string& key) const
{
	return map.getValue(key);
}

HttpImmutableMap::HttpImmutableMap(HttpMap& map)
	:	map(map)
{
}

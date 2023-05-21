#include "HttpImmutableMap.h"

bool HttpImmutableMap::hasKey(const std::string& key) const
{
	return map.hasKey(key);
}

const HttpValue& HttpImmutableMap::getValue(const std::string& key) const
{
	return map.getValue(key);
}

void HttpImmutableMap::getNextPairFromIterator(std::string& key, HttpValue& value)
{
	return map.getNextPairFromIterator(key, value);
}

void HttpImmutableMap::resetIterator()
{
	return map.resetIterator();
}

bool HttpImmutableMap::isIteratorAtEnd() const
{
	return map.isIteratorAtEnd();
}

HttpImmutableMap::HttpImmutableMap(HttpMap& map)
	:	map(map)
{
}

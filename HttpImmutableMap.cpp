#include "HttpImmutableMap.h"

bool HttpImmutableMap::hasKey(const std::string& key) const
{
	return map.hasKey(key);
}

const HttpValue& HttpImmutableMap::getValue(const std::string& key) const
{
	return map.getValue(key);
}

std::pair<std::string, HttpValue> HttpImmutableMap::getNextPairFromIterator()
{
	return map.getNextPairFromIterator();
}

void HttpImmutableMap::resetIterator()
{
	return map.resetIterator();
}

bool HttpImmutableMap::isIteratorAtEnd() const
{
	return map.isIteratorAtEnd();
}

bool HttpImmutableMap::isIteratorReset() const
{
	return map.isIteratorReset();
}

HttpImmutableMap::HttpImmutableMap(HttpMap& map)
	:	map(map)
{
}

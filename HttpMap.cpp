#include "HttpMap.h"

const HttpValue& HttpMap::getValue(const std::string& key) const
{
	static HttpValue nullKey;

	auto iter = map.find(key);
	if (iter != map.end())
		return iter->second;
	return nullKey;
}

void HttpMap::setValue(const std::string& key, const HttpValue& value)
{
	map[key] = value;
}

bool HttpMap::hasKey(const std::string& key) const
{
	return map.find(key) != map.end();
}

std::pair<std::string, HttpValue> HttpMap::getNextPairFromIterator()
{
	it++;

	if (isIteratorAtEnd())
		resetIterator();

	return std::make_pair(it->first, it->second);
}

bool HttpMap::isIteratorAtEnd() const
{
	return it == map.cend();
}

void HttpMap::resetIterator()
{
	it = map.cbegin();
}

bool HttpMap::isIteratorReset() const
{
	return it == map.cbegin();
}

HttpMap::HttpMap()
{
	resetIterator();
}

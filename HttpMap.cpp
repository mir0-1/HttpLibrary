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

void HttpMap::getNextPairFromIterator(std::string& key, HttpValue& value)
{
	key = std::string(it->first);
	value = it->second;

	it++;
	if (isIteratorAtEnd())
		resetIterator();
	
}

bool HttpMap::isIteratorAtEnd() const
{
	return it == map.end();
}

void HttpMap::resetIterator()
{
	it = map.begin();
}

HttpMap::HttpMap()
{
	it = map.begin();
}

HttpMap::~HttpMap()
{
	
}

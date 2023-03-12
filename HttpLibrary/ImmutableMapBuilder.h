#pragma once
#include "ImmutableMap.h"

template <class Key, class Value>
class ImmutableMapBuilder
{
	private:
		bool built;
		ImmutableMap<Key, Value> slave;

	public:
		bool isBuilt();
		void add(const Key& key, const Value& value);
		ImmutableMap<Key, Value>& build();

		ImmutableMapBuilder<Key, Value>();
};

template <class Key, class Value>
bool ImmutableMapBuilder<Key, Value>::isBuilt()
{
	return built;
}

template <class Key, class Value>
void ImmutableMapBuilder<Key, Value>::add(const Key& key, const Value& value)
{
	if (!isBuilt())
		slave.contents[key] = value;
}

template <class Key, class Value>
ImmutableMap<Key, Value>& ImmutableMapBuilder<Key, Value>::build()
{
	built = true;
	return slave;
}

template <class Key, class Value>
ImmutableMapBuilder<Key, Value>::ImmutableMapBuilder() {}
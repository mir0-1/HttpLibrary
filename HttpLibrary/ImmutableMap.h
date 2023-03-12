#pragma once
#include <map>
#include <iostream>

template <class Key, class Value>
class ImmutableMapBuilder;

template <class Key, class Value>
class ImmutableMap
{
	private:
		std::map<Key, Value> contents;

	public:
		const Value& getByKey(const Key& key) const;

		friend std::ostream& operator<<(std::ostream& ostream, const ImmutableMap<Key, Value>& immutableMap)
		{
			ostream << "{" << std::endl;

			for (auto it = immutableMap.contents.begin() ; it != immutableMap.contents.end() ; it++)
				ostream << it->first << ": " << it->second << std::endl;

			return ostream << "}";
		}


		friend class ImmutableMapBuilder<Key, Value>;
};

template <class Key, class Value>
const Value& ImmutableMap<Key, Value>::getByKey(const Key& key) const
{
	return contents.find(key)->second;
}

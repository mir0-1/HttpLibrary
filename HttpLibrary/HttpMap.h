#pragma once
#include <unordered_map>
#include <string>
#include "HttpValue.h"

class HttpMapBuilder;

class HttpMap
{
	private:
		std::unordered_map<std::string, HttpValue> keys;

	public:
		const HttpValue& getValue(const std::string& key) const;
		void setValue(const std::string& key, const HttpValue& value);
		bool hasKey(const std::string& key) const;

		friend class HttpMapBuilder;
};

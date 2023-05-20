#pragma once
#include "HttpMap.h"

class HttpImmutableMap
{
	private:
		HttpMap& map;

	public:
		bool hasKey(const std::string& key) const;
		const HttpValue& getValue(const std::string& key) const;

		HttpImmutableMap(HttpMap& map);
};

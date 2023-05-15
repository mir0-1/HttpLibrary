#pragma once
#include <unordered_map>
#include <string>
#include "HttpValue.h"

class HttpMapBuilder;

class HttpMap
{
	private:
		std::unordered_map<std::string, HttpValue> params;

	public:
		const HttpValue& getParameter(const std::string& key) const;
		bool hasParameter(const std::string& key) const;

		friend class HttpMapBuilder;
};

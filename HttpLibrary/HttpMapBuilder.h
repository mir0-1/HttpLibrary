#pragma once
#include "HttpMap.h"
#include "HttpValue.h"

class HttpMapBuilder
{
	private:
		HttpMap container;
		bool locked = false;

	public:
		void setValue(const std::string& key, const HttpValue& value);
		HttpMap& getContainer();
};

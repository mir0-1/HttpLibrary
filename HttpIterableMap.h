#pragma once
#include <string>
#include <unordered_map>
#include "HttpValue.h"

class HttpIterableMap
{
	public:
		virtual void getNextPairFromIterator(std::string& key, HttpValue& value) = 0;
		virtual void resetIterator() = 0;
		virtual bool isIteratorAtEnd() const = 0;
};

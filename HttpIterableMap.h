#pragma once
#include <string>
#include <unordered_map>
#include "HttpValue.h"

class HttpIterableMap
{
	public:
		virtual bool hasKey(const std::string& key) const = 0;
		virtual std::pair<std::string, HttpValue> getNextPairFromIterator() = 0;
		virtual void resetIterator() = 0;
		virtual bool isIteratorReset() const = 0;
		virtual bool isIteratorAtEnd() const = 0;
};

#pragma once
#include "HttpIterableMap.h"

class HttpMap : public HttpIterableMap
{
	private:
		std::unordered_map<std::string, HttpValue> map;
		std::unordered_map<std::string, HttpValue>::const_iterator it;

	public:
		virtual const HttpValue& getValue(const std::string& key) const;
		virtual void setValue(const std::string& key, const HttpValue& value);

		virtual bool hasKey(const std::string& key) const override;
		std::pair<std::string, HttpValue> getNextPairFromIterator() override;
		virtual void resetIterator() override;
		virtual bool isIteratorReset() const override;
		virtual bool isIteratorAtEnd() const override;

		HttpMap();
};

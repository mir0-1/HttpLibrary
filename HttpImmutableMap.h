#pragma once
#include "HttpMap.h"

class HttpImmutableMap : public HttpIterableMap
{
	private:
		HttpMap& map;

	public:
		const HttpValue& getValue(const std::string& key) const;

		virtual bool hasKey(const std::string& key) const override;
		virtual std::pair<std::string, HttpValue> getNextPairFromIterator() override;
		virtual void resetIterator() override;
		virtual bool isIteratorReset() const override;
		virtual bool isIteratorAtEnd() const override;

		HttpImmutableMap(HttpMap& map);
};

#pragma once
#include "HttpMap.h"

class HttpImmutableMap : public HttpIterableMap
{
	private:
		HttpMap& map;

	public:
		bool hasKey(const std::string& key) const;
		const HttpValue& getValue(const std::string& key) const;

		virtual void getNextPairFromIterator(std::string& key, HttpValue& value) override;
		virtual void resetIterator() override;
		virtual bool isIteratorAtEnd() const override;

		HttpImmutableMap(HttpMap& map);
};

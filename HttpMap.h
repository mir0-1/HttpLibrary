#pragma once
#include "HttpIterableMap.h"

class HttpMap : public HttpIterableMap
{
	private:
		std::unordered_map<std::string, HttpValue> map;
		std::unordered_map<std::string, HttpValue>::iterator it;

	public:
		virtual const HttpValue& getValue(const std::string& key) const;
		virtual void setValue(const std::string& key, const HttpValue& value);
		virtual bool hasKey(const std::string& key) const;

		virtual void getNextPairFromIterator(std::string& key, HttpValue& value) override;
		virtual void resetIterator() override;
		virtual bool isIteratorAtEnd() const override;

		HttpMap();
		~HttpMap();
};

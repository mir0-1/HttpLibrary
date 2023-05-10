#pragma once
#include "HttpMap.h"

template <class T>
class HttpMapBuilder
{
	private:
		HttpMap<T> container;
		bool locked = false;

	public:
		void setValue(const std::string& key, const T& value);
		HttpMap<T>& getContainer();
};

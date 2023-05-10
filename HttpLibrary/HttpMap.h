#pragma once
#include <unordered_map>
#include <string>
#include "HttpParameterValue.h"

template <class T>
class HttpMapBuilder;

template <class T>
class HttpMap
{
	private:
		std::unordered_map<std::string, T> params;

	public:
		const T& getParameter(const std::string& key) const;
		bool hasParameter(const std::string& key) const;

		friend class HttpMapBuilder<T>;
};

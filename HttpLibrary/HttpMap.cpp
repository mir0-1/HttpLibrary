#include "HttpMap.h"

template <class T>
const T& HttpMap<T>::getParameter(const std::string& key) const
{
	static HttpParameterValue nullParameter;

	auto it = params.find(key);
	if (it != params.end())
		return it->second;
	return nullParameter;
}

template <class T>
bool HttpMap<T>::hasParameter(const std::string& key) const
{
	return params.find(key) != params.end();
}

template class HttpMap<HttpParameterValue>;

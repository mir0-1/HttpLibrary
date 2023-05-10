#include "HttpMapBuilder.h"

template <class T>
void HttpMapBuilder<T>::setValue(const std::string& key, const T& value)
{
	if (!locked)
		container.params[key] = value;
}

template <class T>
HttpMap<T>& HttpMapBuilder<T>::getContainer()
{
	locked = true;
	return container;
}

template class HttpMapBuilder<HttpParameterValue>;

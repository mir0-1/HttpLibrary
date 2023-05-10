#include "HttpMapBuilder.h"

void HttpMapBuilder::setValue(const std::string& key, const HttpParameterValue& value)
{
	if (!locked)
		container.params[key] = value;
}

HttpMap<T>& HttpMapBuilder::getContainer()
{
	locked = true;
	return container;
}

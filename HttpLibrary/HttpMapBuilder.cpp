#include "HttpMapBuilder.h"

void HttpMapBuilder::setValue(const std::string& key, const HttpValue& value)
{
	if (!locked)
		container.keys[key] = value;
}

HttpMap& HttpMapBuilder::getContainer()
{
	locked = true;
	return container;
}

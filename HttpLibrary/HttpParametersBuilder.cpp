#include "HttpParametersBuilder.h"

void HttpParametersBuilder::setParameter(const std::string& key, const HttpParameterValue& value)
{
	if (!locked)
		container.params[key] = value;
}

HttpParametersMap& HttpParametersBuilder::getContainer()
{
	locked = true;
	return container;
}

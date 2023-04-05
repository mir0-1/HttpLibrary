#include "HttpParameterValue.h"

std::string HttpParameterValue::getAsString() const
{
	return value;
}

int HttpParameterValue::getAsInt() const
{
	return atoi(value.c_str());
}

bool HttpParameterValue::getAsBool() const
{
	return (bool)getAsInt();
}

bool HttpParameterValue::isNull() const
{
	return nullReference;
}

HttpParameterValue::HttpParameterValue(const std::string& value)
{
	this->value = value;
	nullReference = false;
}

HttpParameterValue::HttpParameterValue()
{
	nullReference = true;
}

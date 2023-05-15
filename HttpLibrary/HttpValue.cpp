#include "HttpValue.h"

std::string HttpValue::getAsString() const
{
	return value;
}

int HttpValue::getAsInt() const
{
	return atoi(value.c_str());
}

bool HttpValue::getAsBool() const
{
	return (bool)getAsInt();
}

bool HttpValue::isNull() const
{
	return nullReference;
}

HttpValue::HttpValue(const std::string& value)
{
	this->value = value;
	nullReference = false;
}

HttpValue::HttpValue()
{
	nullReference = true;
}

#include "HttpParameter.h"
#include <stdlib.h>

const char* HttpParameter::getValue() const
{
	return value;
}

int HttpParameter::getValueAsInt() const
{
	return atoi(value);
}

bool HttpParameter::getValueAsBool() const
{
	return (bool)getValueAsInt();
}

HttpParameter::HttpParameter(char* value): value(value) {}

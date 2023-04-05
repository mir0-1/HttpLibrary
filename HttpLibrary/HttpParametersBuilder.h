#pragma once
#include "HttpParametersMap.h"

class HttpParametersBuilder
{
	private:
		HttpParametersMap container;
		bool locked = false;

	public:
		void setParameter(const std::string& key, const HttpParameterValue& value);
		HttpParametersMap& getContainer();
};
